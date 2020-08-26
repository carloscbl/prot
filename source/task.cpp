#include "task.h"
#include <set>
#include "expanded_taskstory_t.h"


void task_space::to_json(nlohmann::json& new_json, const task_space::pair_interval& ref_interval) {
    new_json = nlohmann::json{
        {"end",     ref_interval.end},
        {"start",   ref_interval.start},
    };
}

void task_space::to_json(nlohmann::json& new_json, const task_space::task& ref_task) {
    new_json = ref_task.get_json();

    new_json["name"]        = ref_task.get_name();
    new_json["description"] = ref_task.get_description();
    new_json["user"]        = ref_task.get_m_user();
    new_json["task_group"]  = ref_task.get_task_group();
    new_json["interval"]    = ref_task.get_interval();
    new_json["tag"]         = ref_task.get_tag();
    new_json["duration"]    = ref_task.get_duration();
    new_json["restrictions"]= ref_task.get_restrictions().get_restrictions();
    new_json["frequency"]   = ref_task.get_frequency().get_frequency_name();
    new_json["id"]          = ref_task.get_id();

    if(!ref_task.get_when().after.empty()){
        new_json["when"] = ref_task.get_when();
    }
}

void task_space::from_json(const nlohmann::json& ref_json, task_space::pair_interval& new_interval){
    ref_json.at("start").get_to(new_interval.start);
    ref_json.at("end").get_to(new_interval.end);
}

void task_space::from_json(const nlohmann::json& ref_json, task_space::task& new_task){
    new_task.inner_json = ref_json;
    new_task.m_frequency.set_frequency( ref_json.at("frequency").get<std::string>() );
    ref_json.at("description").get_to(new_task.description);
    ref_json.at("name").get_to(new_task.name);
    ref_json.at("tag").get_to(new_task.tag);
    ref_json.at("duration").get_to(new_task.m_duration);
    
    //Optional
    if(ref_json.find("restrictions") != ref_json.end()){
        new_task.m_restrictions.set_restrictions( ref_json.at("restrictions") .get<std::set<std::string>>() );
    }
    if(ref_json.find("when") != ref_json.end()){
        ref_json.at("when").get_to(new_task.m_when);
    }
    if(ref_json.find("interval") != ref_json.end()){
        ref_json.at("interval").get_to(new_task.interval);
    }
    if(ref_json.find("id") != ref_json.end()){
        ref_json.at("id").get_to(new_task.id);
    }
    for(const auto & [k,mappings] : prot::designated_periods_to_ratio){
        if(ref_json.find(k) != ref_json.end()){
            new_task.m_designated_period = task::designated_period{
                .type_period = k,
                .value =ref_json.at(k).get<size_t>() - prot::task_expansion::day_period_user_friendly_offset,
            };
        }
    }
}

void task_space::from_json_auto_task(const nlohmann::json& ref_json, task_space::task& new_task){
    new_task.inner_json = ref_json;
    new_task.m_frequency.set_frequency( ref_json.at("frequency").get<std::string>() );
    ref_json.at("description").get_to(new_task.description);
    ref_json.at("name").get_to(new_task.name);
    ref_json.at("tag").get_to(new_task.tag);
    ref_json.at("duration").get_to(new_task.m_duration);
    
    //Optional
    if(ref_json.find("restrictions") != ref_json.end()){
        new_task.m_restrictions.set_restrictions( ref_json.at("restrictions") .get<std::set<std::string>>() );
    }
    if(ref_json.find("when") != ref_json.end()){
        ref_json.at("when").get_to(new_task.m_when);
    }
    if(ref_json.find("interval") != ref_json.end()){
        ref_json.at("interval").get_to(new_task.interval);
    }
    if(ref_json.find("id") != ref_json.end()){
        ref_json.at("id").get_to(new_task.id);
    }
    for(const auto & [k,mappings] : prot::designated_periods_to_ratio){
        if(ref_json.find(k) != ref_json.end()){
            new_task.m_designated_period = task::designated_period{
                .type_period = k,
                .value =ref_json.at(k).get<size_t>() - prot::task_expansion::day_period_user_friendly_offset,
            };
        }
    }
}

void task_space::from_json_user_task(const nlohmann::json& ref_json, task_space::task& new_task){
    new_task.inner_json = ref_json;
    ref_json.at("description").get_to(new_task.description);
    ref_json.at("name").get_to(new_task.name);
    // ref_json.at("tag").get_to(new_task.tag);
    // ref_json.at("duration").get_to(new_task.m_duration);
    
    new_task.m_frequency.set_frequency( "None" );
    //Optional
    if(ref_json.find("restrictions") != ref_json.end()){
        new_task.m_restrictions.set_restrictions( ref_json.at("restrictions") .get<std::set<std::string>>() );
    }
    if(ref_json.find("when") != ref_json.end()){
        ref_json.at("when").get_to(new_task.m_when);
    }
    if(ref_json.find("interval") != ref_json.end()){
        ref_json.at("interval").get_to(new_task.interval);
    }
    if(ref_json.find("id") != ref_json.end()){
        ref_json.at("id").get_to(new_task.id);
    }
    for(const auto & [k,mappings] : prot::designated_periods_to_ratio){
        if(ref_json.find(k) != ref_json.end()){
            new_task.m_designated_period = task::designated_period{
                .type_period = k,
                .value =ref_json.at(k).get<size_t>() - prot::task_expansion::day_period_user_friendly_offset,
            };
        }
    }
}

task_space::task::task() : id(0)
{
    //Acumulation wont happend here, the id will be provided by the db
}

task_space::task::~task()
{
    //cout << "~destroying " << this->id << endl;
}

