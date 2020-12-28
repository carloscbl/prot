#pragma once
#include <chrono>
#include "db_ops.hpp"

// This class handles the loop of all missing domain logic and updates of tasks
//https://stackoverflow.com/questions/33606482/are-there-facilities-in-stdchrono-to-assist-with-injecting-system-clock-for-un

struct production_workmode
{
    using clock = std::chrono::system_clock;
};



template <typename workmode = production_workmode, typename clock = typename workmode::clock >
class discovery{
    public:
    discovery(){}
    void deliver(){
        // We iterate over all user_apps to determine if something missing
        // iterate all user_apps

        // Query by user_apps where (last_discovered is NULL AND qa_history is not NULL) or (last_discovered + 1 DAY > NOW()  AND last_discovered < updated_at)
        db_op::discover_new_app_refresh();

    }
};