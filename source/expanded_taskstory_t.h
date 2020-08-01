#ifndef EXPANDED_TASKSTORY_T_H
#define EXPANDED_TASKSTORY_T_H
#include <optional>
#include <string>
#include "json.hpp"
#include "app_parser.h"
using std::optional;
using std::nullopt;
using std::string;
using nlohmann::json;

bool is_wildcard(json raw_task);
string get_matrix_group_by(const json & subtypes);

class expand_taskstory_t {
private:
    next_question_data_and_taskstory_input & m_nqdati;
    unique_ptr<json> exapand_matrix(const json & type_details);
public:
    expand_taskstory_t(next_question_data_and_taskstory_input & nqdati):
    m_nqdati(nqdati){}

    bool expand_and_set(); // constructs into m_nqdati reference the expanded_taskstory
};

bool expand_taskstory_t::expand_and_set(){
    string type_user_input = this->m_nqdati.current_question_obj["type_user_input"];
    if (type_user_input == "MATRIX"){
        this->m_nqdati.expanded_taskstory = exapand_matrix(this->m_nqdati.current_question_obj["type_details"]);
    }
    if(this->m_nqdati.expanded_taskstory != nullptr){
        return true;
    }
    return false;
}

unique_ptr<json> expand_taskstory_t::exapand_matrix(const json & type_details){
    for(auto [k,v] : m_nqdati.raw_taskstory.items()){
        if (!is_wildcard(v)){ continue; }

        // for (size_t i = 0; i < count; i++)
        // {
        //     /* code */
        // }
        
        for(auto [k,v] : type_details[""].items()){

        }
    }
    return make_unique<json>();
}

string get_matrix_group_by(const json & subtypes){
    string type_ = subtypes["group_by"];
    return subtypes["group_types"][type_];
}

bool is_wildcard(json raw_task){ // Returns the wil
    auto wild_task = raw_task.find("wildcard_task");
    if(wild_task == raw_task.end()) return false;

    auto enabled = wild_task.value().find("enabled");
    if(enabled == wild_task.value().end() && enabled.value().get<bool>() == false) return false;

    return true;
}

#endif //EXPANDED_TASKSTORY_T_H