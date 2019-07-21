#include "form.h"
#include <iostream>
using namespace std;



form::form():CRUD_actionable(form_map,setters){
  static int acummulator = 0;
  id = "form" + to_string(acummulator++);

}
form::form(const string & path):form(){
  pipelined_json_data_setter(path);
}

form::~form()
{
  cout << "form destroyed" << endl;
}

void form::send_action(std::string action, map<char, string> params)
{
  if (form_map.find(action) != form_map.end())
  {
    form_map[action](params);
  }
  else
  {
    cout << action << " :Does not match. Not provided correct arguments" <<endl;
  }
}

void form::set_path(const string & s){
  const auto & path = s;
  try
  {
    pipelined_json_data_setter(path);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    std::cerr << "Wrong File Path or bad format JSON" << '\n';
  }
  
}

void form::pipelined_json_data_setter(const string & json_path){
  json_reader = make_unique<form_reader>(json_path);
  this->name = get_form_name(json_reader->get_json());
}