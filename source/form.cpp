#include "form.h"
#include <iostream>
using namespace std;


form::~form()
{
    cout << "form destroyed" << endl;
}



void form::pipelined_json_data_setter(const string &json_path)
{
    json_reader = make_unique<form_reader>(json_path);
    this->name = get_form_name(json_reader->get_json());
}

void from_json(const nlohmann::json& ref_json, form& new_form){

}