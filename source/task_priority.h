#ifndef TASK_PRIORITY_H
#define TASK_PRIORITY_H
#include <string>
#include <fstream>
#include <iostream>
#include "json.hpp"

using std::string;
using std::cout;
using std::endl;
using nlohmann::json;
class task_priority
{
private:
    const string priority_definitions_file = "../forms/api/priority.json";
    json priorities;
public:
    task_priority(/* args */);
    ~task_priority();
    bool check_priority(const string & mapping_str) const;
    static task_priority & get_priority_lazy_unique_instance();
};

class priority{
private:
    friend void from_json(const nlohmann::json& j, priority& p);
    string m_priority;
public:
    priority():m_priority("day-regular-basics"){ };
    void set_priority(const string priority_);
};


#endif //TASK_PRIORITY_H