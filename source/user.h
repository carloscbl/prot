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
#include "ischeduler.h"

using namespace std;

struct user_minimal_data{
    string username;
    string pass;
};

class user : public iuser{//substitute by the final class
private:
    static map<string,weak_ptr<user>> users; 
    
    // string unique_id; //For now to check existance and to bind task data to someone
    // #string pass;
    itasker tasker;
    ischeduler scheduler;
public:
    user_minimal_data minimal_data;
    user(const user_minimal_data & m_data):minimal_data(m_data){};

    const itasker & get_tasker() const noexcept override{ return tasker; }
    const ischeduler & get_scheduler() const noexcept override{ return scheduler; }

    const string & get_name()const noexcept override{return minimal_data.username;}
};

#endif //USER_H