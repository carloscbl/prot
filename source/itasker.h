#ifndef ITASKER_H
#define ITASKER_H

#include <string>
#include <vector>
#include <memory>
#include "ischeduler.h"
#include <boost/icl/interval.hpp>
using namespace std;
using time_interval = boost::icl::interval<time_t>;

class itasker 
{
public:
    struct pair_interval{
        time_t up;
        time_t low;
    };
    virtual pair_interval & get_interval() = 0;
    virtual void update_time()= 0;
    virtual void create(string name)= 0;
};

#endif //ITASKER_H