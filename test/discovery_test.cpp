#include "Catch2/catch.hpp"
#include "json.hpp"
#include "app_discovery.h"


TEST_CASE( "test app_discovery", "[runner]" ) {
    app_discovery<testing_workmode> dscvr;
    dscvr.deliver();
}