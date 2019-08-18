//
// Created by carlos on 2/03/19.
//

#ifndef USER_H
#define USER_H
#include <memory>
#include <string>
#include <map>
#include "iuser.h"
#include "itasker.h"
#include "tasker.h"
#include "CRUD_actionable.h"
#include "ischeduler.h"
#include "scheduler.h"

using namespace std;

struct user_minimal_data
{
    string username;
    string pass;
};
/*
    User class have one ischeduler and one itasker
    no one else should have one

 */
class user : public CRUD_actionable<user>, public iuser
{ //substitute by the final class
private:
    static inline map<string, unique_ptr<user>> users;
    unique_ptr<tasker> tasker_;
    unique_ptr<ischeduler> scheduler_;
    CRUD_plus_actions_map user_actions_map{
        {"add", [](map<char, string> s) {
            user_minimal_data md {
                s['u'] , s['p']
            };
            users[s['u']] = make_unique<user>(md);
        }},
        {"remove", [](map<char, string> params) {}},
        {"update", [](map<char, string> params) {}},
        {"list", [](map<char, string> s) {
            for (const auto & [k,v] : users)
            {
                cout << "user: " << k << " pass:" << v->minimal_data.pass << endl;
            }
        }},
        {"listsch", [](map<char, string> s) {
            users["carlos"]->scheduler_->print_out();
        }},
        {"getrange", [](map<char, string> s) {
            auto & schedul = users["carlos"]->scheduler_;
            auto vec_opt = schedul->get_range(stoi(s['s']),stoi(s['e']));
            if (!vec_opt.has_value())
            {
                cout << "Empty range" << endl;
                return;
            }
            for_each(vec_opt.value().cbegin(),vec_opt.value().cend(), [](const task_t & task_){
                cout << task_->id << endl;
            });
            
        }},
        {"sch", [](map<char, string> s) {
            auto & schedul = users["carlos"]->scheduler_;
            auto & taske = users["carlos"]->tasker_;
            auto t = taske->tasks.find("task1");

            shared_ptr<task> task_;// = make_shared<task>();

            if (t != taske->tasks.end())
            {
                task_ = t->second;
            }else{
                task_ = make_shared<task>();
                //taske->tasks["task1"] = task_;
            }
            
            task_->set_interval(stoi(s['s']) ,stoi(s['e']));
            schedul->add_single(move(task_));
            schedul->print_out();
        }}};
    map_local_functions setters{
        //{'P', &form::set_path},
    };
    void init();

public:
    user_minimal_data minimal_data;
    user(const user_minimal_data &m_data);
    user(); 
    virtual ~user() {}

    tasker &get_tasker() const noexcept override { return *tasker_; }
    ischeduler &get_scheduler() const noexcept override { return *scheduler_; }

    const string &get_name() const noexcept override { return minimal_data.username; }
};

user::user(const user_minimal_data &m_data): CRUD_actionable(this->user_actions_map, setters),minimal_data(m_data){
    init();
}

user::user():CRUD_actionable(this->user_actions_map, setters){
    init();
}
void user::init(){
    scheduler_ = make_unique<scheduler>();
    tasker_ = make_unique<tasker>();
}

#endif //USER_H