#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <unordered_map>
#include <functional>
#include "db_ops.hpp"
#include "cloud_app_runner.h"

bool switch_jobs(const json & job);

void jobs_watcher_start(std::function<void(void)> func, unsigned int interval)
{
    std::thread([func, interval]() {
        while (true)
        {
            auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval);
            func();
            std::this_thread::sleep_until(x);
        }
    }).detach();
}

void prot_jobs_scheduling()
{
    std::cout << "Resuming pending jobs ..." << std::endl;
    auto &db = mysql_db::get_db_lazy().db;
    db.start_transaction();

    auto jobs = read_prot_jobs(minutes(2));
    for (auto &&[k,v] : jobs){
        (*v)["started_at"] = system_clock::to_time_t(system_clock::now());
        update_prot_jobs(k,*v);
    }
    db.commit_transaction();

    // Swtich the job to its function
    std::vector<uint64_t> correctly_done_jobs;
    for (auto &&[k,v] : jobs){
        if ( switch_jobs(*v) ){
            correctly_done_jobs.push_back(k);
        }
    }
    for (auto &&k : correctly_done_jobs){
        db_op::remove<orm_prot::ProtJobs>(k);
    }
}

// This type is intended to clone tasks but using a scheduling from start of the day to avoid collide
bool task_clone_into_next_period(const json & job){
    cout << "Starting job -> " <<job.dump(4)<< endl;
    try
    {
        // {
        //     "type":"task_clone_into_next_period",
        //     "task_id": 2,
            
        // }
        //steps
        // read the task from db
        auto task = read_task(job["task_id"].get<uint64_t>());
        cout << " job read_task -> " <<task->get_json().dump(4)<< endl;
        // read from_user_apps_id
        auto user_app_opt = db_op::get_user_and_app_from_task(task->get_id());
        if(!user_app_opt.has_value()){
            return false;
        }
        const auto && user_app = move(user_app_opt.value());
        cloud_app_runner car(*user_app.first, *user_app.second);
        auto start = job.find("next_period_start");
        if(start == job.end()){
            cout << "next_period_start needs to exists!"<< endl;
            assert( false ) ;
        } // if doesn't exists it needs to be extracted from the frequency!!
        return car.schedule_single_task( task->get_json(), system_clock::from_time_t( start.value().get<time_t>() ) );
        // update start point in scheduler
        // store in db
        /* code */
    }
    catch(const std::exception& e)
    {
        cout << "pete bad job" << endl;
        std::cerr << e.what() << '\n';
        // Delete job?
        return true;
    }

    return true;

}

inline const unordered_map<string,function<bool(const json &)>> type_job_mapping{
    {"task_clone_into_next_period", &task_clone_into_next_period},
};

bool switch_jobs(const json & job){
    auto type = job.find("type");
    if(type == job.end()){
        cout << "Job have NOT type" << job.dump() << endl;
        return false;
    }

    auto type_function_mapping = type_job_mapping.find(type.value().get<string>());
    if(type_function_mapping == type_job_mapping.end()){
        cout << "Job type is not know mapping have NOT type" << job.dump() << endl;
        return false;
    }

    return type_function_mapping->second(job);
}

// int main()
// {
//     jobs_watcher_start(prot_jobs_scheduling, 1000);
//     while (true)
//         ;
// }