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
    string tag;
};

//Here enters eg [from=00,to=06], time_stamp
inline vector<time_point_interval> _24_hour_interval_to_time_point (json_interval interval, time_point start_day){
    vector<time_point_interval> intervals;
    time_point from = start_day +interval.from;
    time_point to = start_day + interval.to;

    if( interval.from >= interval.to ){
        intervals.push_back(time_point_interval {
            .from = from - days(1),
            .to = to,
            .tag = "prev"
        });
        intervals.push_back(time_point_interval {
            .from = from,
            .to = to + days(1),
            .tag = "post"
        });
    }else{
        intervals.push_back(time_point_interval {
            .from = from,
            .to = to,
            .tag = "within"
        });
    }
    return intervals;
}

inline void print_hour(time_t hour){
    cout << ctime(&hour)<< "-------" << endl;
}
inline void print_hour(time_point hour){
    time_t hour_ = system_clock::to_time_t(hour);
    cout << ctime(&hour_) << "-------" << endl;
}
inline void print_hour(time_point_interval interval){
    time_t from_ = system_clock::to_time_t(interval.from);
    cout << ctime(&from_) ;

    time_t to_ = system_clock::to_time_t(interval.to);
    cout << ctime(&to_) << endl;
}
#endif //TIME_UTILS_H