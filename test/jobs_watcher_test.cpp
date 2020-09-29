#include "Catch2/catch.hpp"
#include "json.hpp"
#include "db_ops.hpp"
#include "jobs_watcher.hpp"

TEST_CASE( "test jobs_watcher", "[runner]" ) {

    json job;
    job["name"] = "do thing";
    job["type"] = "task_clone_into_next_period";
    json job2;
    job2["name"] = "do  2";
    job2["type"] = "task_clone_into_next_period";
    auto id_job = create_prot_jobs(job);
    auto id_job2 = create_prot_jobs(job2);
    REQUIRE( id_job > 0);
    REQUIRE( id_job2 > 0);

    prot_jobs_scheduling();
        
    auto result = read_prot_jobs(seconds(0));
    REQUIRE(result.size() == 0);

    REQUIRE(true);

}