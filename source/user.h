 //
// Created by carlos on 2/03/19.
//

#ifndef USER_H
#define USER_H

#include <string>
#include <map>

using namespace std;

struct user_minimal_data{
    string username;
    string pass;
};
class user{
public:
    user_minimal_data minimal_data;
    user(const user_minimal_data & m_data):minimal_data(m_data){};
};

class default_user: public user{
    user_minimal_data m{
        "lemon3000", "XP>VISTA"
    };
    default_user():user(m){}
};


#endif //USER_H