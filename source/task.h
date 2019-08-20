#ifndef TASK_H
#define TASK_H

#include <string>
#include <functional>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <memory>
#include <ctime>
#include <cmath>
#include "CRUD_actionable.h"
#include <boost/icl/interval.hpp>

struct task_status;
class task;
using time_interval = boost::icl::interval<time_t>;
using task_t = shared_ptr<task>;
using namespace std;

struct pair_interval
{
    time_t end;
    time_t start;
};

struct task_status
{
    task_t task_;
    set<string> tags;//This should be replaced by a (hash, radix, suffix) trie
    bool commited;
};

class task : public CRUD_actionable<task> 
{
private:
    string name = "";
    string description;
    string stamp;
    string m_user;
    time_t dateUTC;
    pair_interval interval;

    inline static map<string, task_t> tasker_commited; //TODO: PlaceHolder of real interface tasker manager
    //washerclothes#normal_user washer_start ->
    //inline static trie_map tag_tasks; //TODO: PlaceHolder of real interface tasker manager

    

    map_local_functions setters{
        {'n', &task::set_name},
        {'d', &task::set_description},
        {'e', &task::set_user},
        {'D', &task::set_day},    //day
        {'M', &task::set_month},  //month
        {'Y', &task::set_year},   //year
        {'h', &task::set_hour},   //hour
        {'m', &task::set_minute}, //minute*/
        // {"T",&task::set_stamp},//unixTimeStamp
    };

    CRUD_plus_actions_map tasks_map{
        {"add", [](map<char, string> s) {
             shared_ptr<task> task_ = make_shared<task>();
             task_->add(s);
             task::tasker_commited[task_->id] = move(task_);
         }},
        {"remove", [this](map<char, string> params) {
             auto it = params.end();

             it = params.find('i');
             if (it != params.end())
             {
                 task::tasker_commited.erase(it->second);
             }
             else
             {
                 cout << "please provide 'i' argument";
                 return;
             }
         }},
        {"update", [this](map<char, string> params) {
             auto it = params.end();

             it = params.find('i');
             if (it != params.end())
             {
                 // auto instance =
                 task::tasker_commited[it->second].get()->update(params);
                 //this->update(params, *instance);
             }
             else
             {
                 cout << "please provide 'i' argument";
                 return;
             }
         }},
        {"list", [&](map<char, string> s) {
             if (task::tasker_commited.size() == 0)
                 cout << "Empty list, no task provided" << endl;
             for (auto &&e : task::tasker_commited)
             {
                 //cout << e.first << "-:-" << e.second->id <<endl;
                 e.second->print_();
             }
         }},
        {"remain", [this](map<char, string> s) {
             this->remain(s);
         }}};

public:
    string id;
    string id2  = "taskorroko";
    task();
    virtual ~task();
    void set_day(string day);
    void set_month(string mon);
    void set_year(string year);
    void set_hour(string hour);
    void set_minute(string minute);

    void set_name(string name_) { name = name_; }
    void set_description(string description_) { description = description_; }
    void set_stamp(string stamp_) { stamp = stamp_; }
    void set_user(string user_) { m_user = user_; }
    void set_interval(time_t start, time_t end){
        this->interval.start = start;
        this->interval.end = end;
    }

    void remove(map<char, string> params, task &instance);

    void print_remain();

    void remain(map<char, string> params);
    void print_() { cout << id << ":" << name << ":" << description << ":" << asctime(localtime(&dateUTC)) << endl; }

    inline const pair_interval &get_interval() const noexcept { return this->interval; }
};

#endif //TASK_H