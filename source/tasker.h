#ifndef TASKER_H
#define TASKER_H

#include <set>
#include "itasker.h"
#include "task.h"


class taskstory_commit_RAII;
class tasker;
using task = task_space::task;
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
class tasker : public itasker 
{
private:
    friend taskstory_commit_RAII;
    map<uint64_t, task_t> tasks_active; // TODO Setters and getters
    //The dispenser is a pre_commit group tasker, that should be used for temporal storage of groups,
    // until its complete submission

    /*
    Here comes the consumable tasks_dispenser
    Its difficult to track life time of this groups,
    probably we should track for a life time and then delete the group
    this wont be persisted and only wont be deleted on test commands
    but a long standing request... could dangle this groups
    */ 
    //FIX: TrackLife time of groups and expirate sync with app runner life time
    //Taskstory : Name -> each task have a tag
    map<string, map< string, task_t > > tasks_dispenser;
    const string & m_user;
    void add_to_group(const string & task_tag, task_t && params, const string & group);
    void commit_group_then_delete(const string & group);
    friend void from_json(const nlohmann::json& ref_json, tasker& new_tasker);
public:
    void print_out() override;
    bool empty() const noexcept override;
    size_t size() const noexcept override;
    void commit_single_task(task_t task_active);
    void add_to_group( task_t && params, const string & group);
    //void update_time() override {};
    void clear() override;
    const string & get_name() const noexcept;

    task_t get_task(const uint64_t id ) const override;
    task_t find_task(const string & tag)  const override;
    inline const map<uint64_t, task_t> & get_tasks() const { return this->tasks_active; }
    tasker(const string & user);
    virtual ~tasker(){}
};
void to_json(nlohmann::json& new_json, const tasker& ref_tasker);
void from_json(const nlohmann::json& ref_json, tasker& new_tasker);
/*
Wraps the group functionality of tasker, you are requiered to commit
if not we destroy the group at end of scope
Usage:
When you start to send commands of a taskstory to tasker, you should know your group
So before start to send commands, you create this object in you scope for security
Then you send all the commands,
Then you COMMIT if you are happy with the result
Then you abandon the scope and everything will be untouched in active_tasks
*/
class taskstory_commit_RAII {
public:
    const string & group;
private:
    tasker & tasker_;
    bool commited = false;

public:

    void commit(){
        tasker_.commit_group_then_delete(group);
        commited = true;
    }

    shared_ptr<map<string,task_t>> get_group(){
        const auto & match = tasker_.tasks_dispenser.find(group);
        if(match != tasker_.tasks_dispenser.end()){
            return make_shared<map<string,task_t>>(match->second);
        }else{
            return shared_ptr<map<string,task_t>>();
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