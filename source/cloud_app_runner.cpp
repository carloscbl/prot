
#include "cloud_app_runner.h"
#include "time_determinator.h"
#include "db_ops.hpp"

cloud_app_runner::cloud_app_runner(user & user_, form &form_)
    : user_(user_),
      form_(form_),
      user_forms_id(get_user_forms_id(this->user_.get_id() , this->form_.get_id()))
{

}

string cloud_app_runner::get_unique_id_session() const noexcept
{
    return user_.get_name() + form_.get_form_name();
}

//FIX: this is a mess composing responses...
const json cloud_app_runner::run(const json &request_json) noexcept
{
    const auto &state = fetch_next_session();
    app_parser fp(form_.get_json(), *state); //,*state
    unique_ptr<next_question_data_and_taskstory_input> response;
    if (request_json.is_null()) // Get, for get current state, for example for resume questionary
    {
        response = fp.get_current_question();
    }
    else
    {
        response = fp.form_next_in_pipeline(request_json["answer"]);
    }
    if(response->question_str == "END"){
        delete_session(state->id);
        // this->user_running_forms.erase(this->get_unique_id_session());
    }else{
        update_session(state->id, *fp.get_state());
    }

    json response_j;
    response_j["next_question"] = response->question_str;
    //TODO: Add pass the taskstory and the parsed variables to the user scheduler

    if (response->taskstory_json.empty())
    {
        return response_j;
    }
    
    ////////////////////////////////////////////////////////
    //Build schedulers and taskers for the given taskstory
    ////////////////////////////////////////////////////////
    if(!perform_taskstory(*response)){
        //Set response_j error message
    }

    return response_j;
}

struct failure_report_t{
    vector<task_t> failures_report;
    task_t no_margin_invalidation;
};

bool cloud_app_runner::perform_taskstory(next_question_data_and_taskstory_input & response){

    failure_report_t report;
    for (size_t day = 0; day < 365; day++)
    {
        provisional_scheduler_RAII provisional_scheduler = this->user_.get_scheduler().get_provisional();
        tasker &tasker_ = static_cast<tasker &>(this->user_.get_tasker());
        taskstory_commit_RAII commiter(response.taskstory_name, tasker_);

        bool complete = true;

        for (const auto &[k, v] : response.taskstory_json.items())
        {
            task_t task_test = make_shared<task>(v.get<task>());
            task_test->inner_json["app_id"] = this->form_.get_id();
            task_test->set_user(this->user_.get_name());
            task_test->set_user_forms_id(user_forms_id);
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

    return true;
}

shared_ptr<form_state> cloud_app_runner::get_session() const noexcept
{
    //This needs to come from db
    const auto &&state = read_session(this->user_.get_name() , this->form_.get_id() );
    if (!state){
        return create_session(this->user_.get_id() , this->form_.get_id() );
    }
    return state;
}

shared_ptr<form_state> cloud_app_runner::fetch_next_session() const noexcept
{
    auto session = get_session();
    if(session->next_branch_id < 0){
        return create_session(this->user_.get_id() , this->form_.get_id() );
    }
    return session;
}

