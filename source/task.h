#ifndef TASK_H
#define TASK_H


#include <string>
#include <functional>
#include <vector>
#include <map>
#include <iostream>
#include <memory>
#include <ctime>
#include <cmath>
#include "CRUD_actionable.h"
#include "ischeduler.h"

using namespace std;


class task : public CRUD_actionable<task>{
private:
  string name = "";
  string description;
  string stamp;
  string m_user;
  time_t dateUTC;
  inline static map<string,unique_ptr< task>> tasker;

  unique_ptr<ischeduler> scheduler;

  map_local_functions setters{
      {'n',&task::set_name},
      {'d',&task::set_description},
      {'e',&task::set_user},
      {'D',&task::set_day},//day
      {'M',&task::set_month},//month
      {'Y',&task::set_year},//year
      {'h',&task::set_hour},//hour
      {'m',&task::set_minute},//minute*/
      // {"T",&task::set_stamp},//unixTimeStamp
  };

  CRUD_plus_actions_map tasks_map{
    {"add",[](map<char,string>s){
        unique_ptr<task> task_ = make_unique<task>();
        task_->add(s);
        task::tasker[task_->id] = move(task_);
    }
    },
    {"remove",[this](map<char,string>params){
      auto it = params.end();

      it = params.find('i');
      if(it != params.end()){
        task::tasker.erase(it->second);
      }else{
        cout << "please provide 'i' argument";
        return;
      }
    }},
    {"update",[this](map<char,string> params){
        
      auto it = params.end();

      it = params.find('i');
      if(it != params.end()){
        auto instance = task::tasker[it->second].get();
        this->update(params, *instance);
      }else{
        cout << "please provide 'i' argument";
        return;
      }
    }},
    {"list",[&](map<char,string> s){
        if(task::tasker.size()==0) cout << "Empty list, no task provided" << endl;
        for(auto && e : task::tasker){
          //cout << e.first << "-:-" << e.second->id <<endl;
          e.second->print_();
        }
    }},
    {"remain",[this](map<char,string> s){
        this->remain(s);
    }}
  };
public:
    string id;
    task();
    virtual ~task();
    void set_day(string day);
    void set_month(string mon);
    void set_year(string year);
    void set_hour(string hour);
    void set_minute(string minute);

    void set_name(string name_){name = name_;}
    void set_description(string description_){description = description_;}
    void set_stamp( string stamp_){stamp = stamp_;}
    void set_user(string user_){m_user = user_;}

    void remove(map<char,string>params, task & instance);

    void print_remain();

    void remain(map<char,string>params);
    void print_(){cout << id << ":" << name << ":" << description << ":" << asctime(localtime(&dateUTC)) << endl;}

    void send_action(std::string action, std::map<char,std::string> params) override;
};

#endif //TASK_H