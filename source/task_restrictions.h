#ifndef TASK_RESTRICTIONS_H
#define TASK_RESTRICTIONS_H

#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include "json.hpp"
#include "api_validated.h"

using std::set;
using std::cout;
using std::endl;


class task_restrictions : public api_validated
{
private:
    /* data */
public:
    task_restrictions() : api_validated("../forms/api/restrictions.json"){};
    static task_restrictions & get_restrictions_lazy_unique_instance();
};

class restrictions{
private:
    set<string> m_restrictions;
    
public:
    void set_restrictions(const set<string> & restrictions_);
    //From, to
    std::pair<int,int> get_all_from_to();
};


#endif //TASK_RESTRICTIONS_H