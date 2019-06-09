#ifndef FORM_PARSER_H
#define FORM_PARSER_H
#include <string>
#include <map>
#include <iostream>
#include <any>
#include <array>
#include <memory>
#include <optional>
#include "json.hpp"
#include "user.h"

using namespace std;
using json = nlohmann::json;
using enum_of_json_t = nlohmann::detail::value_t;


class form_subsection_ADT{
public:
    string section_name;
    form_subsection_ADT(const json & j,string sec_name):section_name(sec_name){
        const auto & section_json = j[section_name];
        for(auto & [key, value]: section_json.items()){
            section[key] = value;
        }
        //print_section();
    }
    void print_section(){
        cout << "Section: " << this->section_name << endl;
        for(const auto & [k, v]: section){
            cout << k << "--" << v << endl;
            //section["form.name"].value.type
            
        }
    }
    map<string,json> section;
};


template <typename T>
class answer_branches{
private:
    using answer_t = T;
    using answer_type_t = string;
    using function_signature_t = function<optional<int>(answer_t, answer_type_t)>;

    struct kind_branch_t{
        answer_type_t answer_type;
        function_signature_t func;
    };
    optional<int> next_branch_result = std::nullopt;
    //answer_t value_err = "ERROR";
    const T answer;
    function<T(T)> answer_transformation_strategy = [](T s) -> T {return s;};
    static int plzhdr(T answer, string answer_type){return 0;}

    //C++17, with inline you can use header :O
    inline const static map<string_view, kind_branch_t> kind_branch_t_map{
        {"range",kind_branch_t{"INTEGER",plzhdr}},
        {"predefined_boolean_yes_no_affirmative_yes", kind_branch_t{"BOOLEAN",plzhdr}},
    };

    void enroute(const json & j){
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

public:
    static const int default_brach_error = -9999;
    answer_branches(const json & j,const T & answer, function<T(T)> answer_transformation_strategy_ = nullptr)
    :answer(answer)
    {
        if (answer_transformation_strategy_ != nullptr)
        {
            this->answer_transformation_strategy = answer_transformation_strategy_;
        }
        
        enroute(j);
    }

    int get_next_branch(){
        return next_branch_result.value_or(default_brach_error);
    }

    void print_out(){
        cout 
        << this->answer << " : "
        << this->next_branch_result.value_or(default_brach_error)
        << endl;
    }
};

class taskstory{
    string command;
    string name;
};

class taskstorys{
    int branch_id;
    vector<taskstory> taskstory_;
};

class questions{
    int id;
    string question;
    string answer_tag;
    string type_user_input;
    vector<string> mode;
    
    map<int,taskstory> taskstorys;
    questions(const json & j);
};

//This class handles the formation of a executable machine of states for the user answers flow, and its correct storage and publish
//Which souns like a to much from a point of design, but lets refactor this ion the future
//Lets try to keep us from add more indirection, to find a more direct and concise solution
class form_parser{
private:
    const json & j;
    map<string,unique_ptr<form_subsection_ADT>> subsections;
    map<string,unique_ptr<form_subsection_ADT>> discover;

    int enroute_json_type(const json & v){
        switch (v.type())
        {
            case json::value_t::string:
                cout << "string" << endl;
                return answer_branches(v,v.get<string>()).get_next_branch();
                break;
            case json::value_t::number_integer:
                cout << "integer" << endl;
                
                //return answer_branches(v,v.value.get<int>() ).get_next_branch();
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

    int get_next_id(){
        const auto & error_ = answer_branches<int>::default_brach_error;
        int next = error_;
        for( auto [k,v] : subsections["questions"]->section ){
            next = enroute_json_type(v);
            if(next != error_){
                //Means we got a valid routing, so lets move on
                break;
            }
        }
        return next;
    }

    optional<json> find_by_id(int id) noexcept{
        
        for(const auto [k,v] : this->subsections["questions"]->section ){
            if(v["id"] == id){
                return nullopt;
            }
        }
        return std::nullopt;
    }
    
    void form_traverse(){
        //subsections["questions"]->section[get_next_id()];
    }

    void form_publisher_vars(){}
    void form_command_tasks(){}
    void form_ready(){}
    void form_pipeline(){
        form_ready();
        form_traverse();
        form_publisher_vars();
        form_command_tasks();
    }

public:
    form_parser(const json & j);
    const array<string,5> subsection_names{
        "form","expressions","variables", "configurables", "questions"
    };
    void form_run(user user){
        //Good idea to thread pool this call 
        //Here is the point of concurrence, once a form is readed and loaded, and a user is responding questions
        form_pipeline();
    }
};


#endif //FORM_PARSER_H