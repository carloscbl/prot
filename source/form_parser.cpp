#include <typeindex>
#include <typeinfo>
#include "form_parser.h"
#include "type_container.h"

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
}

template <typename T>
strategy_return make_get_next_branch(const json & question_obj, any arg ){
    return answer_branches<T>(question_obj, arg ).get_next_branch();
}

std::unordered_map<std::type_index, function<strategy_return(const json &,any)>> type_names{
    {std::type_index(typeid(int)),make_get_next_branch<int>},
    {std::type_index(typeid(string)), make_get_next_branch<string> },
    {std::type_index(typeid(double)),make_get_next_branch<double>},
    {std::type_index(typeid(float)),make_get_next_branch<float>},
};

strategy_return form_parser::enroute_json_type(const json & question_obj, const string & answer){
    
    string expected_answer_type = question_obj["type_user_input"].get<string>();
    std::any converted_answer;
    auto conversor = conversors_map.find(expected_answer_type);
    if(conversor != conversors_map.end()){
        converted_answer = conversor->second(answer);
        auto func = type_names[std::type_index(converted_answer.type())];
        return func(question_obj,converted_answer);
    }
    return strategy_return{};
}

next_question_data form_parser::get_next(const string & answer){
    json question;
    if(this->next_branch_id == static_cast<int>(e_branches::START)){
        //Means is first time
        question = find_questions_by_id(static_cast<int>(e_branches::FIRST)).value();
    }else{
        question = find_questions_by_id(this->next_branch_id).value();
    }
    auto strategy_returned = enroute_json_type(question,answer);//v is question{} object
    this->next_branch_id = strategy_returned.if_branch;
    cout << "taskstory " << strategy_returned.taskstory_id << endl;

    auto nextQ = find_questions_by_id(this->next_branch_id);
    string next_question;
    if(nextQ.has_value()){

        next_question = nextQ.value()["question"].get<string>();
    }else{
        next_question = "END";
    }
    next_question_data nqd{
        next_question,
        question["taskstories"][strategy_returned.taskstory_id]
    };
    return nqd;
}

optional<json> form_parser::find_questions_by_id(int id) noexcept{
    for(const auto & [k,v] : this->subsections["questions"]->section ){
        if(v["id"] == id){
            return v;
        }
    }
    return std::nullopt;
}

template<typename T>answer_branches<T>::answer_branches(const json & question_obj, const any & answer, function<T(T)> answer_transformation_strategy_)
:question_obj(question_obj),
answer(answer)
{
    if (answer_transformation_strategy_ != nullptr)
    {
        this->answer_transformation_strategy = answer_transformation_strategy_;
    }
    enroute(question_obj["answers_branches"]);
}

form_subsection_ADT::form_subsection_ADT(const json & j,string sec_name):section_name(sec_name){
    const auto & section_json = j[section_name];
    for(auto & [key, value]: section_json.items()){
        section[key] = value;
    }
}

void form_subsection_ADT::print_section(){
    cout << "Section: " << this->section_name << endl;
    for(const auto & [k, v]: section){
        cout << k << "--" << v << endl;
    }
}


template<typename T>void answer_branches<T>::print_out(){
    cout 
    << any_cast<T>(this->answer) << " : "
    << this->next_branch_result.value_or(default_brach_error)
    << endl;
}

template <>void answer_branches<string>::enroute(const json & j){
    //for mapped strategies
    for(const auto & [k,v] : j.items()){
        //We test for every structure in order
        const auto & it = kind_branch_t_map.find(k);//matching json structure
        if(it == kind_branch_t_map.end()){
            cout <<"non existing map implementation! for kind_branch_t_map and: "<< k << endl;
            return;
        }
        
        const auto & opt = it->second(v,any(this->answer));
        cout << "the next is: " << opt.value().taskstory_id << endl;
        if(opt.has_value()){

            next_branch_result = opt;
            return;
        }
    }
    //////////#### CUSTOM SELECTORS ####/////////
    const auto & modulated_answer = any_cast<string>(answer); // = answer_transformation_strategy(answer);
    //Match custom selectors
    if(j.find(modulated_answer) != j.end()){
        //"Yes":2, "YEEEESSS":2 , "No":5
        next_branch_result = strategy_return{
            .if_branch = j[modulated_answer],
            .taskstory_id = j["taskstory_id"]
        };
    }else{
        next_branch_result = std::nullopt;
    }
}

template<typename T>void answer_branches<T>::enroute(const json & j){
    //for mapped strategies
    for(const auto & [k,v] : j.items()){
        //We test for every structure in order
        const auto & it = kind_branch_t_map.find(k);//matching json structure
        if(it == kind_branch_t_map.end()){
            //cout << j.dump(4) << endl;
            cout << "Not implemented or not finded implementation" << endl;
            return;
        }
        const auto & opt = it->second(v,any(this->answer));
        cout << "the next is: " << opt.value().if_branch << endl;
        if(opt.has_value()){
            next_branch_result = opt;
            break;
        }
    }
}

