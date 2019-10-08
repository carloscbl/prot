#ifndef TIME_UTILS_H
#define TIME_UTILS_H
#include <chrono>
#include "task_restrictions.h"
#include <ctime>

using std::chrono::seconds;
using std::chrono::minutes;
using std::chrono::hours;
using std::chrono::system_clock;
using time_point = std::chrono::time_point<system_clock>;
using days = std::chrono::duration<int64_t,std::ratio<86400>>;
using weeks = std::chrono::duration<int64_t,std::ratio<604800>>;
using months = std::chrono::duration<int64_t,std::ratio<2629746>>;
using years = std::chrono::duration<int64_t,std::ratio<31556952>>;
using std::chrono::duration_cast;
using std::chrono::ceil;
using std::chrono::round;
using std::chrono::floor;

struct time_point_interval{
    time_point from ;
    time_point to;
};

inline time_point_interval _24_hour_interval_to_time_point (json_interval interval, time_point start_day){
    time_point to = start_day + interval.to;
    if(interval.to > interval.from){
        to += std::chrono::hours(24);
    }
    return time_point_interval {
        .from = start_day + interval.from,
        .to = to
    };
}

#endif //TIME_UTILS_H