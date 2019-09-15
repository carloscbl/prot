#ifndef WHEN_H
#define WHEN_H
#include "json.hpp"

class when
{
private:
public:
    std::string after;
    //delay Similar to duration, better generalice
};

void from_json(const nlohmann::json& j, when& p);

#endif //WHEN_H