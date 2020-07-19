#ifndef TYPE_CONTAINER_H
#define TYPE_CONTAINER_H
#include <string>
#include <optional>
#include <functional>
#include <map>
#include <any>


using namespace std;
// DEPRECATED
template<typename T,typename Y,typename ... args>
any get_value(string s,Y conversor, args ... extra){
    char * p = nullptr;
    T ret;
    ret = static_cast<T>(conversor(s.c_str(), &p, extra...));
    if(*p != '\0'){
        return any();
    }else{
        return ret;
    }
}

std::map<string,function<any(const json &)>> conversors_map{
    {"INTEGER",(function<any(const json &)>)[](const json & s)->any{ return s["data_input_from_user"].get<int>(); }},
    {"DOUBLE", (function<any(const json &)>)[](const json & s)->any{ return s["data_input_from_user"].get<double>();}},
    {"BOOL",   (function<any(const json &)>)[](const json & s)->any{ return s["data_input_from_user"].get<bool>();}},
    {"STRING", (function<any(const json &)>)[](const json & s)->any{ return s["data_input_from_user"].get<string>();}},
    {"SELECT", (function<any(const json &)>)[](const json & s)->any{ return s;}},
    {"MATRIX", (function<any(const json &)>)[](const json & s)->any{ return s;}},
    {"VECTOR", (function<any(const json &)>)[](const json & s)->any{ return s;}},
};

#endif //TYPE_CONTAINER_H
