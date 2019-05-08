#include <string>
#include <map>
#include <any>
#include "type_container.h"
using namespace std;


std::map<string,any> conversors_map{
    {"INTEGER", [](string s){ return std::stoi(s);}},
    {"FLOAT", [](string s){ return std::stof(s);}},
    {"BOOL", [](string s){ return std::stod(s);}},
    {"STRING", [](string s){ return s;}},
};
template<typename T >
type_container<T>::type_container(string type, T value ):value(value),type(type){
    //"87.6" comming from value with type float, or string
    const auto & it = conversion_table.find(type);
    if(it != conversion_table.end()){
        conversion_table[type](value); valid = true;
    } 
}
