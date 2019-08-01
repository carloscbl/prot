#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "ischeduler.h"
#include <map>
#include <set>
#include <climits>
#include "itasker.h"

#include <boost/icl/interval.hpp>
#include <boost/icl/interval_map.hpp>

// Set of IDs that cover a time interval
// interval tree from intervals of timestamps to a set of ids
typedef boost::icl::interval_map<time_t, itasker> im_t;
// a time interval
typedef boost::icl::interval<time_t> interval_t;

class scheduled_validated_task{
private:
    shared_ptr<itasker> task;
    bool validate(){
        
    };
public:
    bool is_valid(){ return validate(); }
}

class scheduler : public ischeduler
{
private:
    im_t m_interval_map;
    bool add_task(itasker * task);
public:
    scheduler(/* args */);
    ~scheduler();
    vector<itasker*> get_tasks_in(interval_t interval);
    bool get_range(time_t start,time_t end) override;
    bool find_range(time_t start,time_t end, time_t min_dur) override;
    bool find_relative(scheduled item, time_with_negative after_before, time_t end, time_t min_dur) override;
};

scheduler::scheduler(/* args */)
{
}

scheduler::~scheduler()
{
}

bool scheduler::find_relative(scheduled item, time_with_negative after_before, time_t end, time_t min_dur) 
{
    return false;
}

#endif //SCHEDULER_H