#ifndef TYPE_CONTAINER_H
#define TYPE_CONTAINER_H
#include <string>
#include <optional>
#include <functional>
#include <map>
#include <any>


using namespace std;
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

std::map<string,function<any(string)>> conversors_map{
    {"INTEGER", (function<any(string)>)[](string s) -> any{ return get_value<int>(s,strtol,10) ; }},
    {"DOUBLE", (function<any(string)>)[](string s)->any{ return get_value<double>(s,strtod);}},
    // {"BOOL", (function<optional<bool>(string)>)[](string s) -> bool{ return static_cast<bool>(strtol(s.c_str(), &p, 10));}},
    {"STRING", (function<any(string)>)[](string s)->any{ return s;}},
    {"SELECT", (function<any(string)>)[](string s)->any{ return s;}},
    {"MATRIX", (function<any(string)>)[](string s)->any{ return s;}},
    {"VECTOR", (function<any(string)>)[](string s)->any{ return s;}},
};

#endif //TYPE_CONTAINER_H
