#ifndef ISCHEDULER_H
#define ISCHEDULER_H

class ischeduler
{

public:
using scheduled = int;
using time_with_negative = char;
virtual auto get_range(time_t start,time_t end) -> bool = 0;
virtual auto find_range(time_t start,time_t end, time_t min_dur) -> bool = 0;
virtual auto find_range(time_t start,time_t end, time_t min_dur) -> bool = 0;
virtual auto find_relative(scheduled item, time_with_negative after_before, time_t end, time_t min_dur) -> bool = 0;
};

#endif //ISCHEDULER_H