#ifndef TASK_FREQUENCY_H
#define TASK_FREQUENCY_H

#include <string>
#include <fstream>
#include <iostream>
#include "json.hpp"
#include "api_validated.hpp"

using std::string;
using std::cout;
using std::endl;

class task_frequency : public api_validated
{
private:
public:
    task_frequency():api_validated("../forms/api/frequencies.json"){};
    static task_frequency & get_frequency_lazy_unique_instance();
};

class frequency{
private:
    string m_frequency;
public:
    frequency():m_frequency("day-regular-basics"){ };
    void set_frequency(const string frequency_);
};


#endif //TASK_FREQUENCY_H