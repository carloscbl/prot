
#include "cloud_app_runner.h"
#include "time_determinator.h"
#include "db_ops.hpp"
#include "time_utils.hpp"
#include "spdlog/spdlog.h"
// SPDLOG_INFO("Welcome to spdlog!");
// SPDLOG_ERROR("Welcome to spdlog!");

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

bool cloud_app_runner::store_qa_history_status( json addition) const {
    // need to detect an already "done": true, maybe we are reconfiguring
    // So we need to detect it and delete it -> always
    auto instl = db_op::read_user_instalations( this->user_.get_id(), this->app_.get_id());
    auto tuple = move(*instl.begin());
    json new_qa_history = json::object();
    auto qa_history_it = tuple->find("qa_history");
    if( qa_history_it != tuple->end() && !tuple->is_null()){
        // exists so we use it
        new_qa_history = qa_history_it.value();
    }
    cout << "existing" << new_qa_history.dump(4) << endl;
    new_qa_history.erase("done");
    cout << "addition" << addition.dump(4) << endl;

    auto add_history = addition.find("history");
    if(add_history != addition.end() ){
        for (auto &&i : add_history.value())
        {
            new_qa_history["history"].push_back(i);
        }
        addition.erase("history");
    }
    new_qa_history.merge_patch(addition);
    cout << "inserting" << new_qa_history.dump(4) << endl;

    auto is_updated = db_op::update_user_instalations(this->user_.get_id(), this->app_.get_id(), new_qa_history);
    return is_updated;
}

//FIX: this is a mess composing responses...
const json cloud_app_runner::run(const json &request_json) noexcept
{
    measure_execution_raii("cloud_app_runner::run");
    const auto &state = fetch_next_session();
    this->m_session_id = state->id;
    app_parser fp(app_.get_json(), *state); //,*state
    unique_ptr<next_question_data_and_taskstory_input> response;
    // auto current_question = fp.get_current_question();
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
        // App qa_history done!
        json add_qa_history = {
            {"done",true},
            {"history", { 
                    { 
                        {"question", response->current_question_obj["question"].get<string>()},
                        {"user_answer", request_json["answer"]}
                    } 
                }
            },
        };
        store_qa_history_status(add_qa_history);
    }else{
        update_session(state->id, *fp.get_state());
        if(!request_json.is_null()){
            json qa = {
                { 
                    {"question", response->current_question_obj["question"].get<string>()},
                    {"user_answer", request_json["answer"]}
                }
            };
            json add_qa_history = {
                //insert here the Q/A structure
                {"history", qa },
            };
            store_qa_history_status(add_qa_history);
        }
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

optional<days> deduce_next_period(const json & j_task, task * task_, optional<std::chrono::time_point<system_clock>> start_from){
    task * tk = task_;
    if( !tk){
        from_json(j_task,*tk);
    }
    days next_period;
    auto future = std::chrono::floor<days>(start_from.value());
    auto now = std::chrono::floor<days>(std::chrono::system_clock::now());
    cout << "days since epoch (future) " << duration_cast<days>(seconds(future.time_since_epoch())).count() << endl; 
    cout << "days since epoch (now) " << duration_cast<days>(seconds(now.time_since_epoch())).count() << endl;
    
    if (now > future){
        cout << "Trying to deduce next period..." << endl;
        auto ratio_opt = task_space::get_period_ratio(*tk);
        if(!ratio_opt.has_value()){ return nullopt; }
        days ratio = duration_cast<days>(ratio_opt.value());
        days offset = now - future; //24 - 22 = 2 days
        int period_multiplier = floorf64x(offset / ratio) + 1; // floor(2 / 7days) + 1 = 1
        future = (period_multiplier * ratio) + future ;
        if(now > future){
            cout << "ERROR Task cannot be schduled in the past! in(DAYS): " << std::chrono::floor<days>(future - now).count() << endl;
            return nullopt;
        }
        cout << "Success deducing!" << endl;
    }
    next_period = std::chrono::floor<days>(future - now);
    return next_period;
}

bool cloud_app_runner::schedule_single_task(const json & j_task, optional<std::chrono::time_point<system_clock>> start_from, task * task_) const{
    cout << "## schedule_single_task ##" << endl;
    provisional_scheduler_RAII provisional_scheduler = this->user_.get_scheduler().get_provisional();
    tasker &tasker_ = static_cast<tasker &>(this->user_.get_tasker());
    auto task_test = create_task_to_schedule(j_task);

    optional<days> projected_next_period_override_start_offset = nullopt;
    if(start_from.has_value()){
        projected_next_period_override_start_offset = deduce_next_period(j_task, task_, start_from);
        if(!projected_next_period_override_start_offset.has_value()){
            // cout << "WRONG projected_next_period_override_start_offset" << endl;
            SPDLOG_ERROR("WRONG projected_next_period_override_start_offset");
            return false;
        }
    }
    
    time_determinator time_dt(task_test, provisional_scheduler);
    optional<bool> result = time_dt.build( days(0), projected_next_period_override_start_offset );
    if(!result.has_value() || !result.value()){
        cout << "WRONG" << endl;
        return false;
    }
    tasker_.commit_single_task(move(task_test));
    cout << "COMMITED" << endl;
    return true;
}

task_t cloud_app_runner::create_task_to_schedule(const json & j_task) const{
    task_t task_test = make_shared<task>(j_task.get<task>());
    task_test->inner_json["app_id"] = this->app_.get_id();
    task_test->set_user(this->user_.get_name());
    task_test->set_user_apps_id(user_apps_id);
    task_test->set_session_id(m_session_id);
    return task_test;
}

void cloud_app_runner::register_runner_scheduled_tasks (std::shared_ptr<std::map<std::string, task_t>> done_tasks){
    if(!done_tasks){
        return;
    }
    optional<vector<task_t>>  current = nullopt;
    if(this->scheduled_tasks.has_value()){
        current = this->scheduled_tasks.value();
    }else{
        current =vector<task_t>();
    }
    for_each(done_tasks->begin(), done_tasks->end(),[&current](const pair<string,task_t>& p){
        current.value().push_back(p.second);
    });
    if(! current.value().empty()){
        this->scheduled_tasks = current;
    }
}

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
            task_t task_test = create_task_to_schedule(v);
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
            auto done_tasks = commiter.get_group();
            register_runner_scheduled_tasks(done_tasks);
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
            task_t task_test = create_task_to_schedule(wildcard_task.get<task>());
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
            auto done_tasks = commiter.get_group();
            register_runner_scheduled_tasks(done_tasks);
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

