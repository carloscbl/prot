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
  vector<string> l_task = {"task add aa bb","task del p"};

  auto bran = map<string,branch>{
    {"Yes",branch{l_task,2}},
    {"No",branch{l_task, static_cast<int>(UsualStates::exit)}}
  };

  state_machine q {
    {1,question_node{1, "Have you washer","No",bran}}
  };
  return q;
}