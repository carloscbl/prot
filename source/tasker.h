#ifndef TASKER_H
#define TASKER_H

#include <set>
#include "itasker.h"
#include "task.h"

using task_t = shared_ptr<task>;

//Vector because of the fast cache access as they need to be updated almost in real time, but CUD operations are comparatively rare
inline vector<weak_ptr<itasker>> taskers_global; //Global reference to all taskers to cycle them updates on caducity
struct task_status
{
    task_t task_;
    set<string> tags;//This should be replaced by a (hash, radix, suffix) trie
    bool commited;
};
/*
Provides the concrete implementation for the management of the tasks of a specific user
 */
class tasker : public itasker , public CRUD_actionable<tasker>
{
private:
    map<string, task_t> tasks_active; // TODO Setters and getters
    map<string,unique_ptr<task> > tasks_dispenser; //Here comes the consumable tasks

    CRUD_plus_actions_map tasker_map{
        {"add", [this](map<char, string> s) {
             shared_ptr<task> task_ = make_shared<task>();
             task_->add(s);
             tasks_active[task_->id] = move(task_);
         }},
        {"remove", [this](map<char, string> params) {
             auto it = params.end();

             it = params.find('i');
             if (it != params.end())
             {
                tasks_active.erase(it->second);
             }
             else
             {
                 cout << "please provide 'i' argument";
                 return;
             }
         }},
        {"update", [this](map<char, string> params) {
             auto it = params.end();

             it = params.find('i');
             if (it != params.end())
             {
                 // auto instance =
                 tasks_active[it->second].get()->update(params);
                 //this->update(params, *instance);
             }
             else
             {
                 cout << "please provide 'i' argument";
                 return;
             }
         }},
        {"list", [&](map<char, string> s) {
             if (tasks_active.size() == 0)
                 cout << "Empty list, no task provided" << endl;
             for (auto &&e : tasks_active)
             {
                 //cout << e.first << "-:-" << e.second->id <<endl;
                 e.second->print_();
             }
         }},
        {"remain", [this](map<char, string> s) {
             this->remain(s);
         }}};

    map_local_functions setters;

public:
    //void update_time() override {};
    void remove(map<char, string> params, task &instance);
    void remain(map<char, string> params);
    task_t get_task(const string & id ) const{ 
        auto match = tasks_active.find(id);
        if(match != tasks_active.end()){
            return match->second;
        }else{
            return task_t();
        }
    }
    tasker();
};

tasker::tasker():CRUD_actionable(tasker_map,setters){}

void tasker::remove(map<char, string> params, task &instance)
{
    auto it = params.end();

    it = params.find('i');
    if (it != params.end())
    {
        tasks_active.erase(it->second);
    }
    else
    {
        cout << "please provide 'i' argument";
        return;
    }
}

void tasker::remain(map<char, string> params)
{
    auto it = params.end();
    it = params.find('i');
    if (it != params.end())
    {
        tasks_active[it->second]->print_remain();
    }
    else
    {
        for (auto &t : tasks_active)
        {
            t.second->print_remain();
        }
    }
}
#endif //TASKER_H