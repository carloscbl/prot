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

class scheduler : public ischeduler
{
private:
    /* data */
public:
    scheduler(/* args */);
    ~scheduler();
};

scheduler::scheduler(/* args */)
{
}

scheduler::~scheduler()
{
}

#endif //SCHEDULER_H