//
// Created by carlos on 2/03/19.
//

#ifndef CRUD_ACTIONABLE_H
#define CRUD_ACTIONABLE_H
#include <iostream>
#include <any>
#include "iactionable.h"
#include "persistor.h"

using namespace std;

template <typename T>
class CRUD_actionable : public iactionable
{
protected:
    using action_map = map<char, string>;
    using CRUD_plus_actions_map = map<string, function<void(map<char, string>)>>;
    using map_local_functions = map<char, function<void(T &, string)>>;
    CRUD_plus_actions_map &actions_map;
    map_local_functions &setters;

public:
    CRUD_actionable(CRUD_plus_actions_map &crud, map_local_functions &lf) : actions_map(crud), setters(lf) {}
    virtual ~CRUD_actionable() = default;
    void add(map<char, string> params)
    {
        if (params.size() == 0)
        {
            cout << "nothing done" << endl;
            return;
        }
        auto it = setters.end();
        for (auto e : params)
        {
            it = setters.find(e.first);
            if (it != setters.end())
            {
                setters[e.first]((*(T *)this), e.second);
            }
        }
    };
    virtual void remove(map<char, string> params, T &instance){};
    void update(map<char, string> params)
    {
        for (auto e : params)
        {
            auto et = setters.end();
            et = setters.find(e.first);
            if (et != setters.end())
            {
                setters[e.first]((*(T *)this), e.second);
            }
        }
    };

    void send_action(std::string action, std::map<char, std::string> params)
    {
        if (actions_map.find(action) != actions_map.end())
        {
            actions_map[action](params);
        }
        else
        {
            cout << action << " :Does not match. Not provided correct arguments" << endl;
        }
    }

};

template<typename T>
class json_serializable
{
public:
    json_serializable() = default;
    ~json_serializable()= default;
    virtual const string & get_name() const noexcept = 0;
    void save(){
        auto & current_persistor = persistor::get_persistor_instance();
        T * downcast_this = static_cast<T*>(this);
        auto name =  downcast_this->get_name();
        if(name.empty()){
            cout << "Cannot store a file with no name" << endl;
            return;
        }
        current_persistor.save(name +".json" , json(*downcast_this) );
    }
    void load(const string & file_name){
        auto & current_persistor = persistor::get_persistor_instance();
        T * downcast_this = static_cast<T*>(this);
        if(file_name.empty()){
            cout << "Cannot store a file with no name" << endl;
            return;
        }
        json j;
        current_persistor.load(file_name+".json" , j );
        from_json(j,*downcast_this);
    }
};



#endif //CRUD_ACTIONABLE_H