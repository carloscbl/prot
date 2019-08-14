#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "ischeduler.h"
#include <map>
#include <set>
#include <queue>
#include <climits>
#include "itasker.h"
#include "task.h"

#include <boost/icl/interval.hpp>
#include <boost/icl/interval_map.hpp>

using namespace std;

// Set of IDs that cover a time interval
// interval tree from intervals of timestamps to a set of ids
using time_interval = boost::icl::interval<time_t>;
using task_t = ischeduler::task_t;

typedef boost::icl::interval_map<time_t, task_t> im_t;
// a time interval
typedef boost::icl::interval<time_t> interval_t;

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
    im_t m_interval_map;       //Here lie the real scheduled task
    queue<task_t> provisional; //Here the waiting to operate


public:
    enum class scheduler_policy{
        deny,
        priority,
        find_gap
    } policy = scheduler_policy::deny;

    scheduler(/* args */);
    virtual ~scheduler();
    /*
    On adding a new element
    An scheduler, potentially can hold multiple task at the same time
    But for us, this is not optimal, we will hold only one task at a time
    This means we can't overlap
    So if we find an intent of overlaping, we will deny the operation if the range is actually filled
    This way we don't need to retroactively to reallocate the displaced ones, forward operations
    In the future, we can do two stretegies, priority rank and find the gap.
    */
    bool add_single(const task_t && task_) override;
    vector<task_t> get_tasks_in(interval_t interval);
    bool get_range(time_t start, time_t end) override { return true; };
    bool find_range(time_t start, time_t end) override;
    bool find_relative(task_t item, chrono::seconds after_before, time_t end, time_t min_dur) override;
    void print_out() const override;
};

#endif //SCHEDULER_H