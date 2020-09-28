#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include "db_ops.hpp"

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
    std::cout << "I am doing something" << std::endl;
    auto &db = mysql_db::get_db_lazy().db;
    db.start_transaction();

    auto jobs = read_prot_jobs(minutes(2));
    for (auto &&[k,v] : jobs){
        (*v)["started_at"] = system_clock::to_time_t(system_clock::now());
        update_prot_jobs(k,*v);
    }
    db.commit_transaction();
    // Swtich the job to its function
    for (auto &&[k,v] : jobs){
        db_ops::remove<orm_prot::ProtJobs>(k);
    }
}

int main()
{
    jobs_watcher_start(prot_jobs_scheduling, 1000);
    while (true)
        ;
}