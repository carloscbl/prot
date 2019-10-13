#ifndef FORM_RUNNER_H
#define FORM_RUNNER_H

#include <chrono>
#include <thread>
#include <queue>
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
using form_t = form;

//So this class will handle the call form_run from form_parser
//On construction it will check for the current form being evaluated prevoiously and unfinnised
//So will find for a existing session and restore it and handle in a threaded way the life time of this session
//Probably a thread pool is a good idea an a queue, but for now, we will start new threads for simplicity
//This will requiere a iuser and a form name to find if the user have currently existing one
//If not we will create a session and store it
using sessions = map<string, shared_ptr<form_state>>;
using variables_t = map<string, json>;

class form_runner
{
private:
    inline static sessions user_running_forms;
    const chrono::minutes life_time = 5min;
    shared_ptr<user> user_;
    form_t & form_;
    unique_ptr<form_parser> fp;

public:
    form_runner(shared_ptr<user> user_, form_t &form_);

    shared_ptr<form_state> get_session() const noexcept;
    string get_unique_id_session() const noexcept;
    const json run(const json &j) noexcept;
    task_t command_to_task(string &taskstory_command, variables_t &variables);
};

#endif //FORM_RUNNER_H