#include "Catch2/catch.hpp"
#include "json.hpp"
#include "db_ops.hpp"
#include "jobs_watcher.hpp"

TEST_CASE( "test jobs_watcher", "[runner]" ) {

    std::ifstream task_json("../test/iofiles_test/test_task.json", std::fstream::in);
    std::ifstream task_wildcard_json("../test/iofiles_test/test_task_wildcard.json", std::fstream::in);
    json task_;
    json task_wildcard;
    task_json >> task_;
    task_wildcard_json >> task_wildcard;

    json job;
    job["name"] = "do thing";
    job["type"] = "task_clone_into_next_period";
    task tk;
    from_json(task_, tk);
    auto tk_id = create_task( tk );
    job["task_id"] = tk_id;
    // json job2;
    // job2["name"] = "do  2";
    // job2["task_id"] = 4;
    // job2["type"] = "task_clone_into_next_period";
    cout << "creating jobs" <<endl;
    auto id_job = create_prot_jobs(job);
    // auto id_job2 = create_prot_jobs(job2);
    cout << "creating jobs" <<endl;
    REQUIRE( id_job > 0);
    // REQUIRE( id_job2 > 0);

    prot_jobs_scheduling();
        
    auto result = read_prot_jobs(seconds(0));
    REQUIRE(result.size() == 0);

    REQUIRE(true);

}