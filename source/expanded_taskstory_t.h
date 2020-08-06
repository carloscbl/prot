#ifndef EXPANDED_TASKSTORY_T_H
#define EXPANDED_TASKSTORY_T_H
#include <optional>
#include <string>
#include <any>
#include <vector>
#include "json.hpp"
#include "app_parser.h"
#include "duration.h"
#include "type_conversor.h"
#include <fmt/core.h>
#include <fmt/format.h>
using std::optional;
using std::any;
using std::nullopt;
using std::string;
using nlohmann::json;

bool is_wildcard(json raw_task);
string get_matrix_group_by(const json & subtypes);
bool substitution_or_interpolation(const json & value, json & type_description, fmt::dynamic_format_arg_store<fmt::format_context> & store, json & expanding_task);

const std::vector<string> fields_to_interpolate{
    "name","description",
};

class expand_taskstory_t {
private:
    next_question_data_and_taskstory_input & m_nqdati;
    unique_ptr<json> exapand_matrix(const json & type_details);
    json autofill_strategy(const json & type_description, size_t main_idx, size_t secondary_idx );
public:
    expand_taskstory_t(next_question_data_and_taskstory_input & nqdati):
    m_nqdati(nqdati){}

    bool expand_and_set(); // constructs into m_nqdati reference the expanded_taskstory
};


#endif //EXPANDED_TASKSTORY_T_H