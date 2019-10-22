#ifndef RELOCATION_GROUP_H
#define RELOCATION_GROUP_H

#include <vector>
#include "time_utils.hpp"
#include "task.h"
#include "scheduler.h"

using std::vector;

/*
    this class is an agregator to handle the displaced groups of tasks
    when other with biggest priority or simply a not vinculated part of
    a group is required to find other location
*/
class relocation_group
{
private:
    //Max days to look for a relocation before return imposibility
    days max_range;
    vector<task_t> m_group;
public:
    relocation_group(const string & group_name, scheduler & sche_);
    ~relocation_group();
    void add(task_t task_) noexcept;
};


#endif //RELOCATION_GROUP_H
