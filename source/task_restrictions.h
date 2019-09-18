#ifndef TASK_RESTRICTIONS_H
#define TASK_RESTRICTIONS_H

#include <string>
#include <fstream>
#include <iostream>
#include "json.hpp"
#include "api_validated.h"


class task_restrictions : public api_validated
{
private:
    /* data */
public:
    task_restrictions() : api_validated("../forms/api/priority.json"){};
    static task_restrictions & get_restrictions_lazy_unique_instance();
};

#endif //TASK_RESTRICTIONS_H