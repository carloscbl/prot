#ifndef FORM_PARSER_H
#define FORM_PARSER_H
#include <string>
#include <map>
#include <iostream>
#include <any>
#include <array>
#include <memory>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class form_subsection_ADT{
public:
    string section_name;
    form_subsection_ADT(const json & j,string sec_name):section_name(sec_name){
        const auto & section_json = j[section_name];
        for(auto & [key, value]: section_json.items()){
            section[key] = value;
        }
        print_section();
    }
    void print_section(){
        cout << "Section: " << this->section_name << endl;
        for(const auto & [k, v]: section){
            cout << k << "--" << v << endl;
            //section["form.name"].value.type
            switch (v.type())
            {
                case json::value_t::string:
                    cout << "string" << endl;
                    break;
                case json::value_t::number_integer:
                    cout << "integer" << endl;
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
        }
    }
    map<string,json> section;
};

class answer_branches{
    map<string,int> branch_name_kind;
    map<string, kind_branch> kind_branch{
        {"range", make_range},
        {"predefined_boolean_yes_no_affirmative_yes", make_predef_bool},
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
    answer_branches branches;
    map<int,taskstory> taskstorys;
    questions(const json & j);
};

class form_parser{
private:
    const json & j;
    map<string,unique_ptr<form_subsection_ADT>> subsections;
    map<string,unique_ptr<form_subsection_ADT>> discover;
public:
    form_parser(const json & j);
    const array<string,5> subsection_names{
        "form","expressions","variables", "configurables", "questions"
    };
};

#endif //FORM_PARSER_H