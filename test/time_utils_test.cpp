#include "Catch2/catch.hpp"
#include "json.hpp"
#include "user.h"
#include "form.h"
#include "task_restrictions.h"
#include "cloud_app_runner.h"

#include <fstream>
#include <iostream>
#include "json.hpp"

#include "time_utils.hpp"

TEST_CASE( "test time_utils", "[time_utils]" ) {


    {
        // weekday
        days period_today_index = days{4};
        auto get = &get_offset_day<weeks>;
        auto result_prev = get(period_today_index, 2);
        auto result_same = get(period_today_index, 4);
        auto result_pos = get(period_today_index, 6);
        auto x = (weeks(1) + days(2)) - period_today_index;
        CHECK( result_prev == x );
        CHECK( result_same == days(0) ); // This should print difference not total
        CHECK( result_pos == days(2) );
    }
    {
        // monthday
        days period_today_index = days{20};
        auto get = &get_offset_day<months>;
        auto result_prev = get(period_today_index, 2);
        auto result_same = get(period_today_index, 20);
        auto result_pos = get(period_today_index, 22);
        
        CHECK( result_prev.count() == ((std::chrono::duration_cast<days>(months(1)) +days(2)) - period_today_index).count() );
        CHECK( result_same ==  days(0) );
        CHECK( result_pos == days(2) );
    }
    {
        // yearday
        days period_today_index = days{200};
        auto get = &get_offset_day<years>;
        auto result_prev = get(period_today_index, 2);
        auto result_same = get(period_today_index, 200);
        auto result_pos = get(period_today_index, 220);
        
        CHECK( result_prev.count() == ((std::chrono::duration_cast<days>(years(1)) +days(2)) - period_today_index).count() );
        CHECK( result_same ==  days(0) );
        CHECK( result_pos == days(20) );
    }

}
