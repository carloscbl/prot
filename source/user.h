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
#include "test_prot.h"
#include "json.hpp"

using nlohmann::json;
using namespace std;

struct user_minimal_data
{
    string username;
};
/*
    User class have one ischeduler and one itasker
    no one else should have one

 */
class user : public json_serializable<user>
{ //substitute by the final class
public://FIX: this should be wrapped
    static inline map<string, shared_ptr<user>> users;
private:
    shared_ptr<tasker> tasker_;
    unique_ptr<scheduler> scheduler_;
    uint64_t id;

    void init();
    friend void from_json(const nlohmann::json& ref_json, user& new_user);

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
    uint64_t get_id() const noexcept { return this->id; }
    void set_id(const uint64_t target_id) noexcept { this->id = target_id; }


    user & get_user(const string & user_) noexcept { return static_cast<user&>(*users[user_]); }
};
void to_json(nlohmann::json& new_json, const user& ref_task);
void from_json(const nlohmann::json& ref_json, user& new_user);

#endif //USER_H