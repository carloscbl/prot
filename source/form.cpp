#include "form.h"
#include <iostream>
using namespace std;


form::~form(){
    cout << "form destroyed" << endl;
}

void form::send_action(std::string action ,map<char,string> params){
      if(form_map.find(action) != form_map.end()){
        form_map[action](params);
      }else{
        cout << "Not provided correct arguments";
      }
  }

