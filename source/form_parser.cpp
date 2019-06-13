#include "form_parser.h"


form_parser::form_parser(const json & j):j(j){

    subsections = map<string,unique_ptr<form_subsection_ADT>>();
    for(const auto& sect : subsection_names){
        subsections[sect] = make_unique<form_subsection_ADT>(j,sect);
    }

    for(const auto & [k,v] : j.items()){
        const auto & it = find(subsection_names.begin(),subsection_names.end(),k);
        if(it != subsection_names.end()){
            continue;
        }
        discover[k] = make_unique<form_subsection_ADT>(j,k);
    }
    form_traverse();
    //answer_branches<string> branches(j,"Lemonade");
}

int form_parser::enroute_json_type(const json & v){
    switch (v.type())
    {
        case json::value_t::string:
            cout << "string" << endl;
            return answer_branches(v,v.get<string>()).get_next_branch();
            break;
        case json::value_t::number_integer:
            cout << "integer" << endl;
            
            return answer_branches(v,v.get<int>() ).get_next_branch();
            break;
        case json::value_t::number_unsigned:
            cout << "unsigned" << endl;
            break;
        case json::value_t::number_float:
            cout << "float" << endl;
            break;
        case json::value_t::null:
            cout << "null" << endl;
            break;
        case json::value_t::object:
            cout << "object" << endl;
            break;
        case json::value_t::array:
            cout << "array" << endl;
            break;
        case json::value_t::boolean:
            cout << "boolean" << endl;
            break;
        case json::value_t::discarded:
            cout << "discarded" << endl;
            break;
        default:
            cout << "number" << endl;
            break;
    }
    return 0;
}

int form_parser::get_next_id(){
    const auto & error_ = static_cast<int>(e_branches::ERROR_JSON);
    int next = error_;
    if(this->current_id == static_cast<int>(e_branches::START)){
        next = static_cast<int>(e_branches::FIRST);
        return next;
    }
    for( auto [k,v] : subsections["questions"]->section ){
        next = enroute_json_type(v);
        if(next != error_){
            //Means we got a valid routing, so lets move on
            break;
        }
    }
    return next;
}

optional<json> form_parser::find_questions_by_id(int id) noexcept{
    for(const auto & [k,v] : this->subsections["questions"]->section ){
        if(v["id"] == id){
            return v;
        }
    }
    return std::nullopt;
}
        

template<typename T>answer_branches<T>::answer_branches(const json & j, const T & answer, function<T(T)> answer_transformation_strategy_)
:answer(answer)
{
    if (answer_transformation_strategy_ != nullptr)
    {
        this->answer_transformation_strategy = answer_transformation_strategy_;
    }
    enroute(j);
}

form_subsection_ADT::form_subsection_ADT(const json & j,string sec_name):section_name(sec_name){
    const auto & section_json = j[section_name];
    for(auto & [key, value]: section_json.items()){
        section[key] = value;
    }
    //print_section();
}

void form_subsection_ADT::print_section(){
    cout << "Section: " << this->section_name << endl;
    for(const auto & [k, v]: section){
        cout << k << "--" << v << endl;
        //section["form.name"].value.type
    }
}


template<typename T>void answer_branches<T>::print_out(){
    cout 
    << this->answer << " : "
    << this->next_branch_result.value_or(default_brach_error)
    << endl;
}

template <>void answer_branches<string>::enroute(const json & j){
    //for mapped strategies
    for(const auto & [k,v] : j.items()){
        //We test for every structure in order
        const auto & it = kind_branch_t_map.find(k);//matching json structure
        const auto & opt = it->second.func(this->answer,it->second.answer_type);
        if(opt.has_value()){
            next_branch_result = opt;
            break;
        }
    }
    const auto & modulated_answer = answer_transformation_strategy(answer);
    //Match custom selectors
    if(j.find(modulated_answer) != j.end()){
        //"Yes":2, "YEEEESSS":2 , "No":5
        next_branch_result = j[modulated_answer];
    }else{
        next_branch_result = std::nullopt;
    }
}
template<typename T>void answer_branches<T>::enroute(const json & j){
    //for mapped strategies
    for(const auto & [k,v] : j.items()){
        //We test for every structure in order
        const auto & it = kind_branch_t_map.find(k);//matching json structure
        const auto & opt = it->second.func(this->answer,it->second.answer_type);
        if(opt.has_value()){
            next_branch_result = opt;
            break;
        }
    }
}

