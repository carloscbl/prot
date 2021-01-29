#ifndef TIME_UTILS_H
#define TIME_UTILS_H
#include <chrono>
#include "task_restrictions.h"
#include "date/date.h"
#include "date/tz.h"
// #include "scheduler.h"
#include <ctime>
#include <iomanip>
#include <fmt/core.h>
#include "spdlog/spdlog.h"

using std::chrono::seconds;
using std::chrono::minutes;
using std::chrono::hours;
using std::chrono::system_clock;
using time_point = system_clock::time_point;
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
    string meta_day;
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
            .meta_day = "prev"
        });
        intervals.push_back(time_point_interval {
            .from = from,
            .to = to + days(1),
            .meta_day = "post"
        });
    }else{
        intervals.push_back(time_point_interval {
            .from = from,
            .to = to,
            .meta_day = "within"
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

inline bool within_same_day(time_point content, time_point container){
    const time_point max = container + days(1);
    const time_point & min = container;
    return content > min && content < max;
}

inline unsigned int get_weekday_index(){

    auto now = std::chrono::system_clock::now();

    auto now_local = date::zoned_time{date::current_zone(), now}. get_local_time();

    auto now_local_in_days = std::chrono::time_point_cast<date::days>(now_local) ;
    auto weekday_index = date::weekday{now_local_in_days}.iso_encoding();
    return weekday_index - 1;
}

inline unsigned int get_monthday_index(){

    auto now = std::chrono::system_clock::now();

    auto now_local = date::zoned_time{date::current_zone(), now}. get_local_time();

    auto now_local_in_days = std::chrono::time_point_cast<date::days>(now_local) ;
    date::year_month_day ymd{now_local_in_days};
    return  static_cast<unsigned int>( ymd.day() ) -1; // Should be - 1
}

inline unsigned int get_yearday_index(){
    using namespace date;
    auto now = std::chrono::system_clock::now();
    auto now_local = date::zoned_time{date::current_zone(), now}.get_local_time();
    auto now_local_in_days = std::chrono::floor<date::days>(now_local) ;
    auto y = year_month_day{now_local_in_days}.year();
    return (now_local_in_days - local_days{y/jan/0}).count() -1; // Should be - 1
}

template <typename T_unit>
inline std::chrono::seconds duration_to_seconds(unsigned long long time){
    return std::chrono::duration_cast<std::chrono::seconds>(T_unit(time));
}

template <typename T_period>
inline days get_offset_day(days today_index, unsigned int designated_period_days){
    // Fill me
    // today > designated_period_days
    // 5|31|269 - 3|22|301
    if(today_index.count() > designated_period_days){ //is in the past, so we forward to next period
        auto aa = std::chrono::duration_cast<days>(T_period(1)) + days(designated_period_days);
        return aa - today_index;
    }else
    {
        return days(designated_period_days) - today_index;
    }
}

typedef std::chrono::duration<double> fsec;
class measure_execution_raii{
    const std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    const string name_of_measure;
public:
    measure_execution_raii(string name_of_measure):name_of_measure(name_of_measure){}
    ~measure_execution_raii(){
        auto end = std::chrono::high_resolution_clock::now();
        fsec duration = std::chrono::duration_cast<std::chrono::nanoseconds>( end - start );
        SPDLOG_INFO("Measuring execution of {} : {:.8f}s", name_of_measure, duration.count());
        // fmt::print("Measuring execution of {0} : {1}\n", name_of_measure, duration);
    }
};

class measure_execution{
    const std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    const string name_of_measure;
public:
    measure_execution(string name_of_measure):name_of_measure(name_of_measure){}
    inline fsec stop(){
        auto end = std::chrono::high_resolution_clock::now();
        fsec duration = std::chrono::duration_cast<std::chrono::nanoseconds>( end - start );
        SPDLOG_INFO("Measuring execution of {} : {:.8f}s", name_of_measure, duration.count());
        return duration;
    }
};


#endif //TIME_UTILS_H