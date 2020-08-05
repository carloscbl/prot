#include "expanded_taskstory_t.h"



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
    auto expanded_taskstory = make_unique<json>();
    string main_tuple = get_matrix_group_by(type_details["subtypes"]);
    string secundary_tuple = main_tuple == "cols" ? "rows" : "cols";
    fmt::print("Hello, world!\n");
    for(auto [k,v] : m_nqdati.raw_taskstory.items()){
        if (!is_wildcard(v)){ continue; }
        
        for (size_t secondary_idx = 0; secondary_idx < type_details[secundary_tuple].get<size_t>(); secondary_idx++)
        { 
            /* eg: "group_by" : "rows", 
            ******* -> 3 diferent inputs, 1 each row per 7 tasks
            ******* ->
            ******* ->
            first iterate pert tasks (7)
            then iterate per input so we can generate all the input for a given task
            */
            json expanded_task = v;
            expanded_task.erase("wildcard_task");
            fmt::dynamic_format_arg_store<fmt::format_context> store;
            bool task_is_filled_with_significant_fields= false;
            for (size_t main_idx = 0; main_idx < type_details[main_tuple].get<size_t>(); main_idx++)
            {
                // store.push_back(fmt::arg("name", "Pepe3000"));
                auto type_description = type_details[main_tuple].at(main_idx); // type:STRING, description:"",string_interpolation_fields:{}...
                auto autofill = autofill_strategy(task_is_filled_with_significant_fields, type_description, main_idx, secondary_idx);
                if(autofill.has_value()){
                    
                    continue;
                }
            }
            // std::string interpolation = fmt::vformat(raw, store);
            if (task_is_filled_with_significant_fields){
                expanded_taskstory->push_back(v);
            }
        }
        for(auto [k,v] : type_details[""].items()){

        }
    }
    return expanded_taskstory;
}


void substitution_or_interpolation(const json & value, json & type_description, fmt::dynamic_format_arg_store<fmt::format_context> & store){
    auto interpolation = type_description.find("interpolation_name");
    auto substitute_whole_field = type_description.find("substitute_whole_field");

    if(interpolation != type_description.end()){
        store.push_back( fmt::arg(interpolation.value().get<string>().c_str(), prot::json_to_string(value).value() ));
    }

    if(substitute_whole_field != type_description.end() && !substitute_whole_field.value().empty()){
        // for (const auto & [_,field] : substitute_whole_field.value().items())
        // {
        //     // auto callable = task_subtype_checker.find(field.get<string>());
        // }
        
    }
}

optional<std::any> expand_taskstory_t::autofill_strategy(
    bool & task_is_filled_with_significant_fields,
    const json & type_description, size_t main_idx, size_t secondary_idx){
    auto autofill = type_description.find("user_input_autofill");
    if(autofill != type_description.end() ){
        // Fill store with, predefined input_data
        auto autofill_struct = autofill.value();
        std::any substitution_value;
        if ( autofill_struct["readonly"].get<bool>() == true){
            substitution_value = autofill_struct["input_data"].at(secondary_idx);
        }else{
            // Fill with user_input
            substitution_value = this->m_nqdati.user_input["data_input_from_user"].at(main_idx).at(secondary_idx);
            if(substitution_value.has_value()){
                task_is_filled_with_significant_fields = true;
            }
        }
        // type_description // format or complete replace field?
        // store.push_back(fmt::arg("name", substitution_value));
        return substitution_value;
    }
    return nullopt;
}

// in order to parse all the multiple specified fields and interpolations we need to convert a vector to variadic argument


string get_matrix_group_by(const json & subtypes){
    string type_ = subtypes["group_by"];
    // return subtypes["group_types"][type_];
    return type_;
}

bool is_wildcard(json raw_task){ // Returns the wil
    auto wild_task = raw_task.find("wildcard_task");
    if(wild_task == raw_task.end()) return false;

    auto enabled = wild_task.value().find("enabled");
    if(enabled == wild_task.value().end() && enabled.value().get<bool>() == false) return false;

    return true;
}