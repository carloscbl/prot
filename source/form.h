//
// Created by carlos on 2/03/19.
//

#ifndef FORM_H
#define FORM_H

#include <string>
#include <map>
#include <functional>
#include <memory>

#include "iactionable.h"
#include "icommandable.h"
#include "form_traverser.h"
#include "form_reader.h"

using namespace std;

class form;//Handles the result, executable of a form, and passes through to its components to auto compose
namespace env {
  static map<string, unique_ptr<form>> forms;
}

using question_node_id = int;
using subaction_map = map<string, function<void(map<char, string>)>>;
using map_f = map<char, function<void(form &, string)> >;
using state_machine = map<question_node_id, question_node>;

//the form class stores the form in raw internally and process it to be able to present it externally
class form : public iactionable {
private:
  //Components
  form_traverser ftraverser;
  unique_ptr<form_reader> json_reader;

  //This map, handles the posible actions to be performed from outside commands
  //Positional params
  subaction_map form_map{
    {"add",    [this](map<char, string> s) {
      //Creates a new form and store it with index id and passing a states machine graph
      unique_ptr<form> form_ = make_unique<form>();
      form_->add(s);
      env::forms[form_->id] = move(form_);
    }},
    {"remove", [](map<char, string> s) {}},
    {"update", [](map<char, string> s) {}},
  };

  //Modulator params like -h -n -P
  map_f setters{ 
    {'P', &form::set_path},
  };
  void set_path(const string & s);
  state_machine test_filler_();
public:
  string id;
  string name = "Washer_family_stationale_per_kg";
  //TODO, resolve the variabilitization of the taskstory
  //The taskstory handles externally all the posible commands for this form
  map<string, string> taskstory{
    {"a1", "task add -hmDMYnd %d %d %d %d %d \"Collect the clothes over home\" \"Gather all the clothes susceptible to be washed and put them in the washer \""},
    {"a2", "task add -nd \"Put the washer\" \"Turn on the machine\""},
    {"a3", "task add -nd \"Collect the clothes\" \"Hang out the washing \""},
    {"a4", "task add -nd \"Iron the clothes\" \"Take a while with some interesting podcast or netflix an let perfect the clothes for the week \""},
  };

  form(const std::string & path);
  form();
  virtual ~form();

  //iactionable
  void send_action(std::string action, map<char, string> params) override;
  //mapped to add
  void add(map<char, string> params);
  void perform_taskstory(string s);
};

#endif //FORM_H
