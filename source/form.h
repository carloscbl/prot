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

using namespace std;

class form;
namespace env {
  static map<string, unique_ptr<form>> forms;
}

using question_node_id = int;
using subaction_map = map<string, function<void(map<char, string>)>>;
using map_f = map<char, function<void(form &, string)> >;
using state_machine = map<question_node_id, question_node>;


class form : public iactionable {
private:
  //Components
  form_traverser ftraverser;

  subaction_map form_map{
    {"add",    [this](map<char, string> s) {
        //we should parse p, with a new component called form parser, and pass it to the map question node

      //Creates a new form and store it with index id and passing a states machine graph
      unique_ptr<form> form_ = make_unique<form>(test_filler_());
      form_->add(s);
      env::forms[form_->id] = move(form_);
    }},
    {"remove", [](map<char, string> s) {}},
    {"update", [](map<char, string> s) {}},
  };

  map_f setters{
    {'n', &form::perform_taskstory},
  };

  state_machine test_filler_();
public:
  string id;

  form(state_machine questions);

  form();

  virtual ~form();

  void send_action(std::string action, map<char, string> params) override;

  string name = "Washer_family_stationale_per_kg";


  map<string, string> taskstory{
    {"a1", "task add -hmDMYnd %d %d %d %d %d \"Collect the clothes over home\" \"Gather all the clothes susceptible to be washed and put them in the washer \""},
    {"a2", "task add -nd \"Put the washer\" \"Turn on the machine\""},
    {"a3", "task add -nd \"Collect the clothes\" \"Hang out the washing \""},
    {"a4", "task add -nd \"Iron the clothes\" \"Take a while with some interesting podcast or netflix an let perfect the clothes for the week \""},
  };

  void add(map<char, string> params);

  void perform_taskstory(string s);
};

#endif //FORM_H
