//
// Created by carlos on 2/03/19.
//

#ifndef USER_H
#define USER_H
#include <memory>
#include <string>
#include <map>
#include "tasker.h"
#include "CRUD_actionable.h"
#include "ischeduler.h"
#include "scheduler.h"
#include "form.h"

using namespace std;

struct user_minimal_data
{
    string username;
};
/*
    User class have one ischeduler and one itasker
    no one else should have one

 */
class user : public CRUD_actionable<user>
{ //substitute by the final class
public://FIX: this should be wrapped
    static inline map<string, shared_ptr<user>> users;
private:
    shared_ptr<tasker> tasker_;
    unique_ptr<scheduler> scheduler_;
    CRUD_plus_actions_map user_actions_map{
        {"add", [](map<char, string> s) {
            user_minimal_data md {
                s['u']
            };
            users[s['u']] = make_unique<user>(md);
        }},
        {"remove", [](map<char, string> params) {}},
        {"update", [](map<char, string> params) {}},
        {"list", [](map<char, string> s) {
            for (const auto & [k,v] : users)
            {
                cout << "user: " << k << endl;
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
            task_t task_ = taske->get_task("task0");

            task_->set_interval(stoi(s['s']) ,stoi(s['e']));
            schedul->add_single(move(task_));
            schedul->print_out();
            auto & schedul2 = users["carlos"]->scheduler_;
            schedul2->print_out();
        }}};
    map_local_functions setters{
        //{'P', &form::set_path},
    };
    void init();

public:
    map<string,form*> instaled_forms; // List all the forms for a user
    user_minimal_data minimal_data;
    user(const user_minimal_data &m_data);
    user(); 
    virtual ~user() {}

    inline tasker &get_tasker() const noexcept { return *tasker_; }
    inline scheduler &get_scheduler() const noexcept { return *scheduler_; }
    void clear(){
        this->get_scheduler().clear();
        this->get_tasker().clear();
    }

    const string &get_name() const noexcept { return minimal_data.username; }
    user & get_user(const string & user_) noexcept { return static_cast<user&>(*users[user_]); }
};

#endif //USER_H