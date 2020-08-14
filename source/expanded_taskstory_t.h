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
using std::vector;
using std::nullopt;
using std::string;
using nlohmann::json;

bool is_wildcard(json raw_task);
string get_matrix_group_by(const json & subtypes);
bool substitution_or_interpolation_store(const json & value,const json & type_description, fmt::dynamic_format_arg_store<fmt::format_context> & store, json & expanding_task);
bool is_required(const json & type_description);
bool this_task_exists_only_if_user_inputs(const json & expanding_task);
bool set_wildcard_task_posible_iterations(const json & type_details, json & expanding_task, size_t secondary_idx, fmt::dynamic_format_arg_store<fmt::format_context> & store);

const std::vector<string> fields_to_interpolate{
    "name","description",//"tag"
};

class expand_taskstory_t {
private:
    next_question_data_and_taskstory_input & m_nqdati;
    bool exapand_matrix(const json & type_details);
    json autofill_strategy(const json & type_description, const size_t main_idx, const size_t secondary_idx );
    json get_input_value(const json & type_description, const size_t main_idx, const size_t secondary_idx );
public:
    expand_taskstory_t(next_question_data_and_taskstory_input & nqdati):
    m_nqdati(nqdati){}

    bool expand_and_set(); // constructs into m_nqdati reference the expanded_taskstory
};


#endif //EXPANDED_TASKSTORY_T_H