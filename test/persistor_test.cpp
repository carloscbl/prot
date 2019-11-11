#include "Catch2/catch.hpp"
#include "json.hpp"
#include "tasker.h"


TEST_CASE( "serialization test", "[persistor]" ) {
    persistor::set_persistor(make_unique<disk_storage>());
    task_t test_probe = make_shared<task>();
    test_probe->set_tag("name");
    test_probe->id = "name";
    test_probe->save();
    REQUIRE(true);

}