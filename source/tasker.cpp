#include "tasker.h"



tasker::tasker():CRUD_actionable(tasker_map,setters){}

void tasker::add_group(task_t && params, const string & group){
    tasks_dispenser[group].push_back(move(params));
}

task_t tasker::get_task(const string & id ) const{ 
    auto match = tasks_active.find(id);
    if(match != tasks_active.end()){
        return match->second;
    }else{
        return task_t();
    }
}

void tasker::commit_group_then_delete(const string & group){
    const auto & match = tasks_dispenser.find(group);
    if(match != tasks_dispenser.end()){
        //Filling the active tasker
        for(auto & task_ : match->second){
            task_t task_active = move(task_);
            tasks_active[task_active->id] = task_active;
        }
        //And delete
        tasks_dispenser.erase(group);
    }
}

void tasker::remove(params_map_t params, task &instance)
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

void tasker::remain(params_map_t params)
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

