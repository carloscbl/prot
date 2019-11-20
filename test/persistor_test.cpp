#include "Catch2/catch.hpp"
#include "json.hpp"
#include "tasker.h"
#include "persistor.h"


TEST_CASE( "serialization test", "[persistor]" ) {
    persistor::set_persistor(make_unique<disk_storage>());


    task_t test_probe = make_shared<task>();

    test_probe->set_name("test");
    test_probe->set_tag("tagg");
    // test_probe->set_restrictions();
    // test_probe->set_task_group("tagg");
    // test_probe->set_interval("tagg");
    test_probe->set_user("useeer");
    test_probe->id = "id";

    test_probe->save();
    REQUIRE(true);

}

TEST_CASE( "serialization test", "[persistor]" ) {
    persistor::set_persistor(make_unique<mysql_db>());


    // task_t test_probe = make_shared<task>();

    // test_probe->set_name("test");
    // test_probe->set_tag("tagg");
    // test_probe->set_restrictions();
    // test_probe->set_task_group("tagg");
    // test_probe->set_interval("tagg");
    // test_probe->set_user("useeer");
    // test_probe->id = "id";

    // test_probe->save();
    REQUIRE(true);

}