#ifndef WHEN_H
#define WHEN_H
#include "json.hpp"
#include "duration.h"
#include <optional>

using prot::duration;
using std::optional;
class when
{
private:
public:
    std::string after;
    duration minimum_delay;
    duration maximum_delay;
};

void from_json(const nlohmann::json& j, when& p);

#endif //WHEN_H