
#include "form_runner.h"
#include "time_determinator.h"

form_runner::form_runner(shared_ptr<user> user_, form_t &form_)
    : user_(user_),
      form_(form_)
{

    //fp = make_unique<form_parser>(state);
    //Create thread
    //Check serialiced session or create a new one
    //Perform or traverse the questionary

    //run();
    // thread th([this](){
    // });
}

form_runner::~form_runner() {}

string form_runner::get_unique_id_session() const noexcept
{
    return user_->get_name() + form_.name;
}

//FIX: this is a mess composing responses...
const json form_runner::run(const json &request_json) noexcept
{
    const auto &state = get_session();
    form_parser fp(form_.get_json(), *state); //,*state
    unique_ptr<next_question_data> response;
    if (request_json.is_null())
    {
        response = fp.get_initial_question();
    }
    else
    {
        //cout << request_json.dump(4) << endl;
        response = fp.form_next_in_pipeline(request_json["answer"]);
    }
    user_running_forms[get_unique_id_session()] = fp.get_state();
    json response_j;
    response_j["next_question"] = response->question_str;
    //TODO: Add pass the taskstory and the parsed variables to the user scheduler

    if (response->taskstory_json.empty())
    {
        return response_j;
    }

    provisional_scheduler_RAII provisional_scheduler = this->user_->get_scheduler().get_provisional();
    tasker &tasker_ = static_cast<tasker &>(this->user_->get_tasker());
    taskstory_commit_RAII commiter(response->taskstory_name, tasker_);

    for (const auto &[k, v] : response->taskstory_json.items())
    {
        //cout << v.dump(4) << endl;
        task_t task_test = make_shared<task>(v.get<task>());
        time_determinator time_dt(task_test, provisional_scheduler);
        cout << "checking task: " << task_test->get_tag() << endl;
        time_dt.build();
        //We are colliding with our own group task as they are not in the scheduler we test unitil we finish
        //So we need to feedback the test_scheduler
        tasker_.add_to_group(move(task_test), response->taskstory_name);
    }
    // auto group_vector = commiter.get_group();

    // if (group_vector)
    // {
    //     queue<task_t> taskstory;
    //     for (auto &[k, v] : response->taskstory_json.items())
    //     {
    //         //This is required to hold the order
    //         taskstory.push(group_vector->at(v["tag"].get<string>()));
    //     }

    //     /////Add all at the same time
    //     if (provisional_scheduler.add_group(move(taskstory)))
    //     {
    //     }
    // }
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
        auto new_state = make_shared<form_state>();
        form_runner::user_running_forms[session_id] = new_state;
        return new_state;
    }
}
