#ifndef API_VALIDATED_H
#define API_VALIDATED_H
#include <string>
#include <fstream>
#include <iostream>
#include "json.hpp"

using std::string;
using std::cout;
using std::endl;
using nlohmann::json;

class api_validated
{
private:
    const string priority_definitions_file;
public:
    json api_json;

    api_validated(const string & api_file):priority_definitions_file(api_file){
        std::ifstream file(priority_definitions_file, std::fstream::in);
        file >> api_json;
    };
    
    bool is_valid(const string & mapping_str) const{
        return api_json.find(mapping_str) != api_json.end();
    }
};

#endif //API_VALIDATED_H