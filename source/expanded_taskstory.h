#ifndef EXPANDED_TASKSTORY_H
#define EXPANDED_TASKSTORY_H
#include "json.hpp"

using nlohmann::json;

struct raw_taskstory{
    json taskstory_json;
};


class expanded_taskstory {
private:
    const raw_taskstory & raw_tsks;
    const json & user_input;
public:
    expanded_taskstory(const raw_taskstory & raw_tsks,const json & user_input):
    raw_tsks(raw_tsks),user_input(user_input){}

    bool build();
    json m_expanded_taskstory_result;
};

#endif //EXPANDED_TASKSTORY_H