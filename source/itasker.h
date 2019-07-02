#ifndef ITASKER_H
#define ITASKER_H

#include <string>
#include <vector>
#include <memory>
#include "ischeduler.h"
using namespace std;
class itasker 
{
private:
    static vector<weak_ptr<itasker>> taskers_global; //Global reference to all taskers to cicle them updates on caducity

public:
    itasker(/* args */);
    virtual ~itasker();

    void Create(string name);
};

#endif //ITASKER_H