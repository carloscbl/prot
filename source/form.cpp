#include "form.h"
#include <iostream>
#include "type_container.h"
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

void test_type_container(){
  type_container<int32_t>("INTEGER",32);
}

state_machine form::test_filler_(){
  //Generation of test cases for development test
  //lt -> 'l'ocal 't'askstory
  auto lt_q1_yes_ = new vector<string>();
    lt_q1_yes_->push_back("task add -hmDMYnd %d %d %d %d %d \"Collect the clothes over home\" \"Gather all the clothes susceptible to be washed and put them in the washer \"");
    lt_q1_yes_->push_back("task del p");
    vector<string> lt_q2{"More days variable"};

  map<string,branch>  q1_branch, q2_branch;
  //q1_branch.insert(make_pair("", branch{nullptr, static_cast<int>(UsualStates::self)}));
  q1_branch.insert(make_pair("yes", branch{lt_q1_yes_,2}));
  q1_branch.insert(make_pair("no", branch{nullptr, static_cast<int>(UsualStates::exit)}));

  

  state_machine q {
    {1,question_node{1, "Do you have washer","No",q1_branch}},
    {2,question_node{2, "How much kg it have","No",q2_branch}},
  };
  return q;
}

