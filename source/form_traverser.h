//
// Created by carlos on 2/03/19.
//

#ifndef FORM_TRAVERSER_H
#define FORM_TRAVERSER_H

#include <map>
#include <memory>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct branch
{
  //This struct designate the source of actions that will be executed because of this branch election
  vector<string> local_taskstory;
  int next_node_id;
  //map<string,string> params;
};
struct question_node{
  //Handle the base question, with the Q and stores the answer given, but the posible branches from this
  //As.. the usual bool case, no -> Exit | yes -> question id 2
    using posible_answer_given = string;

    int                              id               = 0;
    string                           Question;
    string                           Answer;
    map<posible_answer_given,branch> branches;//"yes" : branch {"task add -n washer", UsualStates::exit};
    //set<string> tags;
};

enum class UsualStates : int {
  exit = -1
};

class form_traverser {
private:
    using question_node_id = int;
    map<question_node_id, question_node > state_machine;

    UsualStates us = UsualStates::exit;
    bool is_valid_= false;
    void validate_form();
public:

    form_traverser(map<question_node_id, question_node> questions):state_machine(questions){validate_form();}
    form_traverser() = default;
    bool is_valid(){return is_valid_;}
    void run();
    void process_branches_outputs(question_node node) const noexcept;
    void print_out() const;

    inline void set_questions(map<question_node_id, question_node > questions){
      state_machine = questions;
      validate_form();
    }
};


#endif //FORM_TRAVERSER_H
