#ifndef FORM_RUNNER_H
#define FORM_RUNNER_H
#include <chrono>
#include <thread>
#include <queue>
#include <string>
#include <map>
#include <memory>
#include "json.hpp"
#include "iuser.h"
#include "form_parser.h"
#include "command_expr_evaluator.h"

using namespace std;
using namespace chrono_literals;
using form_t = form;



//So this class will handle the call form_run from form_parser
//On construction it will check for the current form being evaluated prevoiously and unfinnised
//So will find for a existing session and restore it and handle in a threaded way the life time of this session
//Probably a thread pool is a good idea an a queue, but for now, we will start new threads for simplicity
//This will requiere a iuser and a form name to find if the user have currently existing one
//If not we will create a session and store it
using sessions = map<string, shared_ptr<form_state>>; 
class form_runner
{
private:
    inline static sessions user_running_forms;
    const chrono::minutes life_time = 5min;
    iuser & user;
    form_t & form_;
    unique_ptr<form_parser> fp;
public:
    form_runner(iuser & user, form_t & form_);
    ~form_runner();

    shared_ptr<form_state> get_session() const noexcept;
    string get_unique_id_session() const noexcept;
    const json run(const json & j) const noexcept;
};

form_runner::form_runner(iuser & user, form_t & form_):
user(user),
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

form_runner::~form_runner(){}

string form_runner::get_unique_id_session() const noexcept{
    return user.get_name() + form_.name;
}

const json form_runner::run(const json & request_json) const noexcept{
    const auto & state = get_session();
    form_parser fp (form_.get_json(),*state);//,*state
    string question;
    if(request_json.is_null()){
        question = fp.get_initial_question();
    }else{
        cout << request_json.dump(4) << endl;
        question = fp.form_next_in_pipeline(request_json["answer"]);
    }
    user_running_forms[get_unique_id_session()] = fp.get_state();
    json response_j;
    response_j["next_question"] = question;
    return response_j;
}

shared_ptr<form_state> form_runner::get_session()const noexcept{
    auto session_id = get_unique_id_session();
    const auto && state = form_runner::user_running_forms.find(session_id);
    if(state != form_runner::user_running_forms.end()){
        return state->second;
    }else{
        auto new_state = make_shared<form_state>();
        form_runner::user_running_forms[session_id] = new_state;
        return new_state;
    }
}

#endif //FORM_RUNNER_H