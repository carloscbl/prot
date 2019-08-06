#include "scheduler.h"

scheduler::scheduler()
{

}

scheduler::~scheduler()
{
}

bool scheduler::find_relative(scheduled item, time_with_negative after_before, time_t end, time_t min_dur) 
{
    return false;
}

//No check we assume checked
bool scheduler::add_task(task_t task){
    const auto & interval_ = task->get_interval();
    this->m_interval_map.insert( make_pair(time_interval::closed( interval_.low , interval_.up) , task));
    return true;
}