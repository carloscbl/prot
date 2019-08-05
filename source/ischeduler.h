#ifndef ISCHEDULER_H
#define ISCHEDULER_H

#include <time.h>
class ischeduler
{

public:
using scheduled = int;
using time_with_negative = char;
virtual bool get_range(time_t start,time_t end) = 0;
virtual bool find_range(time_t start,time_t end, time_t min_dur) = 0;
virtual bool find_relative(scheduled item, time_with_negative after_before, time_t end, time_t min_dur) = 0;
};

#endif //ISCHEDULER_H