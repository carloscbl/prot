#ifndef FORM_PARSER_H
#define FORM_PARSER_H
#include <string>
#include <map>
#include <iostream>
#include <any>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;
using bsjson = nlohmann::basic_json<>;

class form_subsection_ADT{
protected:
    string section_name;
    form_subsection_ADT(const json & j,string sec_name):section_name(sec_name){
        const auto & section_json = j[section_name];
        for(auto & [key, value]: section_json.items()){
            section[key] = value;
            // cout << key << "--" 
            // << any_cast<decltype(value)>(section[key])
            //  << endl;
        }
    }
    map<string,json> section;
};

class form_metadata : public form_subsection_ADT{
public:
    form_metadata(const json & j):form_subsection_ADT(j,"form"){

        for(const auto & [k, v]: section){
            cout << k << "--" << section[k] << endl;
        }
    }
};

class form_parser{
private:
    const json & j;
    //map<string> taskstories;
    form_metadata fm;
    //map<string,string> form_metadata;
    map<string,string> expressions;
    map<string,string> variables;
    map<string,string> configurables;
    map<string,string> questions;
public:
    form_parser(const json & j);


};

#endif //FORM_PARSER_H