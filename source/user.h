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
#include "CRUD_actionable.h"
#include "ischeduler.h"

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
class user : public CRUD_actionable<task>, public iuser
{ //substitute by the final class
private:
    static inline map<string, unique_ptr<user>> users;
    itasker *tasker;
    ischeduler *scheduler;
    CRUD_plus_actions_map user_actions_map{
        {"add", [](map<char, string> s) {}},
        {"remove", [](map<char, string> params) {}},
        {"update", [](map<char, string> params) {}},
        {"list", [](map<char, string> s) {}},
        {"remain", [](map<char, string> s) {}}};
    map_local_functions setters{
        //{'P', &form::set_path},
    };

public:
    user_minimal_data minimal_data;
    user(const user_minimal_data &m_data) : CRUD_actionable(this->user_actions_map, setters), minimal_data(m_data){};
    virtual ~user() {}

    const itasker &get_tasker() const noexcept override { return *tasker; }
    const ischeduler &get_scheduler() const noexcept override { return *scheduler; }

    const string &get_name() const noexcept override { return minimal_data.username; }
    static user test_user();

    void send_action(std::string action, std::map<char, std::string> params) override;
};

void user::send_action(std::string action, std::map<char, std::string> params)
{
    if (tasks_map.find(action) != tasks_map.end())
    {
        tasks_map[action](params);
    }
    else
    {
        cout << action << " :Does not match. Not provided correct arguments" << endl;
    }
}

user user::test_user()
{
    user_minimal_data md{
        "test_user", "123456"};
    return user(md);
}

#endif //USER_H