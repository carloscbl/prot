#ifndef FORM_READER_H
#define FORM_READER_H
#include "json.hpp"
#include <iostream>
#include <iomanip>
using json = nlohmann::json;
class form_reader
{
private:
    mutable json j;
public:
    const json & get_json() const{
        return j;
    }
    form_reader(std::string file_path){
        
        file_path = "../source/design/washclothes.json";
        std::ifstream i(file_path);
        i >> j;

        //j["pi"] = 3.141; Example
        std::cout << j.dump(4) << std::endl;

        std::ofstream o("pretty.json");
        o << std::setw(4) << j << std::endl;
    }

};


#endif //FORM_READER_H