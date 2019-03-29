#include "form_traverser.h"


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