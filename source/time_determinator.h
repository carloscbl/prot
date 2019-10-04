#ifndef TIME_DETERMINATOR_H
#define TIME_DETERMINATOR_H

#include "task.h"
#include "scheduler.h"

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
bool time_determinator::build_restrictions(const scheduler & sche_){
    im_t interval_map = sche_.clone_interval_map();

    const auto & rest = task_->get_restrictions();
    rest.
    for each restriction add a range
    interval_map.set(make_pair(time_interval::closed(interval_.start, interval_.end)

    return false;
}

time_determinator::~time_determinator()
{
}


#endif //TIME_DETERMINATOR_H
