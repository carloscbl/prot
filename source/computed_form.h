 //
// Created by carlos on 2/03/19.
//

#ifndef COMPUTED_FORM_H
#define COMPUTED_FORM_H

#include <string>
#include <map>
#include <functional>
#include <memory>
#include "form.h"

using namespace std;

class user;
class state_machine_ended;
 
class computed_form{
public:
    const form& parent_form;
    const user& interviewee;
    const state_machine_ended& stmcend;
    computed_form(const form & parent, const user & interviewee, const state_machine_ended & stmcend);
};


#endif //COMPUTED_FORM_H