#include "tasker.h"



tasker::tasker(const string & user):CRUD_actionable(tasker_map,setters),m_user(user){}

void tasker::add_to_group(const string & task_tag,task_t && params, const string & group){
    params->set_task_group(group);
    tasks_dispenser[group][task_tag] = (move(params));
}

void tasker::add_to_group( task_t && params, const string & group){
    this->add_to_group( params->get_tag(), move(params), group);
}

void tasker::clear(){
    this->tasks_active.clear();
    this->tasks_dispenser.clear();
}

const string & tasker::get_name() const noexcept{
    return this->m_user;
}

void from_json(const nlohmann::json& ref_json, tasker& new_tasker){
    for (const auto &i : ref_json.items())
    {
        const auto & task_ = i.value();
        task_t taskorino = make_shared<task>(task_);
        string id = taskorino->id;
        new_tasker.tasks_active[id] = taskorino;
    }
    
}

void to_json(nlohmann::json& new_json, const tasker& ref_tasker) {
    for_each(ref_tasker.get_tasks().begin(),ref_tasker.get_tasks().end(), [&new_json](pair<string,task_t> pair){
        new_json.push_back(*pair.second);
    });
}

task_t tasker::find_task(const string & tag) const {
    for (auto &&[k,v] : tasks_active)
    {
        if(v->get_tag() == tag){
            return v;
        }
    }
    return nullptr;
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
            task_t task_active = move(task_.second);
            tasks_active[task_active->id] = task_active;
        }
        //And delete
        tasks_dispenser.erase(group);
    }
}

void tasker::print_out(){
    for (auto [k,v] : this->tasks_active)
    {
        cout << k << " : "; 
        v->print_();
    }
}

bool tasker::empty() const noexcept{
    return tasks_active.empty();
};

size_t tasker::size() const noexcept {
    return this->tasks_active.size();
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

