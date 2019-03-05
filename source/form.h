//
// Created by carlos on 2/03/19.
//

#ifndef FORM_H
#define FORM_H

#include <string>
#include <map>
#include "iactionable.h"
#include "icommandable.h"

using namespace std;

class form : public iactionable{

private:
  using subaction_map = map<string,function<void(map<char,string>)>> ;
  subaction_map form_map{
        {"adds",[](map<char,string> s){}},
        {"remove",[](map<char,string> s){}},
        {"update",[](map<char,string> s){}},
    };

public:
  virtual ~form(){};
  void send_action(std::string action ,map<char,string> params) override{
      if(form_map.find(action) != form_map.end()){
        form_map[action](params);
      }else{
        cout << "Not provided correct arguments";
      }
  }
  string name = "Washer_family_stationale_per_kg";
  map<string,string> form_statements{
    {"Do you have washer","no"},
    {"How much people in home","3"},
    {"Kg of capacity of the washer","7KG"},
    {"Is winter or autumn?","yes"},
    };

  map<string, string>taskstory{
      {"","task add -nd \"Collect the clothes over home\" \"Gather all the clothes susceptible to be washed and put them in the washer \""},
      {"","task add -nd \"Put the washer\" \"Turn on the machine\""},
      {"","task add -nd \"Collect the clothes\" \"Hang out the washing \""},
      {"","task add -nd \"Iron the clothes\" \"Take a while with some interesting podcast or netflix an let perfect the clothes for the week \""},
  };
  //Setear las task cuando se llama al formulario hay que setear todas las task a un usuario
  //Si no hay usuario asumimos que solo existe 1 usuario
  //entonces si relanzamos el formulario, debemos de impedir que se relance el seteo de task redundantes pero si actualizar las existentes
  //hay que hacer un match o identificacion de tasks con el form
  //mas adelante hacerlo con inferencia inteligente basada en tags o campos o quizas no haga falta

  //Computar variables del form sobre las tasks

  //1 Ejecutar las task
  //2 Ejecutar sin que pisen las existentes ni se dupliquen
  //3 Ejecutar con computo de variables
  //4 Hacer sencillo el pipeline de inputs

  void perform_taskstory(){
    for(auto & e  : taskstory){
      //
    }
  }




};



#endif //FORM_H
