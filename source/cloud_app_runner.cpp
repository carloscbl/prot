
#include "cloud_app_runner.h"
#include "time_determinator.h"
#include "db_ops.hpp"
#include "time_utils.hpp"

cloud_app_runner::cloud_app_runner(user & user_, app &app_)
    : user_(user_),
      app_(app_),
      user_apps_id(get_user_apps_id(this->user_.get_id() , this->app_.get_id()))
{

}

cloud_app_runner::cloud_app_runner(user & user_, app &app_, uint64_t user_apps_id)
    : user_(user_),
      app_(app_),
      user_apps_id(user_apps_id)
{

}

string cloud_app_runner::get_unique_id_session() const noexcept
{
    return user_.get_name() + app_.get_app_name();
}

//FIX: this is a mess composing responses...
const json cloud_app_runner::run(const json &request_json) noexcept
{
    const auto &state = fetch_next_session();
    this->m_session_id = state->id;
    app_parser fp(app_.get_json(), *state); //,*state
    unique_ptr<next_question_data_and_taskstory_input> response;
    if (request_json.is_null()) // Get, for get current state, for example for resume questionary
    {
        response = fp.get_current_question();
    }
    else
    {
        response = fp.app_next_in_pipeline(request_json["answer"]);
    }
    if(response->next_question_text == "END"){
        delete_session(state->id);
    }else{
        update_session(state->id, *fp.get_state());
    }

    json response_j;
    response_j["next_question"] = response->next_question_text;
    response_j["data_type"] = response->current_question_obj["type_user_input"];
    response_j["type_details"] = response->current_question_obj["type_details"];
    //TODO: Add pass the taskstory and the parsed variables to the user scheduler

    if (response->taskstory_name.empty())
    {
        return response_j;
    }
    
    ////////////////////////////////////////////////////////
    //Build schedulers and taskers for the given taskstory
    ////////////////////////////////////////////////////////
    try
    {
        expand_taskstory_t ep (*response);
        ep.expand_and_set();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    if(schedule_taskstory(*response))
    {
        // if we have successfully completed the process means we have to apply domain logic
        // Delete same user_app tasks already scheduled
        // this->user_apps_id // To search for the apps
        // this->m_session_id  // To search for the apps
        auto tasks_to_delete =  search_tasks_by_not_this_session_id(this->m_session_id, this->user_apps_id);
        vector<uint64_t> to_delete_ids;
        to_delete_ids.reserve(tasks_to_delete.size());
        std::for_each(tasks_to_delete.cbegin(),tasks_to_delete.cend(), [&to_delete_ids](const auto & pair){
            fmt::print("storing to deleting task {}",pair.first);
            to_delete_ids.push_back( pair.first );
        });
        delete_task(to_delete_ids);
    }else{

    }

    return response_j;
}

struct failure_report_t{
    vector<task_t> failures_report;
    task_t no_margin_invalidation;
};

bool cloud_app_runner::schedule_taskstory(next_question_data_and_taskstory_input & response){

    failure_report_t report;
    for (size_t day = 0; day < 365; day++)
    {
        provisional_scheduler_RAII provisional_scheduler = this->user_.get_scheduler().get_provisional();
        tasker &tasker_ = static_cast<tasker &>(this->user_.get_tasker());
        taskstory_commit_RAII commiter(response.taskstory_name, tasker_);

        bool complete = true;

        for (const auto &v : *response.non_wildcard_expanded_taskstory)
        {
            task_t task_test = make_shared<task>(v.get<task>());
            task_test->inner_json["app_id"] = this->app_.get_id();
            task_test->set_user(this->user_.get_name());
            task_test->set_user_apps_id(user_apps_id);
            task_test->set_session_id(m_session_id);
            time_determinator time_dt(task_test, provisional_scheduler);
            //cout << "checking task: " << task_test->get_tag() << " day:" << day << endl;
            optional<bool> result = time_dt.build(days(day));
            if(result.has_value() ){
                if(result.value()){
                    //task_test->save();
                    tasker_.add_to_group(move(task_test), response.taskstory_name);
                }else{
                    report.failures_report.push_back(task_test);
                    complete = false;
                }
            }else{//Now time to make it fail and control the failure
                report.no_margin_invalidation = task_test;
                return false;
            }
        }

        if(complete){
            commiter.commit(); //Disolves group && activate the tasks
            provisional_scheduler.commit();
            break;
        }
    }
    apply_wildcards(response);

    return true;
}


void cloud_app_runner::apply_wildcards(next_question_data_and_taskstory_input & response){

    failure_report_t report;
    for (const auto &[k,v] : *response.wildcard_expanded_taskstory)
    {
        provisional_scheduler_RAII provisional_scheduler = this->user_.get_scheduler().get_provisional();
        tasker &tasker_ = static_cast<tasker &>(this->user_.get_tasker());
        taskstory_commit_RAII commiter(response.taskstory_name, tasker_);
        bool completed_period = true;
        for (const auto & wildcard_task : v){
            task_t task_test = make_shared<task>(wildcard_task.get<task>());
            task_test->inner_json["app_id"] = this->app_.get_id();
            task_test->set_user(this->user_.get_name());
            task_test->set_user_apps_id(user_apps_id);
            task_test->set_session_id(m_session_id);
            string tag = task_test->get_tag();
            time_determinator time_dt(task_test, provisional_scheduler, k);
            //cout << "checking task: " << task_test->get_tag() << " day:" << day << endl;
            optional<bool> result = time_dt.build(days(0));
            if(result.has_value() ){
                if(result.value()){
                    //task_test->save();
                    fmt::print("-added_to_group {}\n", tag);
                    tasker_.add_to_group(move(task_test), response.taskstory_name);
                }else{
                    fmt::print("-Unable to schedule {}\n", tag);
                    report.failures_report.push_back(task_test);
                    completed_period = false;
                    break;
                }
            }else{//Now time to make it fail and control the failure
                fmt::print("-Failed time determination {}\n", tag);
                report.no_margin_invalidation = task_test;
                break;
                // return false;
            }
        }
        if(completed_period){
            commiter.commit(); //Disolves group && activate the tasks
            provisional_scheduler.commit();
        }
    }
}

shared_ptr<app_state> cloud_app_runner::get_session() const noexcept
{
    //This needs to come from db
    const auto &&state = read_session(this->user_.get_name() , this->app_.get_id() );
    if (!state){
        return create_session(this->user_.get_id() , this->app_.get_id() );
    }
    return state;
}

shared_ptr<app_state> cloud_app_runner::fetch_next_session() const noexcept
{
    auto session = get_session();
    if(session->next_branch_id < 0){
        return create_session(this->user_.get_id() , this->app_.get_id() );
    }
    return session;
}

