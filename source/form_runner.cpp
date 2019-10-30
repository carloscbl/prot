
#include "form_runner.h"
#include "time_determinator.h"
#include "relocation_group.h"

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
    provisional_scheduler_RAII provisional_scheduler = this->user_->get_scheduler().get_provisional();
    tasker &tasker_ = static_cast<tasker &>(this->user_->get_tasker());
    taskstory_commit_RAII commiter(response->taskstory_name, tasker_);
    relocation_group fail_to_alocate (response->taskstory_name,provisional_scheduler);
    bool fail = false;

    for (const auto &[k, v] : response->taskstory_json.items())
    {
        task_t task_test = make_shared<task>(v.get<task>());
        time_determinator time_dt(task_test, provisional_scheduler);
        cout << "checking task: " << task_test->get_tag() << endl;

        if(!fail){
            //asuring not build after a fail, but keep iterating
            fail = !time_dt.build();
        }
        if(!fail){
            tasker_.add_to_group(move(task_test), response->taskstory_name);
        }else{//Now time to make it fail and control the failure
            fail_to_alocate.add(task_test);
        }
    }

    commiter.commit(); //Disolves group && activate the tasks

    return response_j;
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
