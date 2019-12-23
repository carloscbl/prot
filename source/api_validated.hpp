#ifndef API_VALIDATED_H
#define API_VALIDATED_H
#include <string>
#include <fstream>
#include <iostream>
#include "json.hpp"
#include <boost/filesystem.hpp>

using std::string;
using std::cout;
using std::endl;
using nlohmann::json;

namespace fs = boost::filesystem;
using namespace fs;
class api_validated
{
private:
    const string definitions_file;
public:
    json api_json;

    api_validated(const string & api_file):definitions_file(api_file){
        boost::filesystem::path full_path(boost::filesystem::current_path());
        std::cout << "Current path is : " << full_path << std::endl;
        std::ifstream file(definitions_file, std::fstream::in);
        file >> api_json;
    };
    
    bool is_valid(const string & mapping_str) const{
        return api_json.find(mapping_str) != api_json.end();
    }
};

#endif //API_VALIDATED_H