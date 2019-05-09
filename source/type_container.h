#ifndef TYPE_CONTAINER_H
#define TYPE_CONTAINER_H
#include <string>
#include <functional>
#include <map>
#include <any>


using namespace std;

std::map<string,any> conversors_map{
    {"INTEGER", (function<int(string)>)[](string s) -> int{ return std::stoi(s);}},
    {"FLOAT", (function<float(string)>)[](string s)->float{ return std::stof(s);}},
    {"BOOL", (function<bool(string)>)[](string s)->bool{ return (bool)std::stoi(s);}},
    {"STRING", (function<string(string)>)[](string s)->string{ return s;}},
};


//We want to store the answer given and parse it to de desired type
//So we pass it
template<typename T> 
class type_container{
public:
    type_container(string type, string answer ):type(type){
        //"87.6" comming from value with type float, or string
        const auto & it = conversors_map.find(type);
        if(it != conversors_map.end()){
            auto expr = any_cast<function<T(string)> >(conversors_map[type]);
            value = expr(answer);
            valid = true;
        }
    }

    T value;
    string type;
    bool valid = false;
};

#endif //TYPE_CONTAINER_H