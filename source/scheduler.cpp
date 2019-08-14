#include "scheduler.h"

scheduler::scheduler()
{
}

scheduler::~scheduler()
{
}

bool scheduler::find_relative(task_t item, chrono::seconds after_before, time_t end, time_t min_dur)
{
    return false;
}

//No check we assume checked
bool scheduler::add_single(const task_t && task_)
{
    const auto &interval_ = task_->get_interval();
    //this->m_interval_map.insert(make_pair(time_interval::closed(interval_.start, interval_.end), task_));
    this->m_interval_map.insert(make_pair(time_interval::closed(interval_.start, interval_.end), task_));
    //print_out();
    return true;
}

bool scheduler::find_range(time_t start, time_t end){
    for(auto& p : boost::make_iterator_range(this->m_interval_map.equal_range(time_interval::closed(start, end)))){
        cout << p.second->id << endl;
    }
    return true;
}

void scheduler::print_out() const
{
    for ( const auto & [k,v] : m_interval_map){
        cout << k << " interval : Task " << v->id << endl;
    }
}