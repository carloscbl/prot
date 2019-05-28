#ifndef FORM_PARSER_H
#define FORM_PARSER_H
#include <string>
#include <map>
#include <iostream>
#include <any>
#include <memory>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class form_subsection_ADT{
protected:
    string section_name;
    form_subsection_ADT(const json & j,string sec_name):section_name(sec_name){
        const auto & section_json = j[section_name];
        for(auto & [key, value]: section_json.items()){
            section[key] = value;
        }
    }
    void print_section(){
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

class form_metadata : public form_subsection_ADT{
public:
    form_metadata(const json & j):form_subsection_ADT(j,"form"){
        print_section();
    }
};

class form_parser{
private:
    const json & j;
    map<string,unique_ptr<form_subsection_ADT>> subsections;
public:
    form_parser(const json & j);


};

#endif //FORM_PARSER_H