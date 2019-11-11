#include "task.h"
#include <set>


void task_space::to_json(nlohmann::json& j, const pair_interval& p) {
    j = nlohmann::json{
        {"end", p.end},
        {"start", p.start},
    };
}

void task_space::to_json(nlohmann::json& j, const task_space::task& p) {
    j = nlohmann::json{
        {"name", p.get_name()},
        {"description", p.get_description()},
        {"user", p.get_m_user()},
        {"task_group", p.get_task_group()},
        {"interval", p.get_interval()},
        {"tag", p.get_tag()},
        {"duration", p.get_duration()},
        {"restrictions", p.get_restrictions().get_restrictions()},
        {"frequency", p.get_frequency().get_frequency_name()},
        {"when", p.get_when()},
    };
}

void task_space::from_json(const nlohmann::json& j, task_space::task& p){

    p.m_frequency.set_frequency( j.at("frequency").get<std::string>() );
    j.at("description").get_to(p.description);
    j.at("name").get_to(p.name);
    j.at("tag").get_to(p.tag);
    //
    j.at("duration").get_to(p.m_duration);
    
    //Optional
    if(j.find("restrictions") != j.end()){
        p.m_restrictions.set_restrictions( j.at("restrictions") .get<std::set<std::string>>() );
    }
    if(j.find("when") != j.end()){
        j.at("when").get_to(p.m_when);
    }
}

task_space::task::task() : CRUD_actionable(this->tasks_map, setters)
{
    static int acummulator = 0;
    id = "task" + to_string(acummulator++);
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
