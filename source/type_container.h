#ifndef TYPE_CONTAINER_H
#define TYPE_CONTAINER_H
#include <string>
#include <functional>
#include <cstdlib>
#include <map>
#include <any>


using namespace std;

std::map<string,function<any(string,char *)>> conversors_map{
    {"INTEGER", (function<int(string,char *)>)[](string s, char * p) -> int{ return stoi(s, &p, 10);}},
    {"FLOAT", (function<float(string,char *)>)[](string s)->float{ return std::stof(s);}},
    {"BOOL", (function<bool(string,char *)>)[](string s)->bool{ return (bool)std::stoi(s);}},
    {"STRING", (function<string(string,char *)>)[](string s)->string{ return s;}},
};


#endif //TYPE_CONTAINER_H