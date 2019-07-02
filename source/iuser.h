#ifndef IUSER_H
#define IUSER_H

#include "itasker.h"
#include "ischeduler.h"

template<typename T_user,typename T_tasker,typename T_scheduler>
class iuser 
{
private:
    // string unique_id; //For now to check existance and to bind task data to someone
    // #string pass;
    itasker<T_tasker> tasker;
    ischeduler scheduler;
public:
    inline const itasker & get_tasker() const noexcept { return tasker; }
    inline const ischeduler & get_scheduler() const noexcept { return scheduler; }
};

#endif //IUSER_H