#include <chrono>
#include "task_restrictions.h"

using std::chrono::seconds;
using std::chrono::system_clock;
using time_point = std::chrono::time_point<system_clock>;

struct time_point_interval{
    time_point from ;
    time_point to;
};

time_point_interval _24_hour_interval_to_time_point (json_interval interval, time_point start_day){
    time_point to = start_day + interval.to;
    if(interval.to > interval.from){
        to += std::chrono::hours(24);
    }
    return time_point_interval {
        .from = start_day + interval.from,
        .to = to
    };
}