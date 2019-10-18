#ifndef ITASKER_H
#define ITASKER_H

#include <string>
#include <vector>
#include <memory>
#include "task.h"
//#include <boost/icl/interval.hpp>
using namespace std;
//using time_interval = boost::icl::interval<time_t>;
using task = task_space::task;
/*
This class should hold the management of the whole set of task for a given user
The relation with a scheduler is that the scheduler gets the injection of this
dependency, to syncronize the representation.
This class handles the subgrouping of task and its relations with CRUD operations
Meanwhile scheduler does the time orchestation, both are very close and related
eg:
Setted washer group task, need to be renamed, or parametrized in a non timely manner
It is handle here.
But eg:
If the whole set needs to be rescheduled and find where they fit, is a scheduler task
*/

class itasker
{
public:
    
    //virtual void update_time() = 0;
    virtual task_t get_task(const string & id ) const = 0;
    virtual task_t find_task(const string & tag)  const = 0;
};

#endif //ITASKER_H