#include "form_reader.h"
#include <iostream>
#include <fstream>
#include <iomanip>

form_reader::form_reader(const std::string &file_path) : original_file_path(file_path)
{
    std::ifstream i(file_path);
    i >> j;

    //j["pi"] = 3.141; Example
    //std::cout << j.dump(4) << std::endl;

    //std::ofstream o("pretty.json");
    //o << std::setw(4) << j << std::endl;
}

void form_reader::save_to_file()
{
    std::ofstream o(original_file_path);
    o << std::setw(4) << j << std::endl;
}

void form_reader::save_to_file(std::string new_path)
{
    std::ofstream o(new_path);
    o << std::setw(4) << j << std::endl;
}

void form_reader::reload()
{
    std::ifstream i(original_file_path);
    i >> this->j;
}