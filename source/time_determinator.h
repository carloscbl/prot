#ifndef TIME_DETERMINATOR_H
#define TIME_DETERMINATOR_H

#include "task.h"
#include "scheduler.h"
#include "time_utils.hpp"

/*
  This class is intended to extract from the info of a task, the correct start time of a task
  by getting reading the restrictions, duration and when structures
  this class assumes 24 hour cicle, maybe in the futere sthis can be expanded
*/
class time_determinator
{
private:
    task_t task_;
    scheduler & sche_;

public:

    //Setup
    bool build();
    bool build_daily_restrictions( const time_point from, const time_point to, im_t & interval_map, int64_t iteration_day)const noexcept;
    //Diferent pipelines
    bool forward_pipeline(const im_t & interval_map, time_point current_day_begin) noexcept;
    bool when_pipeline(const im_t & interval_map,const when & when_, time_point current_day_begin) noexcept;
    //Ops
    optional<time_point> check_slot(const im_t & interval_map, time_point day_to_search_in) const noexcept ;
    void apply_slot(time_point start) noexcept;
    bool find_gap(time_t prev_upper, time_t current_lower, seconds duration_ ) const noexcept;
    time_point get_current_day_begin(int64_t iteration_day, const time_point & from) const noexcept;

    time_determinator(task_t task_, scheduler &sche_);
    ~time_determinator() = default;
};

#endif //TIME_DETERMINATOR_H
