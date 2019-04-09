#include "form_traverser.h"

branch::branch(vector<string> *local_taskstory, int next_node_id):
  local_taskstory(local_taskstory),next_node_id(next_node_id){}

void form_traverser::run(){

}

void form_traverser::validate_form(){
  for(const auto & questions : state_machine ){
    for(const auto & branch : questions.second.branches){
      if(branch.second.next_node_id == static_cast<int>(UsualStates::exit))
      {
        is_valid_ = true;
        break;
      }
    }
    if(is_valid_)break;
  }
}

void form_traverser::print_out() const{
cout << "is valid : " << is_valid_ << endl;
  for (const auto & e : state_machine)
  {
    for(const auto & ee : *e.second.branches.cbegin()->second.local_taskstory){
      cout << e.second.id << ":" << e.second.Question << ":" << e.second.Answer << ":" << ":" << ee
      << ":" << e.second.branches.cbegin()->second.next_node_id << endl;
    }
  }
}