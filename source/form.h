//
// Created by carlos on 2/03/19.
//

#ifndef FORM_H
#define FORM_H

#include <string>
#include <map>
#include <functional>
#include <memory>

#include "iactionable.h"
#include "icommandable.h"
#include "form_traverser.h"

using namespace std;
class form;
namespace env
{
  static map<string, unique_ptr<form>> forms;
}

class form : public iactionable
{

private:
  using subaction_map = map<string, function<void(map<char, string>)>>;
  using map_f = map<char,function<void(form&,string)> >;
  subaction_map form_map{
      {"add", [](map<char, string> s) {
         unique_ptr<form> form_ = make_unique<form>();
         form_->add(s);
         env::forms[form_->id] = move(form_);
       }},
      {"remove", [](map<char, string> s) {}},
      {"update", [](map<char, string> s) {}},
  };

  map_f setters{
      {'n', &form::perform_taskstory},
  };

public:
  string id;
  form();
  virtual ~form();
  void send_action(std::string action, map<char, string> params) override;
  string name = "Washer_family_stationale_per_kg";
  map<string, string> form_statements{
      {"Do you have washer", "no"},
      {"How much people in home", "3"},
      {"Kg of capacity of the washer", "7KG"},
      {"Is winter or autumn?", "yes"},
  };

  map<string, string> taskstory{
      {"a1", "task add -hmDMYnd %d %d %d %d %d \"Collect the clothes over home\" \"Gather all the clothes susceptible to be washed and put them in the washer \""},
      {"a2", "task add -nd \"Put the washer\" \"Turn on the machine\""},
      {"a3", "task add -nd \"Collect the clothes\" \"Hang out the washing \""},
      {"a4", "task add -nd \"Iron the clothes\" \"Take a while with some interesting podcast or netflix an let perfect the clothes for the week \""},
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
  void add(map<char, string> params);
  void perform_taskstory(string s);
};

#endif //FORM_H
