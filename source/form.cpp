#include "form.h"
#include <iostream>
using namespace std;


form::~form()
{
    cout << "form destroyed" << endl;
}


form::form( const json & valid_form ): m_form(valid_form){
    forms[get_form_name()] = make_unique<form>(*this);
}