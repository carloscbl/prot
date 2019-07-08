#ifndef IUSER_H
#define IUSER_H

#include "itasker.h"
#include "ischeduler.h"


class iuser 
{
public:
    virtual const itasker & get_tasker() const noexcept;
    virtual const ischeduler & get_scheduler() const noexcept;
    virtual const string & get_name() const noexcept;
};

#endif //IUSER_H