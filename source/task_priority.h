#ifndef TASK_PRIORITY_H
#define TASK_PRIORITY_H

#include <string>
#include <fstream>
#include <iostream>
#include "json.hpp"
#include "api_validated.h"

using std::string;
using std::cout;
using std::endl;

class task_priority : public api_validated
{
private:
public:
    task_priority():api_validated("../forms/api/priority.json"){};
    static task_priority & get_priority_lazy_unique_instance();
};

class priority{
private:
    string m_priority;
public:
    priority():m_priority("day-regular-basics"){ };
    void set_priority(const string priority_);
};


#endif //TASK_PRIORITY_H