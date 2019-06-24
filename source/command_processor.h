//
// Created by carlos on 2/03/19.
//

#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H


#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <functional>
#include <memory>
#include "boost/program_options/parsers.hpp"

#include "iactionable.h"
#include "icommandable.h"
#include "commander.h"


using namespace std;

class command_processor : public commander, public icommandable{
private:
  struct param_op{
    function<void()> action = nullptr;
  };
  using tree_map = map<string,param_op>;
  using subaction_map = map<string,function<void(map<char,string>)>> ;

  map<string,iactionable *> actioners;
  tree_map positional_params {
    {"--help",param_op{
        .action=[](){
          cout <<"\t task | form! | --help(this):\n"
            " \t\ttask -> add|remove|update|list ->(sequential|batched) -n(name)d(description)\n"
            " \t\tEg1: task add -nd Carlos \"Busy creating prot\"\n"
            " \t\tEg2: task update -in task8 Jeorge\n"
            " \t\tEg3: task remove -i task8\n"
            " \t\tEg4: task list"
            << endl;
        }
    }},
  };

  bool params_sanity(vector<string> s,map<char,string> & correspondance);
  void route_positional_param (vector<string> args);

public:
  void perform_command(string s) override ;
  void perform_command(vector<string> s)override;
  void register_actionable(string named_by, iactionable * ac);
};
#endif //COMMAND_PROCESSOR_H
