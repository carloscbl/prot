#ifndef EXPANDED_TASKSTORY_T_H
#define EXPANDED_TASKSTORY_T_H
#include "json.hpp"
#include <optional>
#include "app_parser.h"
using std::optional;
using std::nullopt;
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
        if (!is_wildcard(v)){ continue; }

        for(auto [k,v] : m_nqdati.type_details[""].items()){



    }
}

bool is_wildcard(json raw_task){ // Returns the wil
    auto wild_task = raw_task.find("wildcard_task");
    if(wild_task == raw_task.end()) return false;

    auto enabled = wild_task.value().find("enabled");
    if(enabled == wild_task.value().end() && enabled.value().get<bool>() == false) return false;

    return true;
}

#endif //EXPANDED_TASKSTORY_T_H