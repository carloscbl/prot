#include "Catch2/catch.hpp"
#include "json.hpp"
#include "tasker.h"
#include "user.h"
#include "persistor.h"


// TEST_CASE( "serialization test", "[persistor]" ) {
//     persistor_instance::set_instance(make_unique<mysql_db>());


//     auto test_probe = make_shared<user>();

//     // test_probe->set_name("test");
//     // test_probe->set_tag("tagg");
//     // // test_probe->set_restrictions();
//     // // test_probe->set_task_group("tagg");
//     // // test_probe->set_interval("tagg");
//     // test_probe->set_user("useeer");
//     // test_probe->id = "id";

//     test_probe->load("a");
//     REQUIRE(true);

// }

// TEST_CASE( "serialization test", "[persistor]" ) {
//     persistor<mysql_db>::set_persistor(make_unique<mysql_db>());


    
//     REQUIRE(true);

// }