#ifndef TIME_DETERMINATOR_H
#define TIME_DETERMINATOR_H

#include "task.h"
#include "scheduler.h"
#include "time_determinator.h"
#include "task_frequency.h"
#include "time_utils.hpp"

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
    time_t today = system_clock::to_time_t(from);
    cout << std::ctime(&today) << endl;
    task_t dummy_task = std::make_shared<task>();
    for (days::rep day = 0; day < d.count(); day++)
    {
        for (auto &_24_restriction_interval : restrictions_interval)
        {
            time_point day_from = (day * days(1)) + from;
            time_point start =  day_from + _24_restriction_interval.from ;
            time_t start_ = system_clock::to_time_t(start);
            cout << std::ctime(&start_) << endl;
            
            time_point end   = day_from + _24_restriction_interval.to ;
            time_t end_ = system_clock::to_time_t(end);
            cout << std::ctime(&end_) << endl;
            //TODO auto time_p_interval = _24_hour_interval_to_time_point(i, )// Today);
            interval_map.set(make_pair(time_interval::closed(system_clock::to_time_t(start), system_clock::to_time_t(end)), move(dummy_task)));
        }

        //now that restrictions are apply, time to check if there is slot
    }
    
    //auto it = interval_map.lower_bound(18)
    return true;
}

time_determinator::~time_determinator()
{
}

#endif //TIME_DETERMINATOR_H
