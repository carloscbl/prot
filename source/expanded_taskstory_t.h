#ifndef EXPANDED_TASKSTORY_T_H
#define EXPANDED_TASKSTORY_T_H
#include "json.hpp"
#include "app_parser.h"

using nlohmann::json;

class expand_taskstory_t {
private:
    next_question_data_and_taskstory_input & m_nqdati;
public:
    expand_taskstory_t(next_question_data_and_taskstory_input & nqdati):
    m_nqdati(nqdati){}

    bool expand_and_set(); // constructs into m_nqdati reference the expanded_taskstory
    unique_ptr<json> exapand_matrix(const json & type_details);
};

unique_ptr<json> expand_taskstory_t::exapand_matrix(const json & type_details){
    for(auto [k,v] : m_nqdati.raw_taskstory.items()){
        
    }
}

#endif //EXPANDED_TASKSTORY_T_H