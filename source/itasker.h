#ifndef ITASKER_H
#define ITASKER_H

#include <string>
#include <vector>
#include <memory>
#include "ischeduler.h"
using namespace std;

class itasker 
{
public:
    void update_time();
    void create(string name);
};

#endif //ITASKER_H