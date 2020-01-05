#include "task.h"
#include <set>


void task_space::to_json(nlohmann::json& new_json, const task_space::pair_interval& ref_interval) {
    new_json = nlohmann::json{
        {"end",     ref_interval.end},
        {"start",   ref_interval.start},
    };
}

void task_space::to_json(nlohmann::json& new_json, const task_space::task& ref_task) {
    new_json = nlohmann::json{
        {"name",        ref_task.get_name()},
        {"description", ref_task.get_description()},
        {"user",        ref_task.get_m_user()},
        {"task_group",  ref_task.get_task_group()},
        {"interval",    ref_task.get_interval()},
        {"tag",         ref_task.get_tag()},
        {"duration",    ref_task.get_duration()},
        {"restrictions",ref_task.get_restrictions().get_restrictions()},
        {"frequency",   ref_task.get_frequency().get_frequency_name()},
        {"id",          ref_task.get_id()},
    };
    if(!ref_task.get_when().after.empty()){
        new_json["when"] = ref_task.get_when();
    }
}

void task_space::from_json(const nlohmann::json& ref_json, task_space::pair_interval& new_interval){
    ref_json.at("start").get_to(new_interval.start);
    ref_json.at("end").get_to(new_interval.end);
}

void task_space::from_json(const nlohmann::json& ref_json, task_space::task& new_task){
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
}

task_space::task::task() : CRUD_actionable(this->tasks_map, setters),id(0)
{
    //Acumulation wont happend here, the id will be provided by the db
}

task_space::task::~task()
{
    //cout << "~destroying " << this->id << endl;
}


void task_space::task::print_remain() const
{
    double remain_sec = difftime(this->interval.start, time(nullptr));
    //cout << remain_sec << endl;

    double years = remain_sec / (60 * 60 * 24 * 365);
    double days = modf(years, &years) * 365;
    double hours = modf(days, &days) * 24;
    double minutes = modf(hours, &hours) * 60;
    modf(minutes, &minutes); // * 60;// task add -nMDY pipo 0 4 2020

    cout << id << ":"
         << "remain"
         << ": " << minutes << " minutes :" << hours << " hours :" << (uint32_t)days << " days :" << (uint32_t)years << " years" << endl;
}
