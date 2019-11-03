
#include "form_runner.h"
#include "time_determinator.h"

form_runner::form_runner(shared_ptr<user> user_, form &form_)
    : user_(user_),
      form_(form_)
{}

string form_runner::get_unique_id_session() const noexcept
{
    return user_->get_name() + form_.name;
}

//FIX: this is a mess composing responses...
const json form_runner::run(const json &request_json) noexcept
{
    const auto &state = fetch_next_session();
    form_parser fp(form_.get_json(), *state); //,*state
    unique_ptr<next_question_data> response;
    if (request_json.is_null())
    {
        response = fp.get_initial_question();
    }
    else
    {
        response = fp.form_next_in_pipeline(request_json["answer"]);
    }
    if(response->question_str == "END"){
        this->user_running_forms.erase(this->get_unique_id_session());
    }
    user_running_forms[get_unique_id_session()] = fp.get_state();
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

bool form_runner::perform_taskstory(next_question_data & response){

    
    for (size_t day = 0; day < 365; day++)
    {
        provisional_scheduler_RAII provisional_scheduler = this->user_->get_scheduler().get_provisional();
        tasker &tasker_ = static_cast<tasker &>(this->user_->get_tasker());
        taskstory_commit_RAII commiter(response.taskstory_name, tasker_);

        bool complete = true;

        for (const auto &[k, v] : response.taskstory_json.items())
        {
            task_t task_test = make_shared<task>(v.get<task>());
            time_determinator time_dt(task_test, provisional_scheduler);
            cout << "checking task: " << task_test->get_tag() << " day:" << day << endl;
            optional<bool> result = time_dt.build(days(day));
            if(result.has_value() ){
                if(result.value()){
                    tasker_.add_to_group(move(task_test), response.taskstory_name);
                }else{
                    complete = false;
                }
            }else{//Now time to make it fail and control the failure
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

shared_ptr<form_state> form_runner::get_session() const noexcept
{
    auto session_id = get_unique_id_session();
    const auto &&state = form_runner::user_running_forms.find(session_id);
    if (state != form_runner::user_running_forms.end())
    {
        return state->second;
    }
    else
    {
        return new_session(session_id);
    }
}

shared_ptr<form_state> form_runner::fetch_next_session() const noexcept
{
    auto session = get_session();
    if(session->next_branch_id < 0){
        return new_session();
    }
    return session;
}

shared_ptr<form_state> form_runner::new_session() const noexcept
{
    auto session_id = get_unique_id_session();
    auto new_state = make_shared<form_state>();
    form_runner::user_running_forms[session_id] = new_state;
    return new_state;
}

shared_ptr<form_state> form_runner::new_session(const string & session_id) const noexcept
{
        auto new_state = make_shared<form_state>();
        form_runner::user_running_forms[session_id] = new_state;
        return new_state;
}
