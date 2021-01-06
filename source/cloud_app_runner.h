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
private:
    user & user_;
    app & app_;
    uint64_t user_apps_id;

    bool store_qa_history_status(json addition) const;
    shared_ptr<app_state> get_session() const noexcept;
    shared_ptr<app_state> fetch_next_session() const noexcept;
    shared_ptr<app_state> new_session(const string & session_id) const noexcept;
    shared_ptr<app_state> new_session() const noexcept;
    void apply_wildcards(next_question_data_and_taskstory_input & response);
    void register_runner_scheduled_tasks (std::shared_ptr<std::map<std::string, task_t>> done_tasks);
    task_t create_task_to_schedule(const json & j_task) const;

public:
    // public only for testing, dont use in production code directly, use run
    bool schedule_taskstory(next_question_data_and_taskstory_input & response);

    optional<vector<task_t>> scheduled_tasks;
    uint64_t m_session_id = 0;
    cloud_app_runner(user & user_, app &app_);
    cloud_app_runner(user & user_, app &app_, uint64_t user_apps_id); // Used for test
    bool schedule_single_task(const json & j_task, optional<std::chrono::time_point<system_clock>> start_from=nullopt,  task * task_ = nullptr) const;
    const json run(const json &j) noexcept;
    const optional<json> programatic_run_injecting_history_answers(const json &history) noexcept;
};



#endif //CLOUD_APP_RUNNER_H