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
        {"stamp", p.get_stamp()},
        {"m_user", p.get_m_user()},
        {"task_group", p.get_task_group()},
        {"interval", p.get_interval()},
    };
}



void task_space::from_json(const nlohmann::json& j, task_space::task& p){
        //j.at("name").get_to(p.name);
        p.m_priority.set_priority( j.at("priority").get<std::string>() );
        j.at("description").get_to(p.description);
        j.at("duration").get_to(p.m_duration);
        j.at("when").get_to(p.m_when);
        //Optional
        if(j.find("restrictions") != j.end()){
            p.m_restrictions.set_restrictions( j.at("restrictions") .get<std::set<std::string>>() );
        }
}

task_space::task::task() : CRUD_actionable(this->tasks_map, setters),dateUTC(time(nullptr))
{
    static int acummulator = 0;
    id = "task" + to_string(acummulator++);
}

task_space::task::~task()
{
    cout << "~destroying " << this->id << endl;
}


void task_space::task::print_remain()
{
    double remain_sec = difftime(this->dateUTC, time(nullptr));
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

void task_space::task::set_day(string day)
{
    tm *tm_localtime = localtime(&dateUTC);
    tm_localtime->tm_mday = stoi(day);
    dateUTC = mktime(tm_localtime);
}

void task_space::task::set_month(string mon)
{
    tm *tm_localtime = localtime(&dateUTC);
    tm_localtime->tm_mon = stoi(mon) - 1;
    dateUTC = mktime(tm_localtime);
}

void task_space::task::set_year(string year)
{
    tm *tm_localtime = localtime(&dateUTC);
    tm_localtime->tm_year = stoi(year) - 1900;
    dateUTC = mktime(tm_localtime);
}

void task_space::task::set_hour(string hour)
{
    tm *tm_localtime = localtime(&dateUTC);
    tm_localtime->tm_hour = stoi(hour);
    dateUTC = mktime(tm_localtime);
}

void task_space::task::set_minute(string minute)
{
    tm *tm_localtime = localtime(&dateUTC);
    tm_localtime->tm_min = stoi(minute);
    dateUTC = mktime(tm_localtime);
}