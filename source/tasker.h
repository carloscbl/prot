#ifndef TASKER_H
#define TASKER_H

#include "itasker.h"

//Vector because of the fast cache access as they need to be updated almost in real time, but CUD operations are comparatively rare
inline vector<weak_ptr<itasker>> taskers_global; //Global reference to all taskers to cycle them updates on caducity

/*
Provides the concrete implementation for the management of the tasks of a specific user
 */
class tasker : public itasker
{
private:
public:
    map<string,shared_ptr<task> > tasks; // TODO Setters and getters
    //virtual ~tasker();
    //void update_time() override {};
};

#endif //TASKER_H