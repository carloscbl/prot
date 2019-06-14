#ifndef TYPE_CONTAINER_H
#define TYPE_CONTAINER_H
#include <string>
#include <functional>
#include <map>
#include <any>


using namespace std;

std::map<string,function<any(string)>> conversors_map{
    {"INTEGER", (function<int(string)>)[](string s) -> int{ return std::stoi(s);}},
    {"FLOAT", (function<float(string)>)[](string s)->float{ return std::stof(s);}},
    {"BOOL", (function<bool(string)>)[](string s)->bool{ return (bool)std::stoi(s);}},
    {"STRING", (function<string(string)>)[](string s)->string{ return s;}},
};


#endif //TYPE_CONTAINER_H