#pragma once
#ifndef TYPE_CONVERSOR_H
#define TYPE_CONVERSOR_H

#include <string>
#include <optional>
#include <functional>
#include <map>
#include <unordered_map>
#include <any>
#include "json.hpp"
#include "duration.h"
#include <fmt/core.h>
#include <fmt/format.h>

using nlohmann::json;
using std::function;
using std::unordered_map;
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

const inline unordered_map<string, string> period_label_to_frequency{
    {"day_week", "weekly"},
    {"day_month", "monthly"},
    {"day_year", "yearly"},
};


const inline map<string, function<json(const json &)>> task_subtype_checker_and_adaptor{
    {"duration",
        [](const json & type)-> json { 
            prot::duration d;
            type.at("data_input_from_user").get_to(d);
            json checked_duration;
            to_json(checked_duration, d);
            return checked_duration;
        }
    },
};

optional<std::string> json_to_string(const json & value);
bool check_null_or_empty(const json & value);

};



#endif //TYPE_CONVERSOR_H
