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
            json expanding_task = v;
            expanding_task.erase("wildcard_task");
            fmt::dynamic_format_arg_store<fmt::format_context> store;
            bool invalid_input = false;

            for (size_t main_idx = 0; main_idx < type_details[main_tuple].get<size_t>(); main_idx++)
            {
                // store.push_back(fmt::arg("name", "Pepe3000"));
                // fmt::print("{}", type_details["subtypes"]["group_types"][main_tuple].dump(4));
                auto type_description = type_details["subtypes"]["group_types"][main_tuple].at(main_idx); // type:STRING, description:"",

                json autofill_value = autofill_strategy( type_description, main_idx, secondary_idx);
                if( !prot::check_null_or_empty(autofill_value) )
                {
                    substitution_or_interpolation_store(autofill_value, type_description, store, expanding_task); // We can check if there was a failure
                    continue;
                }

                // get value from input not from type description
                auto input_value = get_input_value( type_description, main_idx, secondary_idx );
                if(prot::check_null_or_empty(input_value)){
                    invalid_input = true;
                    break;
                }
                substitution_or_interpolation_store( input_value, type_description, store, expanding_task); // We can check if there was a failure

            }
            if(invalid_input){
                continue;
            }
            //After get the whole store from the main, we should apply it
            std::for_each(fields_to_interpolate.begin(),fields_to_interpolate.end(), [&store,&expanding_task](const string & field_string){
                std::cout << expanding_task[field_string].get<string>() << std::endl;
                
                std::string interpolation = fmt::vformat(expanding_task[field_string].get<string>(), store);
                expanding_task[field_string] = interpolation;
            });
            std::cout << expanding_task.dump(4) << std::endl;
            *expanded_taskstory += expanding_task; // insert the expanded task
        }
    }
    return expanded_taskstory;
}

json expand_taskstory_t::get_input_value(const json & type_description, const size_t main_idx, const size_t secondary_idx ){
    return this->m_nqdati.user_input["data_input_from_user"].at(main_idx).at(secondary_idx);
}

bool substitution_or_interpolation_store(const json & value,
 const json & type_description,
 fmt::dynamic_format_arg_store<fmt::format_context> & store,
 json & expanding_task){
    auto interpolation = type_description.find("interpolation_name");
    auto substitute_whole_field = type_description.find("substitute_whole_field");

    bool success = true; // Good candidate to reporting structure

    if(interpolation != type_description.end()){
        std::cout << "ADDING ARG: " << interpolation.value().get<string>() << " : " << prot::json_to_string(value).value() << std::endl;
        store.push_back( fmt::arg(interpolation.value().get<string>().c_str(), prot::json_to_string(value).value() ));
    }

    if(substitute_whole_field != type_description.end() && !substitute_whole_field.value().empty()){
        for (const auto & [idx,field] : substitute_whole_field.value().items()) // returns idx it (0,1,2) and array it
        {   
            string field_name = field.get<string>();
            auto pair_callable = prot::task_subtype_checker_and_adaptor.find(field_name);
            if (pair_callable == prot::task_subtype_checker_and_adaptor.end()){
                // success = false;
                fmt::print("Field {0} not match validator, maybe uninplemented or not valid one", field_name );
                continue;
            }
            const json checked_value = pair_callable->second(value);
            // now we can safely substitute
            expanding_task[field_name] = checked_value;
        }
    }
    return success;
}

json expand_taskstory_t::autofill_strategy(
    const json & type_description, size_t main_idx, size_t secondary_idx)
{
    auto autofill = type_description.find("user_input_autofill");
    if(autofill != type_description.end() ){
        auto autofill_struct = autofill.value();
        json substitution_value;
        if ( autofill_struct["readonly"].get<bool>() == true){
            substitution_value = autofill_struct["input_data"].at(secondary_idx);
        }else{
            substitution_value = this->m_nqdati.user_input["data_input_from_user"].at(main_idx).at(secondary_idx);
        }
        return substitution_value;
    }
    return json();
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