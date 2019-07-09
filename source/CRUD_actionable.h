 //
// Created by carlos on 2/03/19.
//

#ifndef CRUD_ACTIONABLE_H
#define CRUD_ACTIONABLE_H
#include <iostream>
#include "iactionable.h"

using namespace std;

template<typename T>
class CRUD_actionable : public iactionable{
protected:
    using CRUD_plus_actions_map = map<string,function<void(map<char,string>)>> ;
    using map_local_functions = map<char,function<void(T &,string)> >;

    CRUD_plus_actions_map & tasks_map;
    map_local_functions & setters;
public:
    CRUD_actionable(CRUD_plus_actions_map & crud, map_local_functions & lf):tasks_map(crud),setters(lf){}
    //CRUD_actionable() = delete;
    void add(map<char,string>params){
        if(params.size() == 0){
            cout << "nothing done" << endl;
            return;
        } 
        auto it = setters.end();
        for(auto e: params){
            it = setters.find(e.first);
            if(it != setters.end()){
                setters[e.first]((*(T*)this),e.second);
            }
        }
    };
    virtual void remove(map<char,string>params, T & instance);//{ throw exception; };
    virtual void update(map<char,string>params, T & instance){
        for(auto e : params){
            auto  et = setters.end();
            et = setters.find(e.first);
            if(et != setters.end()){
                setters[e.first](instance,e.second);
            }
        }
    };
};

#endif //CRUD_ACTIONABLE_H