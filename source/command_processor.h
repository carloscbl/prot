//
// Created by carlos on 2/03/19.
//

#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H


#include <string>
#include <vector>
#include <iostream>
#include <ctime>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>
#include <cstring>
#include <functional>
#include <vector>
#include <memory>
#include "boost/program_options/parsers.hpp"
#include "iactionable.h"
#include "icommandable.h"
#include "commander.h"


#define p(X) std::cout <<  X  <<std::endl;

using namespace std;

class command_processor : public commander, public icommandable{
private:
  map<string,iactionable *> actioners;
  

  using subaction_map = map<string,function<void(map<char,string>)>> ;
  struct param_op{
      function<void()> action = nullptr;
  };

  using tree_map = map<string,param_op>;
  tree_map positional_params {
      {"--help",param_op{
          .action=[](){
            p("\t task | form! | --help(this):\n"
              " \t\ttask -> add|remove|update|list ->(sequential|batched) -n(name)d(description)\n"
              " \t\tEg1: task add -nd Carlos \"Busy creating prot\"\n"
              " \t\tEg2: task update -in task8 Jeorge\n"
              " \t\tEg3: task remove -i task8\n"
              " \t\tEg4: task list"
              );
          }
      }},
  };

  bool params_sanity(const vector<string> s,map<char,string> & correspondance){
    if(s.size() <= 0){
      //      p("No params");
      return false;
    }
    if(s[0][0] == '-'){
      int len = s[0].size();


      auto check_correspondance_seq = [&s,&correspondance](){
          char last_mapped = ' ';
          for(auto e : s){
            if(e[0] == '-'){
              //e is param indicator
              last_mapped = e[1];
            }else{
              //e is param
              correspondance[last_mapped] = e;
            }
          }
      };

      auto check_correspondance_bch = [&s,&correspondance]()-> bool {
          //safety check
          if(s[0].size()-1 != s.size() -1){
            return true;
          }
          for (size_t i = 1; i < s.size(); ++i) {
            correspondance[s[0][i]] = s[i];
          }
          return false;
      };

      if(len-2){//Means 0 sequential,
        //p("batched");
        if(check_correspondance_bch()){
          p("Not matching args, be sure your args match provided indicators");
          return true;
        }
        return false;
      }else{
        //p("Sequential");
        check_correspondance_seq();
        return false;
      }

    }else{
      p("Please provide arguments in batches: {-eAh eee 'AAAAParam' hhhh} or sequentially {-e eee -A 'AAA' -h hhh}  ");
      return true;
    }
  }

  //Refactor to handle the calls in the own iactionables
  auto route_positional_param (vector<string> args)
  {
    if(args.size() > 0){
      //Means that at leas we got relative path and a param, lets check if it is valid
      auto it = positional_params.end();
      it = positional_params.find(args[0]);
      if (it != positional_params.end()) {
        //Good! then enroute it!
        if(it->second.action != nullptr){
          it->second.action();
          return;
        }
      }else if(actioners.find(args[0]) != actioners.end()){
        map<char,string> correspondance;
        if(!params_sanity(vector<string>(args.begin()+2,args.end()), correspondance)){
          actioners[args[0]]->send_action(args[1],correspondance);
          return;
        }
      }else{
        p("Wrong Positional param, --help to se options");
      }
    }else{
      p("Please provide a correct positional param, --help to se options");
    }
  };

public:

  void perform_command(string s) override {
    auto result = boost::program_options::split_unix(s);
    route_positional_param(result);
  }
  void perform_command(vector<string> s)override{
    route_positional_param(s);
  }

  void register_actionable(string named_by, iactionable * ac){
    actioners[named_by] = ac;
  }
};

#endif //COMMAND_PROCESSOR_H
