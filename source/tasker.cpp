#include "tasker.h"
#include "db_ops.hpp"
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

tasker::tasker(const string & user):m_user(user){}

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
        auto id = taskorino->get_id();
        new_tasker.tasks_active[id] = taskorino;
    }
    
}

void to_json(nlohmann::json& new_json, const tasker& ref_tasker) {
    for_each(ref_tasker.get_tasks().begin(),ref_tasker.get_tasks().end(), [&new_json](const pair<uint64_t,task_t>& pair){
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

task_t tasker::get_task(const uint64_t id ) const{ 
    auto match = tasks_active.find(id);
    if(match != tasks_active.end()){
        return match->second;
    }else{
        return task_t();
    }
}

void tasker::commit_single_task(task_t task_active){
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
        
    task_active->inner_json["prot_id"] = boost::uuids::to_string(uuid);
    try
    {
        if(create_task({{this->m_user,false}},*task_active)){
            // get expire date
            json job;
            job["type"] = "task_clone_into_next_period";
            job["task_id"] = task_active->get_id(); // If at the moment of job execution it doesn't exists wont execute the clone
            job["start_job_at"] =  system_clock::to_time_t( std::chrono::floor<days>(system_clock::from_time_t( task_active->get_interval().start)) - days(1));
            auto next_period_start_day = task_space::next_period_start(task_active);
            if(next_period_start_day.has_value()){
                job["next_period_start"] = system_clock::to_time_t( next_period_start_day.value() );
                create_prot_jobs(job); // it holds next day to schedule the clone task
            }
            
        }
        tasks_active[task_active->get_id()] = task_active; // wont work with create_task step
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void tasker::commit_group_then_delete(const string & group){
    const auto & match = tasks_dispenser.find(group);
    if(match != tasks_dispenser.end()){
        //Filling the active tasker
        for(auto & task_ : match->second){
            task_t task_active = move(task_.second);
            commit_single_task(task_active);
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
