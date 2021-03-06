#ifndef ISCHEDULER_H
#define ISCHEDULER_H

#include <time.h>
#include <chrono>
#include <queue>
#include <memory>
#include <optional>
#include "task.h"
class transactional_group_scheduler_RAII;
using std::queue;

class ischeduler
{
public:
    using scheduled = int;
    using time_with_negative = char;
    using task_t = shared_ptr<task_space::task>;
    virtual bool add_single(const task_t && task_) = 0;
    virtual bool add_group(queue<task_t> && provisional_taskstory) = 0;
    virtual optional<vector<task_t>> get_range(time_t start, time_t end) = 0;
    virtual bool find_range(time_t start, time_t end) = 0;
    virtual bool find_relative(task_t item, std::chrono::seconds after_before, time_t end, time_t min_dur) = 0;
    virtual void print_out() const = 0;
    virtual transactional_group_scheduler_RAII get_provisional() = 0;
};

#endif //ISCHEDULER_H