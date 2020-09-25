#ifndef CLOUD_APP_RUNNER_H
#define CLOUD_APP_RUNNER_H

#include <chrono>
#include <thread>
#include <queue>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "json.hpp"
#include "user.h"
#include "expanded_taskstory_t.h"
#include "app_parser.h"
#include "app.h"
#include "tasker.h"
#include "ischeduler.h"
#include "scheduler.h"

using namespace std;
using namespace chrono_literals;

//So this class will handle the call app_run from app_parser
//On construction it will check for the current app being evaluated prevoiously and unfinnised
//So will find for a existing session and restore it and handle in a threaded way the life time of this session
//Probably a thread pool is a good idea an a queue, but for now, we will start new threads for simplicity
//This will requiere a user and a app name to find if the user have currently existing one
//If not we will create a session and store it
using sessions = map<string, shared_ptr<app_state>>;
using variables_t = map<string, json>;

class cloud_app_runner
{
protected:
    inline static sessions user_running_apps;
    const std::chrono::minutes life_time = 5min;
    user & user_;
    app & app_;
    uint64_t user_apps_id;
    unique_ptr<app_parser> fp;

public:
    uint64_t m_session_id = 0;
    cloud_app_runner(user & user_, app &app_);
    cloud_app_runner(user & user_, app &app_, uint64_t user_apps_id); // Used for test
    bool schedule_taskstory(next_question_data_and_taskstory_input & response);
    shared_ptr<app_state> get_session() const noexcept;
    shared_ptr<app_state> fetch_next_session() const noexcept;
    shared_ptr<app_state> new_session(const string & session_id) const noexcept;
    shared_ptr<app_state> new_session() const noexcept;
    string get_unique_id_session() const noexcept;
    const json run(const json &j) noexcept;
    task_t command_to_task(string &taskstory_command, variables_t &variables);
    inline void clear_sessions(){
        user_running_apps.clear();
    }
    void apply_wildcards(next_question_data_and_taskstory_input & response);
};



#endif //CLOUD_APP_RUNNER_H