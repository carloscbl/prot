#ifndef TIME_DETERMINATOR_H
#define TIME_DETERMINATOR_H

#include "task.h"
#include "scheduler.h"
#include "time_determinator.h"
#include "task_frequency.h"
#include "time_utils.hpp"
#include <boost/icl/interval.hpp>
#include <boost/icl/interval_map.hpp>

/*
  This class is intended to extract from the info of a task, the correct start time of a task
  by getting reading the restrictions, duration and when structures
  this class assumes 24 hour cicle, maybe in the futere sthis can be expanded
*/
class time_determinator
{
private:
    task_t task_;
    scheduler &sche_;

public:
    bool build_restrictions( time_point from, time_point to);
    bool build();
    optional<time_point> check_slot(im_t & interval_map, time_point day_to_search_in);
    time_determinator(task_t task_, scheduler &sche_);
    ~time_determinator();
};

time_determinator::time_determinator(task_t task_, scheduler &sche_) : task_(task_), sche_(sche_)
{
}

bool time_determinator::build()
{
    //Formula:
    /*
    RP = restrictions (Period) # apply local restrictions to each day of period
    bool find_slot(RP, task)
    true set()
    false reallocate_and_set(is, RP, task)
    */
    //1º Apply restrictions to a period range
    //1.1 Get range
    const time_point end = this->task_->get_frequency().get_period() + system_clock::now();
    const time_point start = floor<days>(system_clock::now());
    //For each day Apply restrictions
    build_restrictions(start, end);

    return false;
}

//We need to get the current user scheduler,
//Then we add the restrictions on top
//Restrictions apply json-land and then user-land configs
//Then we get the first free interval as start point

//IMPORTANT, We need to do an equal range for the selected day, and then create
// a local interval_map for that day restrictions

// The number need to be a stamp for the given start of the operated day
// If the interval is nightly then we need to check if the "to" less than "from"
// We need to add it for the next day

bool time_determinator::build_restrictions( time_point from, time_point to)
{

    im_t interval_map = sche_.clone_interval_map();
    days d = ceil<days>(to - from);
    //auto tasks_range = sche_.get_range(system_clock::to_time_t( start ), system_clock::to_time_t( end ));
    //apply restrictions for each day... day1 = now() + day_iteration
    //day_restrictions_interval = day1.start + restriction.start , day1.end + restriction.end
    const auto &rest = task_->get_restrictions();
    vector<json_interval> restrictions_interval = rest.get_all_from_to();

    task_t dummy_task = std::make_shared<task>();
    for (days::rep day = 0; day < d.count(); day++)
    {
        time_point day_from = (day * days(1)) + from;

        for (auto &_24_restriction_interval : restrictions_interval)
        {
            time_point start =  day_from + _24_restriction_interval.from ;
            time_t start_ = system_clock::to_time_t(start);
            
            time_point end   = day_from + _24_restriction_interval.to ;
            time_t end_ = system_clock::to_time_t(end);

            interval_map.set(make_pair(time_interval::closed(start_, end_), dummy_task ));
        }

        //now that restrictions are apply, time to check if there is slot
        check_slot(interval_map , day_from);
        if(true){
            //apply_slot();
            break;
        }
    }
    
    //auto it = interval_map.lower_bound(18)
    return true;
}

optional<time_point> time_determinator::check_slot(im_t & interval_map, time_point day_to_search_in){
    //time_t end_of_day = system_clock::to_time_t( day_to_search_in + days(1) );
    time_t day_start = system_clock::to_time_t( day_to_search_in );
    //First check for upper bound of the beggin of the day... with this we find if exists place
    //Then we need to do lower_bound from result of valid upper_bound + duration of task
    seconds duration = task_->get_duration().m_duration;
    auto a = interval_map.upper_bound(interval_t::closed(day_start, day_start));
    a->first.
    return nullopt;
}

time_determinator::~time_determinator()
{
}

#endif //TIME_DETERMINATOR_H
