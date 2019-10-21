#ifndef RELOCATION_GROUP_H
#define RELOCATION_GROUP_H

#include "time_utils.hpp"

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
public:
    relocation_group(const string & group_name, scheduler & sche_);
    ~relocation_group();
};


#endif //RELOCATION_GROUP_H
