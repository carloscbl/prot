#include "fake_clock.hh"


struct testing_workmode
{
    using clock = testing::fake_clock;
};
