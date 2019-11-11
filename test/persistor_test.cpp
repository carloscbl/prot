#include "Catch2/catch.hpp"
#include "json.hpp"
#include "tasker.h"


TEST_CASE( "serialization test", "[persistor]" ) {
    persistor::set_persistor(make_unique<disk_storage>());
    task_t test_probe = make_shared<task>();
    test_probe->set_tag("tagg");
    test_probe->get_restrictions();
    test_probe->get_task_group("tagg");
    test_probe->set_interval("tagg");
    test_probe->set_user("tagg");
    test_probe->id = "name";
    test_probe->save();
    REQUIRE(true);

}