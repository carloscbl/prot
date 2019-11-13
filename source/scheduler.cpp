#include "scheduler.h"
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <ctime>

using task = task_space::task;

void print_time(const im_t &  interval_map) 
{
    auto put_time_ = [&](const time_t& t)-> auto { 
        return std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    };
    for ( const auto & [k,v] : interval_map){
        cout << put_time_(k.lower()) << " - " << put_time_(k.upper())  << " : Task " << v->id << " " << v->get_tag() << endl;
    }
}
task_t scheduler::get_task(string tag){
    for (auto &&[k,v] : m_interval_map)
    {
        if(v->get_tag() == tag){
            return v;
        }
    }
    return nullptr;
}

scheduler::scheduler(scheduler_policy policy)
:policy(policy)
{
    switch (policy)
    {
    case scheduler_policy::deny :
        this->policy_fun = &scheduler::deny_policy;
        break;
    //////////////////////////////////////////////////
    default:
        break;
    }
}

im_t scheduler::clone_interval_map() const {
    std::scoped_lock<mutex> lock (this->scheduler_mutex);
    return this->m_interval_map;
}

bool scheduler::find_relative(task_t item, chrono::seconds after_before, time_t end, time_t min_dur)
{
    throw;
    return false;
}

optional<vector<task_t>> scheduler::get_range(time_t start, time_t end){
    vector<task_t> range_vec;
    const auto & range =boost::make_iterator_range(
            this->m_interval_map.equal_range(
                time_interval::closed(
                    start, 
                    end)));
    for (auto & element : range)
    {
        range_vec.push_back(element.second);
    }
    
    return range_vec.empty() ? nullopt : make_optional(range_vec);
}

//No check, we assume checked
bool scheduler::add_single(const task_t && task_)
{
    mod = true;
    const auto &interval_ = task_->get_interval();
    if(find_range(interval_.start, interval_.end)){
        this->m_interval_map.set(make_pair(time_interval::closed(interval_.start, interval_.end), task_));
        return true;
    }else{
        return false;
    }
}



bool scheduler::find_range(time_t start, time_t end){
    return policy_fun(this, policy_relevant_data{
        .start = start,
        .end = end
    });
}

void scheduler::print_out() const
{
    auto put_time_ = [&](const time_t& t)-> auto { 
        return std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    };
    for ( const auto & [k,v] : m_interval_map){
        cout << put_time_(k.lower()) << " - " << put_time_(k.upper())  << " : Task " << v->id << " " << v->get_tag() << endl;
    }
}

bool scheduler::deny_policy(policy_relevant_data && task_info_for_scheduler ) {
    const auto & range = boost::make_iterator_range(
        this->m_interval_map.equal_range(
            time_interval::closed(
                task_info_for_scheduler.start, 
                task_info_for_scheduler.end)));

    if (!range.empty())
    {
        return false;
    }
    return true;
}
void scheduler::clear(){
    std::scoped_lock<mutex> lock (this->scheduler_mutex);
    this->m_interval_map.clear();
}
//If the whole group got allocation then true;
bool scheduler::add_group(queue<task_t> && provisional_taskstory){
    bool all_correct = true;
    while (!provisional_taskstory.empty())
    {
    //    const auto & current_task = provisional_taskstory.front();
        //We got the element... now we need to evaluate it
        if(!add_single( move(provisional_taskstory.front()) )){
            break;
            all_correct = false;
        }
        provisional_taskstory.pop();
    }

    return all_correct;
}

bool provisional_scheduler_RAII::add_group(queue<task_t> && provisional_taskstory){
    this->valid = scheduler::add_group(move(provisional_taskstory));
    return this->valid;
}

provisional_scheduler_RAII scheduler::get_provisional(){
    return provisional_scheduler_RAII(*this);
}

provisional_scheduler_RAII::provisional_scheduler_RAII(scheduler & parent)
:parent(parent)
{
    parent.scheduler_mutex.lock();
    m_interval_map = im_t(parent.m_interval_map);
}

void provisional_scheduler_RAII::commit(){
    parent.m_interval_map.swap(this->m_interval_map);
}

provisional_scheduler_RAII::~provisional_scheduler_RAII()
{
    if(valid){
        parent.m_interval_map.swap(this->m_interval_map);
    }

    parent.scheduler_mutex.unlock();
    //cout << "~provisional_scheduler_RAII " << endl;
}