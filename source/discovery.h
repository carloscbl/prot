#pragma once
#include <chrono>

// This class handles the loop of all missing domain logic and updates of tasks
//https://stackoverflow.com/questions/33606482/are-there-facilities-in-stdchrono-to-assist-with-injecting-system-clock-for-un

struct production_workmode
{
    using clock = std::chrono::system_clock;
};



template <typename workmode = production_workmode, typename clock = typename workmode::clock >
class discovery{
    discovery(){}
    void deliver(){
        // iterate all users
        // iterate all user_apps
        // iterate all user_tasks
    }
};