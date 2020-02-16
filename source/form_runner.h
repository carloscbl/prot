#ifndef FORM_RUNNER_H
#define FORM_RUNNER_H

#include <chrono>
#include <thread>
#include <queue>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "json.hpp"
#include "user.h"
#include "form_parser.h"
#include "form.h"
#include "tasker.h"
#include "ischeduler.h"
#include "scheduler.h"

using namespace std;
using namespace chrono_literals;

//So this class will handle the call form_run from form_parser
//On construction it will check for the current form being evaluated prevoiously and unfinnised
//So will find for a existing session and restore it and handle in a threaded way the life time of this session
//Probably a thread pool is a good idea an a queue, but for now, we will start new threads for simplicity
//This will requiere a user and a form name to find if the user have currently existing one
//If not we will create a session and store it
using sessions = map<string, shared_ptr<form_state>>;
using variables_t = map<string, json>;

class form_runner
{
private:
    inline static sessions user_running_forms;
    const std::chrono::minutes life_time = 5min;
    user & user_;
    form & form_;
    uint64_t user_forms_id;
    unique_ptr<form_parser> fp;

public:
    form_runner(user & user_, form &form_);
    bool perform_taskstory(next_question_data & response);
    shared_ptr<form_state> get_session() const noexcept;
    shared_ptr<form_state> fetch_next_session() const noexcept;
    shared_ptr<form_state> new_session(const string & session_id) const noexcept;
    shared_ptr<form_state> new_session() const noexcept;
    string get_unique_id_session() const noexcept;
    const json run(const json &j) noexcept;
    task_t command_to_task(string &taskstory_command, variables_t &variables);
    inline void clear_sessions(){
        user_running_forms.clear();
    }
};

#endif //FORM_RUNNER_H