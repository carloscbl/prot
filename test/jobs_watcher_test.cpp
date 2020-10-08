#include "Catch2/catch.hpp"
#include "json.hpp"
#include "db_ops.hpp"
#include "jobs_watcher.hpp"

TEST_CASE( "test jobs_watcher", "[runner]" ) {

    std::ifstream task_json("../test/iofiles_test/test_task.json", std::fstream::in);
    std::ifstream task_wildcard_json("../test/iofiles_test/test_task_wildcard.json", std::fstream::in);
    std::ifstream app_daily_file("../test/iofiles_test/test_jobs_dailyworkout.json", std::fstream::in);
    json task_;
    task_json >> task_;
    json task_wildcard;
    task_wildcard_json >> task_wildcard;

    json app_daily;
    app_daily_file >> app_daily;
    create_user("pepe");
    create_app(app_daily, "pepe");
    auto install_id = create_instalation("pepe", "test Daily workout");
    //-- JOB 1
    json job;
    job["name"] = "do thing";
    job["type"] = "task_clone_into_next_period";
    task tk;
    from_json(task_, tk);
    tk.set_user_apps_id(install_id);
    auto tk_id = create_task( tk );
    job["task_id"] = tk_id;
    auto next_period_start_day = task_space::next_period_start(make_shared<task>(tk));
    if(next_period_start_day.has_value()){
        job["next_period_start"] = system_clock::to_time_t( next_period_start_day.value() );
    }
    //-- JOB 2
    json job2;
    job2["name"] = "do  2";
    job2["type"] = "task_clone_into_next_period";
    task tk2;
    from_json(task_wildcard, tk2);
    tk2.set_user_apps_id(install_id);
    auto tk_id2 = create_task( tk2 );
    job2["task_id"] = tk_id2;
    auto next_period_start_day2 = task_space::next_period_start(make_shared<task>(tk2));
    if(next_period_start_day2.has_value()){
        job2["next_period_start"] = system_clock::to_time_t( next_period_start_day2.value() );
    }


    cout << "creating jobs" <<endl;
    auto id_job = create_prot_jobs(job);
    auto id_job2 = create_prot_jobs(job2);
    cout << "creating jobs" <<endl;
    REQUIRE( id_job > 0);
    REQUIRE( id_job2 > 0);

    prot_jobs_scheduling();
        
    auto result = read_prot_jobs(seconds(0));
    REQUIRE(result.size() == 2);

    REQUIRE(true);

}