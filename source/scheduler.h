#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "ischeduler.h"
#include <map>
#include <set>
#include <queue>
#include <climits>
#include "itasker.h"

#include <boost/icl/interval.hpp>
#include <boost/icl/interval_map.hpp>

using namespace std;

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
};


/* In the future this would be templatized as is trivial the contained type
TODO:
Start from scrach the scheduler, empty
Add a task single Task
  -Check if there is available hours
  -Introduce it into the interval map
Add a Group of task interdependant
    -Block mutex
    -Iterate over the commands
    -Evaluate and enqueue in the provisional queue
    -Add the provisional queue to the expresions map, to resolve dependences created in situ
    -...End iterations
    -Add all the provisional to the interval map in order of queue
    -Clean the provisional
    -unlock mutex
Get Range from interval
    -Search for it in the interval map
    -Return a vector with all of them
Delete
    -Search in the interval map
    -Delete
Delete Group
    -Find the group and delete it
Euristics to find gaps
    -FindRange
    -FindRelative
    -...
*/
class scheduler : public ischeduler
{
private:
    im_t m_interval_map;//Here lie the real scheduled task
    bool add_task(itasker * task);
    queue<itasker*> provisional;//Here the waiting to operate

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