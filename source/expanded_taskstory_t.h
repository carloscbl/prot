#ifndef EXPANDED_TASKSTORY_T_H
#define EXPANDED_TASKSTORY_T_H
#include "json.hpp"
#include "app_parser.h"

using nlohmann::json;

struct raw_taskstory_t{
    json taskstory_json;
};


class expand_taskstory_t {
private:
    next_question_data_and_taskstory_input & m_nqdati;
public:
    expand_taskstory_t(next_question_data_and_taskstory_input & nqdati):
    m_nqdati(nqdati){}

    bool expand_and_set(); // constructs into m_nqdati reference the expanded_taskstory
};

void exapand_matrix(const json & type_details, const json & expanded_taskstory){
    for(auto [k,v] : expanded_taskstory.items()){
        
    }
}

#endif //EXPANDED_TASKSTORY_T_H