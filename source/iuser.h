#ifndef IUSER_H
#define IUSER_H

#include "itasker.h"
#include "ischeduler.h"


class iuser 
{
public:
    inline const itasker & get_tasker() const noexcept;
    inline const ischeduler & get_scheduler() const noexcept;
};

#endif //IUSER_H