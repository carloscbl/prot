#ifndef EXPANDED_TASKSTORY_T_H
#define EXPANDED_TASKSTORY_T_H
#include <optional>
#include <string>
#include <any>
#include "json.hpp"
#include "app_parser.h"
#include "duration.h"
#include "type_conversor.h"
#include <fmt/core.h>
using std::optional;
using std::any;
using std::nullopt;
using std::string;
using nlohmann::json;

bool is_wildcard(json raw_task);
string get_matrix_group_by(const json & subtypes);

const inline map<string, function<json(const json &)>> task_subtype_checker{
    {"duration",
        [](const json & type)-> json { 
            prot::duration d;
            type.at("data_input_from_user").get_to(d);
            json checked_duration;
            to_json(checked_duration, d);
            return checked_duration;
        }
    },
};

class expand_taskstory_t {
private:
    next_question_data_and_taskstory_input & m_nqdati;
    unique_ptr<json> exapand_matrix(const json & type_details);
    optional<std::any> autofill_strategy(bool & task_is_filled_with_significant_fields,const json & type_description, size_t main_idx, size_t secondary_idx );
public:
    expand_taskstory_t(next_question_data_and_taskstory_input & nqdati):
    m_nqdati(nqdati){}

    bool expand_and_set(); // constructs into m_nqdati reference the expanded_taskstory
};


#endif //EXPANDED_TASKSTORY_T_H