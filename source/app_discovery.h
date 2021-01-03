#pragma once
#include <chrono>
#include "fake_clock.h"
#include "discovered_components.h"
#include "db_ops.hpp"
#include "spdlog/spdlog.h"


// This class handles the loop of all missing domain logic and updates of tasks
//https://stackoverflow.com/questions/33606482/are-there-facilities-in-stdchrono-to-assist-with-injecting-system-clock-for-un

struct production_workmode
{
    using clock = std::chrono::system_clock;
};

struct testing_workmode
{
    using clock = testing::fake_clock;
};

template <typename workmode = production_workmode, typename clock = typename workmode::clock >
class app_discovery{
    public:
    app_discovery(){}
    void deliver(){
        // We iterate over all user_apps to determine if something missing
        // iterate all user_apps

        auto discovered_objs = db_op::discover_new_app_refresh(clock::now());
        // With the apps running, now we should perform projections and cleanups

    }
};


