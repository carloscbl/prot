#ifndef TASKER_H
#define TASKER_H

#include <set>
#include "task.h"
#include "unordered_map"


class taskstory_commit_RAII;
class tasker;
class taskstory_commit_batched_raii;
using task = task_space::task;
using task_t = shared_ptr<task>;
using params_map_t = map<char, string>;

using std::unordered_map;
/*
Provides the concrete implementation for the management of the tasks of a specific user
 */
class tasker 
{
private:
    friend taskstory_commit_RAII;
    friend taskstory_commit_batched_raii;
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
    //FIX: TrackLife time of groups and expirate sync with app runner life time
    //Taskstory : Name -> each task have a task_id
    unordered_map<string, unordered_map< string, task_t > > tasks_dispenser; //groups, <tasks_ids, tasks>
    const string & m_user_id;
    void add_to_group(const string & task_id, task_t && params, const string & group);
    void commit_group_then_delete(const string & group);
    void commit_group_then_delete(const vector<string> & groups);
    bool commit_batch(unordered_map<string,task_t> & tasks);
    bool commit_batch(const vector<task_t> & tasks);
    bool group_delete(const string & group);
    friend void from_json(const nlohmann::json& ref_json, tasker& new_tasker);
public:
    void print_out() ;
    bool empty() const noexcept ;
    size_t size() const noexcept ;
    void commit_single_task(task_t task_active);
    void add_to_group( task_t && params, const string & group);
    //void update_time()  {};
    void clear() ;
    const string & get_name() const noexcept;

    task_t get_task(const string id ) const ;
    task_t find_task(const string & task_id)  const ;
    inline const map<string, task_t> & get_tasks() const { return this->tasks_active; }
    tasker(const string & user);
    virtual ~tasker(){}
};

void to_json(nlohmann::json& new_json, const tasker& ref_tasker);
void from_json(const nlohmann::json& ref_json, tasker& new_tasker);

class taskstory_commit_batched_raii
{
private:
    bool commited = false;
public:
    vector<string> groups;
    tasker & tasker_;
    vector<task_t> tasks_batch;
    taskstory_commit_batched_raii(tasker & tasker_):tasker_(tasker_){}
    ~taskstory_commit_batched_raii(){
        if(commited){
            return;
        }

        for (const auto & group : groups)
        {
            this->tasker_.tasks_dispenser.erase( group );
        }
    }

    void commit(){
        tasker_.commit_batch(tasks_batch);       
        // tasker_.commit_group_then_delete(groups);
        commited = true;
    }
};

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
    shared_ptr<taskstory_commit_batched_raii> batched = nullptr;

public:

    void commit(){
        if(!batched){
            tasker_.commit_group_then_delete(group);
        }else{
            store_current_tasks_in(batched->tasks_batch);
            batched->groups.push_back(group);
            tasker_.group_delete(group);
        }
        commited = true;
    }

    shared_ptr<unordered_map<string,task_t>> get_group(){
        const auto & match = tasker_.tasks_dispenser.find(group);
        if(match != tasker_.tasks_dispenser.end()){
            return make_shared<unordered_map<string,task_t>>(match->second);
        }else{
            return shared_ptr<unordered_map<string,task_t>>();
        }
    }

    bool store_current_tasks_in(vector<task_t> & tasks_batch){
        const auto & match = tasker_.tasks_dispenser.find(group);
        if(match != tasker_.tasks_dispenser.end()){
            for (const auto &[k,v] : match->second)
            {
                tasks_batch.push_back(v);
            }
            return true;
        }
        return false;
    }

    taskstory_commit_RAII(string & group, tasker & tasker_, shared_ptr<taskstory_commit_batched_raii> batched = nullptr)
        :group(group),tasker_(tasker_),batched(batched){}
    ~taskstory_commit_RAII(){
        if(!commited && !batched){
            this->tasker_.tasks_dispenser.erase(this->group);
        }
    }
};



#endif //TASKER_H