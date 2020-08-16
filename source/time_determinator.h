#ifndef TIME_DETERMINATOR_H
#define TIME_DETERMINATOR_H

#include "task.h"
#include "scheduler.h"
#include "time_utils.hpp"
#include <string_view>

struct current_after_t
{
    task_t prev_task;
    seconds offset_min;
    seconds offset_max;
};



/*
  This class is intended to extract from the info of a task, the correct start time of a task
  by getting reading the restrictions, duration and when structures
  this class assumes 24 hour cicle, maybe in the futere sthis can be expanded
*/
class time_determinator
{
private:
    task_t task_;
    scheduler &sche_;
    optional<string> period = nullopt;

public:
    //Setup
    // TODO
    // We should do 4 passes
    // 1º Try to do within the especified json margins OK
    // 2º if not We should check if we have more priority that the current tasks in our preferred range
    // 3º if a lower priority loses the possibility to be allocated then we should allocate the soones as posible within a {max_range}
    // 4º if the max_range is exced then we should inform that with the current scheduler this set of tasks wont be possible, and got excluded

    optional<bool> build(days start_offset);
    bool build_daily_restrictions(const time_point from, const time_point to, im_t &interval_map, int64_t iteration_day) const noexcept;
    bool build_elapsed_today_restriction(days start_offset, const time_point &day_from, im_t &interval_map);

    //Diferent pipelines, they cannot be noexcept signature
    bool forward_pipeline(const im_t &interval_map, time_point current_day_begin);
    bool when_pipeline(const im_t &interval_map, time_point current_day_begin);

    //Ops
    // PERFORMANCE -This one can be optimiced with SWAR ops, with 4 masks per day and 64 bits to get 5-10 minutes precision
    optional<time_point> check_within_day_slot(const im_t &interval_map, time_point day_to_search_in, const current_after_t *after = nullptr) const noexcept;
    bool apply_slot(time_point start) noexcept;
    bool find_time_gap(time_t prev_upper, time_t current_lower, seconds duration_) const noexcept;
    time_point get_current_day_begin(int64_t iteration_day, const time_point &from) const noexcept;

    bool find_time_gap_edge(time_t prev_upper, const im_t &interval_map, seconds duration_, time_point today) const;
    const optional<size_t> is_specific_period() noexcept;

    time_determinator(task_t task_, scheduler &sche_);
    ~time_determinator() = default;
};

#endif //TIME_DETERMINATOR_H
