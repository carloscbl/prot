#include "relocation_group.h"

relocation_group::relocation_group(const string & group_name, scheduler & sche_)
{
}

relocation_group::~relocation_group()
{
}

void relocation_group::add(task_t task_) noexcept{
    this->m_group.push_back(task_);
}