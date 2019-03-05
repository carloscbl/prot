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
#include "iactionable.h"

using namespace std;

class task;
namespace env{
    static map<string,unique_ptr< task>> tasker;
}
class task : public iactionable{
private:
    string name = "";
    string description;
    string stamp;
    string /*user*/ m_user;
    time_t dateUTC;
    using map_f = map<char,function<void(task&,string)> >;
    using subaction_map = map<string,function<void(map<char,string>)>> ;

    map_f setters{
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

    map_f remover;
    map_f updater {};
    subaction_map tasks_map{
      {"add",[](map<char,string>s){
          unique_ptr<task> task_ = make_unique<task>();
          task_->add(s);
          env::tasker[task_->id] = move(task_);
      }
      },
      {"remove",[this](map<char,string>s){
          this->remove(s);
      }},
      {"update",[this](map<char,string> s){
          this->update(s);
      }},
      {"list",[&](map<char,string> s){
          if(env::tasker.size()==0) cout << "Empty list, no task provided" << endl;
          for(auto && e : env::tasker){
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
    task():dateUTC(time(nullptr)){
        static int acummulator = 0;
        id = "task" + to_string(acummulator++);
    }
    virtual ~task(){
      cout << "~destroying " << this->id <<endl;
    }
    void set_day(string day){

        //cout << asctime(localtime(&dateUTC))<< endl;
        tm * tm_localtime = localtime(&dateUTC);
        tm_localtime->tm_mday = stoi(day);
        dateUTC = mktime(tm_localtime);
        //cout << asctime(localtime(&dateUTC))<< endl;
    }
    void set_month(string mon){
        tm * tm_localtime = localtime(&dateUTC);
        tm_localtime->tm_mon = stoi(mon)-1;
        dateUTC = mktime(tm_localtime);
    }
    void set_year(string year){
        tm * tm_localtime = localtime(&dateUTC);
        tm_localtime->tm_year = stoi(year)-1900;
        dateUTC = mktime(tm_localtime);
    }
    void set_hour(string hour){
        tm * tm_localtime = localtime(&dateUTC);
        tm_localtime->tm_hour = stoi(hour);
        dateUTC = mktime(tm_localtime);
    }
    void set_minute(string minute){
        tm * tm_localtime = localtime(&dateUTC);
        tm_localtime->tm_min = stoi(minute);
        dateUTC = mktime(tm_localtime);
    }

    void set_name(string name_){
        name = name_;
    }
    void set_description(string description_){
        description = description_;
    }
    void set_stamp( string stamp_){stamp = stamp_;}
    void set_user(string user_){m_user = user_;}
    void add(map<char,string>params){
        if(params.size() == 0) cout << "nothing done" << endl;
        auto it = setters.end();
        for(auto e: params){
            it = setters.find(e.first);
            if(it != setters.end()){
                setters[e.first](*this,e.second);
            }
        }
        //print_();

    }

        //now we need to interpret every param and dispatch
    void remove(map<char,string>params){
      auto it = params.end();

      it = params.find('i');
      if(it != params.end()){
        env::tasker.erase(it->second);
      }else{
        cout << "please provide 'i' argument";
        return;
      }
    }
    void update(map<char,string>params){
      auto it = params.end();

      it = params.find('i');
      if(it != params.end()){
        auto & t = env::tasker[it->second];

        for(auto e : params){
          auto  et = setters.end();
          et = setters.find(e.first);
          if(et != setters.end()){
            setters[e.first](*t,e.second);
          }
        }
      }else{
        cout << "please provide 'i' argument";
        return;
      }
    }
    void print_remain(){
      double remain_sec = difftime(this->dateUTC ,time(nullptr) );
      //cout << remain_sec << endl;

      double years = remain_sec / (60*60*24*365);
      double days  =  modf(years,&years) * 365;
      double hours = modf(days,&days) * 24;
      double minutes = modf(hours,&hours) * 60;
      double seconds = modf(minutes,&minutes) * 60;// task add -nMDY pipo 0 4 2020

      cout << id << ":" << "remain" << ": " << minutes<< " minutes :" << hours << " hours :"<< (uint32_t)days<< " days :" <<(uint32_t)years<< " years" << endl;
    }

    void remain(map<char,string>params){
      auto it = params.end();
      it = params.find('i');
      if(it != params.end()){
        env::tasker[it->second]->print_remain();
      }else{
        for(auto & t : env::tasker){
          t.second->print_remain();
        }
      }
    }

    void print_(){
        cout << id << ":" << name << ":" << description << ":" << asctime(localtime(&dateUTC)) << endl;
    }

    void send_action(std::string action, std::map<char,std::string> params) override{
      if(tasks_map.find(action) != tasks_map.end()){
        tasks_map[action](params);
      }else{
        cout << "Not provided correct arguments";
      }
    }
};

#endif //TASK_H