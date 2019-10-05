#ifndef TIME_DETERMINATOR_H
#define TIME_DETERMINATOR_H

#include "task.h"
#include "scheduler.h"
#include "time_determinator.h"

/*
  This class is intended to extract from the info of a task, the correct start time of a task
  by getting reading the restrictions, duration and when structures
  this class assumes 24 hour cicle, maybe in the futere sthis can be expanded
*/
class time_determinator
{
private:
    task_t task_;
public:
    bool build_restrictions(const scheduler & sche_);
    time_determinator(task_t task_);
    ~time_determinator();
};

time_determinator::time_determinator(task_t task_):task_(task_)
{
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

bool time_determinator::build_restrictions(const scheduler & sche_){
    im_t interval_map = sche_.clone_interval_map();
    //interval_map->equal_range... Get the sub_interval_map for the given days... maybe is wise to do it after determine restrictions
    const auto & rest = task_->get_restrictions();
    vector<json_interval> restrictions_interval = rest.get_all_from_to();
    //for each restriction add a range
    task_t dummy_task = std::make_shared<task>();
    for (auto &i : restrictions_interval)
    {
        auto time_p_interval = _24_hour_interval_to_time_point(i, // Today);
        interval_map.set(make_pair(time_interval::closed(i.from, i.to), move(dummy_task)));
    }
    //auto it = interval_map.lower_bound(18)
    return true;
}

time_determinator::~time_determinator()
{
}


#endif //TIME_DETERMINATOR_H
