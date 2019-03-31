#include "form.h"
#include <iostream>
using namespace std;


form::form(state_machine questions):form(){
  ftraverser.set_questions(questions);
}

form::form(){
  static int acummulator = 0;
  id = "form" + to_string(acummulator++);
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


void form::add(map<char,string> params){
  if(params.size() == 0) cout << "nothing done" << endl;

  this->perform_taskstory("");

  auto it = setters.end();
  for(auto e: params){
    it = setters.find(e.first);
    if(it != setters.end()){
        setters[e.first](*this,e.second);
    }
  }
}

void form::perform_taskstory(string s)
{
  cout << "taskstory" << endl;
  for (auto &e : taskstory)
  {
    cout << e.second << endl;
  }
  cout << endl;
  ftraverser.print_out();
}


state_machine form::test_filler_(){
  //Generation of test cases for development test
  //lt -> 'l'ocal 't'askstory
  auto lt_q1_yes_ = new vector<string>();
    lt_q1_yes_->push_back("task add -hmDMYnd %d %d %d %d %d \"Collect the clothes over home\" \"Gather all the clothes susceptible to be washed and put them in the washer \"");
    lt_q1_yes_->push_back("task del p");
    

  auto q1_branch = map<string,branch>();
    q1_branch["Yes"] = branch{lt_q1_yes_,2};
    q1_branch["No"] =  branch{nullptr, static_cast<int>(UsualStates::exit)};

  state_machine q {
    {1,question_node{1, "Have you washer","No",q1_branch}}
  };
  return q;
}