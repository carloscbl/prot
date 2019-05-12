#ifndef FORM_READER_H
#define FORM_READER_H
#include <string>
#include "json.hpp"

using json = nlohmann::json;
class form_reader
{
private:
    mutable json j;
    std::string original_file_path;
public:
    const json & get_json() const{
        return j;
    }
    form_reader(std::string file_path);
    void save_to_file();
    void save_to_file(std::string new_path);
};


#endif //FORM_READER_H