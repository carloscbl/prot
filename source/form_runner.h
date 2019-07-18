#ifndef FORM_RUNNER_H
#define FORM_RUNNER_H
#include <chrono>
#include <thread>
#include <queue>
#include <map>
#include <memory>
#include "form.h"
#include "iuser.h"
#include "command_expr_evaluator.h"

using namespace std;
using namespace chrono_literals;
using form_t = form;

class form_state;

namespace env {
  static map<string, unique_ptr<form_state>> user_running_forms;
}

class form_state{
    string id;
    int current_id;
    queue<command> taskstory;
    map<string,string> answers_history;
};
//So this class will handle the call form_run from form_parser
//On construction it will check for the current form being evaluated prevoiously and unfinnised
//So will find for a existing session and restore it and handle in a threaded way the life time of this session
//Probably a thread pool is a good idea an a queue, but for now, we will start new threads for simplicity
//This will requiere a iuser and a form name to find if the user have currently existing one
//If not we will create a session and store it
class form_runner
{
private:
    /* data */
    const chrono::minutes life_time = 5min;
    iuser & user;
    form_t & form_;
public:
    form_runner(iuser & user, form_t & form_);
    ~form_runner();

    unique_ptr<form_state> get_session() noexcept;
    string get_unique_id_session() const noexcept;
};

form_runner::form_runner(iuser & user, form_t & form_):
user(user),
form_(form_)
{
    auto state = get_session();
    //Create thread
    //Check serialiced session or create a new one
    //Perform or traverse the questionary
    thread th;

}

form_runner::~form_runner()
{
}
string form_runner::get_unique_id_session() const noexcept{
    return user.get_name() + form_.name;
}

unique_ptr<form_state> form_runner::get_session() noexcept{
    auto session_id = get_unique_id_session();
    const auto && state =  env::user_running_forms.find(session_id);
    if(state != env::user_running_forms.end()){
        return move(state->second);
    }else{
        auto new_state = make_unique<form_state>();
        env::user_running_forms[session_id] = move(new_state);
        return new_state;
    }
}

#endif //FORM_RUNNER_H