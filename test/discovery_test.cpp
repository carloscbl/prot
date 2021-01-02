#include "Catch2/catch.hpp"
#include "json.hpp"
#include "discovery.h"


TEST_CASE( "test discovery", "[runner]" ) {
    discovery<testing_workmode> dscvr;
    dscvr.deliver();
}