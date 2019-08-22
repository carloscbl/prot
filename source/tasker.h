#ifndef TASKER_H
#define TASKER_H

#include <set>
#include "itasker.h"
#include "task.h"

class taskstory_commit_RAII;
class tasker;
using task_t = shared_ptr<task>;
using params_map_t = map<char, string>;
//Vector because of the fast cache access as they need to be updated almost in real time, but CUD operations are comparatively rare
inline map<string,weak_ptr<tasker>> taskers_global; //Global reference to all taskers to cycle them updates on caducity
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
    friend taskstory_commit_RAII;
    map<string, task_t> tasks_active; // TODO Setters and getters
    //The dispenser is a pre_commit group tasker, that should be used for temporal storage of groups,
    // until its complete submission

    /*
    Here comes the consumable tasks_dispenser
    Its difficult to track life time of this groups,
    probably we should track for a life time and then delete the group
    this wont be persisted and only wont be deleted on test commands
    but a long standing request... could dangle this groups
    */ 
    //FIX: TrackLife time of groups and expirate sync with form runner life time
    map<string, vector< task_t > > tasks_dispenser;

    CRUD_plus_actions_map tasker_map{
        {"add", [&](params_map_t s) {
            task_t task_ = make_shared<task>();
            auto tasker_ = taskers_global["carlos"].lock();

            auto match = s.find('g');
            task_->add(s);
            if (match != s.end()){
                //Got groups, they aren't directly committed
                ((tasker&)*tasker_).add_group(move(task_),match->second);
            }else{
                ((tasker&)*tasker_).tasks_active[task_->id] = move(task_);
            }
        }},
        {"remove", [this](params_map_t params) {
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
        {"update", [this](params_map_t params) {
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
        {"list", [&](params_map_t s) {
            if (tasks_active.size() == 0)
                cout << "Empty list, no task provided" << endl;
            for (auto &&e : tasks_active)
            {
                //cout << e.first << "-:-" << e.second->id <<endl;
                e.second->print_();
            }
        }},
        {"remain", [this](params_map_t s) {
            this->remain(s);
        }},
        {"listdispatcher", [this](params_map_t s) {
            for(auto & [k,v] : this->tasks_dispenser){
                cout << k << ":" << endl;
                for(auto & task : v){
                    task->print_();
                }
            }
        }},
        };

    map_local_functions setters;
    void add_group(task_t && params, const string & group);
    void commit_group_then_delete(const string & group);

public:
    //void update_time() override {};
    void remove(params_map_t params, task &instance);
    void remain(params_map_t params);
    task_t get_task(const string & id ) const override;
    tasker();
};

/*
Wraps the group functionality of tasker, you are requiered to commit
if not we destroy the group at end of scope
Usage:
When you start to send commands of a taskstory to tasker, you should know your group
So before start to send commands, you create this object in you scope for security
Then you send all the commands,
Then you COMMIT if you are happy with the result
Then you abandon the scope and everything will be in its site in active_tasks
*/
class taskstory_commit_RAII {
private:
    string & group;
    tasker & tasker_;
    bool commited = false;

public:

    void commit(){
        tasker_.commit_group_then_delete(group);
        commited = true;
    }

    shared_ptr<vector<task_t>> get_group(){
        const auto & match = tasker_.tasks_dispenser.find(group);
        if(match != tasker_.tasks_dispenser.end()){
            return make_shared<vector<task_t>>(match->second);
        }else{
            return shared_ptr<vector<task_t>>();
        }
    }

    taskstory_commit_RAII(string & group, tasker & tasker_ )
        :group(group),tasker_(tasker_){}
    ~taskstory_commit_RAII(){
        if(!commited){
            this->tasker_.tasks_dispenser.erase(this->group);
        }
    }
};

#endif //TASKER_H