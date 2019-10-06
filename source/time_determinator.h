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
public:
    bool build_restrictions(const scheduler & sche_);
    bool build_frequency_frequency();
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

    //We need to determine TODAY which is today plus the frequency and the scheduler giving us slot
    //Based on the frequency and priorities
    task_t dummy_task = std::make_shared<task>();
    for (auto &i : restrictions_interval)
    {
        //TODO auto time_p_interval = _24_hour_interval_to_time_point(i, )// Today);
        interval_map.set(make_pair(time_interval::closed(i.from.count(), i.to.count()), move(dummy_task)));
    }
    //auto it = interval_map.lower_bound(18)
    return true;
}

bool time_determinator::build_frequency_frequency(){
    // const frequency & prior_freq =  this->task_->get_frequency();
    // given a task, and its frequency, find in a range within and get the first slot
    // But if the priority doesnt match with our range, then we need to rescheduler something else

    // Create a class that handle a task candidate and a scheduler, and then returns the new scheduler
    // with our new task included or return error if it is not possible
    //

    return false;

}

time_determinator::~time_determinator()
{
}


#endif //TIME_DETERMINATOR_H
