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

template <>
void answer_branches<string>::enroute(const json & j){
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
template<typename T>
void answer_branches<T>::enroute(const json & j){
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