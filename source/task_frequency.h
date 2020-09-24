#ifndef TASK_FREQUENCY_H
#define TASK_FREQUENCY_H

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <functional>
#include "json.hpp"
#include "api_validated.hpp"
#include "time_utils.hpp"

using std::string;
using std::cout;
using std::endl;

class task_frequency : public api_validated
{
private:
public:
    task_frequency():api_validated("../apps/api/frequencies.json"){};
    static task_frequency & get_frequency_lazy_unique_instance();
};

class frequency{
private:
    std::map<std::string,std::function<seconds(int64_t)>> conversors{
        {"seconds", [](int64_t m){ return duration_cast<seconds>(seconds(m));   }},
        {"minutes", [](int64_t m){ return duration_cast<seconds>(minutes(m));   }},
        {"hours",   [](int64_t m){ return duration_cast<seconds>(hours(m));     }},
        {"days",    [](int64_t m){ return duration_cast<seconds>(days(m));      }},
        {"weeks",   [](int64_t m){ return duration_cast<seconds>(weeks(m));     }},
        {"months",  [](int64_t m){ return duration_cast<seconds>(months(m));    }},
        {"years",   [](int64_t m){ return duration_cast<seconds>(years(m));     }},
    };
    string m_frequency;
    seconds period;

    //this determines the hierarchy and how the heuristic of overrie should work
    string is;
    bool get_json();
public:
    frequency():m_frequency("day-regular-basics"){ };
    void set_frequency(const string frequency_);
    seconds get_period() const noexcept { return period; }
    const string & get_frequency_name() const { return this->m_frequency ;} 
};


#endif //TASK_FREQUENCY_H