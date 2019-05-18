#ifndef FORM_PARSER_H
#define FORM_PARSER_H
#include <string>
#include <map>
#include <iostream>
#include <any>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class form_metadata{
public:
    form_metadata(const json & j){
        const auto & metadata_json = j["form"];
        std::cout << metadata_json.dump(4) << std::endl;
        for (json::const_iterator it = metadata_json.cbegin(); it != metadata_json.cend(); ++it) {
            std::cout << *it << '\n'; //* operator seems to be overloaded to point to the value
        }
        for(auto & [key, value]: metadata_json.items()){
            metadata[key] = value;
            cout << key << "--" 
            << any_cast<decltype(value)>(metadata[key])
             << endl;
        }
    }
private:
    map<string,any> metadata;
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