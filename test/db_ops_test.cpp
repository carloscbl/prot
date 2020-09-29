#include "Catch2/catch.hpp"
#include "json.hpp"
#include "db_ops.hpp"

TEST_CASE( "test jobs", "[runner]" ) {

    json job;
    job["name"] = "do thing";
    auto id_job = create_prot_jobs(job);
    REQUIRE( id_job > 0);
    auto result = read_prot_jobs(seconds(0));
    REQUIRE(result.size() >= 1);

    (*result[id_job])["started_at"] = system_clock::to_time_t(system_clock::now());
    update_prot_jobs(id_job,(*result[id_job]));

    auto result2 = read_prot_jobs(minutes(7));

    for (auto &&[k,v] : result2)
    {
        cout << v->dump(4) << endl;
    }
    
    for (auto &&[k,v] : result)
    {
        REQUIRE( true == db_ops::remove<orm_prot::ProtJobs>(k));
        
    }
    
    REQUIRE(true);

}