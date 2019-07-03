#ifndef TASKER_H
#define TASKER_H

#include "itasker.h"

class tasker : public itasker
{
private:
    static vector<weak_ptr<itasker>> taskers_global; //Global reference to all taskers to cicle them updates on caducity
public:
    tasker(/* args */);
    virtual ~tasker();
};

#endif //TASKER_H