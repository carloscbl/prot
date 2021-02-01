#include "task.h"
#include <set>
#include "expanded_taskstory_t.h"
#include "type_conversor.h"


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
    new_json["task_id"]     = ref_task.get_task_id();
    new_json["duration"]    = ref_task.get_duration();
    new_json["restrictions"]= ref_task.get_restrictions().get_restrictions();
    new_json["frequency"]   = ref_task.get_frequency().get_frequency_name();
    new_json["id"]          = ref_task.get_id();
    new_json["session_id"]  = ref_task.get_session_id().value_or(0);

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
    ref_json.at("task_id").get_to(new_task.task_id);
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
    if(ref_json.find("session_id") != ref_json.end()){
        new_task.set_session_id(ref_json.at("session_id").get<uint64_t>());
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
    ref_json.at("task_id").get_to(new_task.task_id);
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
    if(ref_json.find("session_id") != ref_json.end()){
        new_task.set_session_id(ref_json.at("session_id").get<uint64_t>());
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
    // ref_json.at("task_id").get_to(new_task.task_id);
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
    if(ref_json.find("session_id") != ref_json.end()){
        new_task.set_session_id(ref_json.at("session_id").get<uint64_t>());
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

//Computes for a task, when should be its next cicle
optional<std::chrono::system_clock::time_point> task_space::next_period_start(const task_t task){
    // get frequency
    auto period  = task->get_designated_period();
    if(period.has_value()){
        auto dpm = prot::designated_periods_to_ratio.find(period.value().type_period);
        if(dpm !=prot::designated_periods_to_ratio.end()){
            auto ratio_spacing = dpm->second.get_ratio_seconds(1); // this will get: 1 day (1 week) 1 month 1 year 1 hour
            auto next_period_start_of_day = chrono::floor<days>( system_clock::from_time_t(task->get_interval().start) + ratio_spacing );
            return next_period_start_of_day;
        }
        return nullopt;
    }

    //---------------------------
    auto freq = task->get_frequency();
    auto ratio_spacing = freq.get_period();
    auto next_period_start_of_day = chrono::floor<days>( system_clock::from_time_t(task->get_interval().start) + ratio_spacing );
    return next_period_start_of_day;

}

// Just computes the period in seconds
optional<seconds> task_space::get_period_ratio(const task & task){
    // get frequency
    auto period  = task.get_designated_period();
    if(period.has_value()){
        auto dpm = prot::designated_periods_to_ratio.find(period.value().type_period);
        if(dpm !=prot::designated_periods_to_ratio.end()){
            auto ratio_spacing = dpm->second.get_ratio_seconds(1); // this will get: 1 day (1 week) 1 month 1 year 1 hour
            return ratio_spacing;
        }
        return nullopt;
    }

    //---------------------------
    auto freq = task.get_frequency();
    auto ratio_spacing = freq.get_period();
    return ratio_spacing;
}

task_space::task::task()
{
    //Acumulation wont happend here, the id will be provided by the db
}

task_space::task::~task()
{
    //cout << "~destroying " << this->id << endl;
}

