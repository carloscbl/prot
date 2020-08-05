#pragma once
#ifndef TYPE_CONVERSOR_H
#define TYPE_CONVERSOR_H

#include <string>
#include <optional>
#include <functional>
#include <map>
#include <any>
#include "json.hpp"
#include <fmt/core.h>
#include <fmt/format.h>

using nlohmann::json;
using std::function;
using std::map;
using std::any;
using std::nullopt;
using std::optional;


using namespace std;


namespace prot{

template<typename T,typename Y,typename ... args>
std::any get_value(string s,Y conversor, args ... extra){
    char * p = nullptr;
    T ret;
    ret = static_cast<T>(conversor(s.c_str(), &p, extra...));
    if(*p != '\0'){
        return std::any();
    }else{
        return ret;
    }
}
const inline std::map<string,function<std::any(const json &)>> conversors_map{
    {"INTEGER",(function<std::any(const json &)>)[](const json & s)->std::any{ return s["data_input_from_user"].get<int>(); }},
    {"DOUBLE", (function<std::any(const json &)>)[](const json & s)->std::any{ return s["data_input_from_user"].get<double>();}},
    {"BOOL",   (function<std::any(const json &)>)[](const json & s)->std::any{ return s["data_input_from_user"].get<bool>();}},
    {"STRING", (function<std::any(const json &)>)[](const json & s)->std::any{ return s["data_input_from_user"].get<string>();}},
    {"SELECT", (function<std::any(const json &)>)[](const json & s)->std::any{ return s;}},
    {"MATRIX", (function<std::any(const json &)>)[](const json & s)->std::any{ return s;}},
    {"VECTOR", (function<std::any(const json &)>)[](const json & s)->std::any{ return s;}},
};
optional<std::string> json_to_string(const json & value);
};



#endif //TYPE_CONVERSOR_H
