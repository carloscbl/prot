//
// Created by carlos on 2/03/19.
//

#ifndef USER_H
#define USER_H
#include <memory>
#include <string>
#include <map>
#include "tasker.h"
#include "ischeduler.h"
#include "scheduler.h"
#include "app.h"
#include "orm_prot.h"
#include "json.hpp"

using nlohmann::json;
using namespace std;

struct user_minimal_data
{
    string username;
};

class user 
{
private:
    shared_ptr<tasker> tasker_;
    unique_ptr<scheduler> scheduler_;
    string id;
    struct user_conf{
        int backwards_projection = 1;
        int forward_projection = 1;
    };

    void init();
    friend void from_json(const nlohmann::json& ref_json, user& new_user);

public:
    user_conf conf;
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
    friend void to_json(nlohmann::json& new_json, const user& ref_task);
    friend void from_json(const nlohmann::json& ref_json, user& new_user);

    const string &get_name() const noexcept { return minimal_data.username; }
    string get_id() const noexcept { return this->id; }
    void set_id(const string target_id) noexcept { this->id = target_id; }

};
void to_json(nlohmann::json& new_json, const user& ref_task);
void from_json(const nlohmann::json& ref_json, user& new_user);

#endif //USER_H