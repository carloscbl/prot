 //
// Created by carlos on 2/03/19.
//

#ifndef USER_H
#define USER_H

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
template<typename T_user,typename T_tasker,typename T_scheduler>
class user : public iuser{//substitute by the final class
private:
    // string unique_id; //For now to check existance and to bind task data to someone
    // #string pass;
    itasker<T_tasker> tasker;
    ischeduler<T_scheduler> scheduler;
public:
    user_minimal_data minimal_data;
    user(const user_minimal_data & m_data):minimal_data(m_data){};
};


#endif //USER_H