#ifndef TASK_RESTRICTIONS_H
#define TASK_RESTRICTIONS_H

#include <string>
#include <fstream>
#include <iostream>
#include <set>
#include <optional>
#include "json.hpp"
#include "api_validated.hpp"
#include <vector>
#include <chrono>

using std::vector;

using std::set;
using std::cout;
using std::endl;
using std::optional;



class task_restrictions : public api_validated
{
private:
    /* data */
public:
    task_restrictions() : api_validated("../forms/api/restrictions.json"){};
    static task_restrictions & get_restrictions_lazy_unique_instance();
};
using std::chrono::hours;
struct json_interval {
    hours from;
    hours to;
    string restriction_name;
};

optional<json_interval> find_restriction(const string & restriction_name) noexcept;
class restrictions{
private:
    set<string> m_restrictions;
    
public:
    void set_restrictions(const set<string> & restrictions_);
    //From, to
    vector<json_interval> get_all_from_to() const;

};


#endif //TASK_RESTRICTIONS_H