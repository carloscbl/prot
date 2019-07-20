 //
// Created by carlos on 2/03/19.
//

#ifndef FORM_H
#define FORM_H

#include <string>
#include <map>
#include <functional>
#include <memory>

#include "CRUD_actionable.h"
#include "form_reader.h"
#include "form_parser.h"

using namespace std;


class form;





//the form class stores the form in raw internally and process it to be able to present it externally
class form : public CRUD_actionable<form> {
private:
  //Components
  unique_ptr<form_reader> json_reader;
  unique_ptr<form_parser> json_parser;

  //This map, handles the posible actions to be performed from outside commands
  //Positional params
  CRUD_plus_actions_map form_map{
    {"add",    [this](map<char, string> s) {
      //Creates a new form and store it with index id and passing a states machine graph
      unique_ptr<form> form_ = make_unique<form>();
      form_->add(s);
      //aaa();
      form::forms[form_->id] = move(form_);
    }},
    {"remove", [](map<char, string> s) {}},
    {"update", [](map<char, string> s) {}},
    {"compute", [](map<char, string> s) {}},
    {"list", [](map<char, string> s) {
      for(const auto & form : form::forms){
        cout << form.second->name << endl;
      }
    }},
  };
  void aaa(){
    cout << form::forms.cbegin()->second->name << endl;
  }

  //Modulator params like -h -n -P
  map_local_functions setters{ 
    {'P', &form::set_path},
  };
  
  void set_path(const string & s);
  void pipelined_json_data_setter(const string & json_path);
public:
  inline static map<string, unique_ptr<form>> forms;
  bool ready_to_compute = false;
  string id;
  string name;//name of the form
  map<string, string> taskstory;//The taskstory handles all the posible commands for this form

  form();
  form(const std::string & path);
  virtual ~form();

  //iactionable
  void send_action(std::string action, map<char, string> params) override;

};

#endif //FORM_H
