#ifndef TASK_H
#define TASK_H

#include <string>
#include <functional>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <memory>
#include <ctime>
#include <cmath>
#include "CRUD_actionable.h"
#include "json.hpp"
#include <boost/icl/interval.hpp>


namespace task_space{
struct task_status;
class task;
using json = nlohmann::json;
using std::vector;
using std::string;


struct pair_interval
{
    time_t end;
    time_t start;
};
void to_json(json& j, const pair_interval& p);


class task : public CRUD_actionable<task>
{
private:
    string name = "";
    string description;
    string stamp;
    string m_user;
    string task_group;
    time_t dateUTC;
    pair_interval interval;
    CRUD_plus_actions_map tasks_map{

    };

    map_local_functions setters{
        {'n', &task::set_name},
        {'d', &task::set_description},
        {'e', &task::set_user},
        {'D', &task::set_day},    //day
        {'M', &task::set_month},  //month
        {'Y', &task::set_year},   //year
        {'h', &task::set_hour},   //hour
        {'m', &task::set_minute}, //minute*/
        // {"T",&task::set_stamp},//unixTimeStamp
    };
public:
    string id;
    task();
    virtual ~task();
    void set_day(string day);
    void set_month(string mon);
    void set_year(string year);
    void set_hour(string hour);
    void set_minute(string minute);

    void set_name(string name_) { name = name_; }
    void set_description(string description_) { description = description_; }
    void set_stamp(string stamp_) { stamp = stamp_; }
    void set_user(string user_) { m_user = user_; }
    void set_interval(time_t start, time_t end){
        this->interval.start = start;
        this->interval.end = end;
    }
    const string        & get_name()        const noexcept { return name;        }
    const string        & get_description() const noexcept { return description; }
    const string        & get_stamp()       const noexcept { return stamp;       }
    const string        & get_m_user()      const noexcept { return m_user;      }
    const string        & get_task_group()  const noexcept { return task_group;  }
    const time_t        & get_dateUTC()     const noexcept { return dateUTC;     }
    const pair_interval & get_interval()    const noexcept { return interval; }

    void print_remain();

    void remain(map<char, string> params);
    void print_() { cout << id << ":" << name << ":" << description << ":" << asctime(localtime(&dateUTC)) << endl; }

    // void from_json(const json& j, task& p) {
    //     j.at("name").get_to(p.name);
    //     j.at("address").get_to(p.address);
    //     j.at("age").get_to(p.age);
    // }
};
void to_json(json& j, const task& p);

}
using task_t = shared_ptr<task_space::task>;
using time_interval = boost::icl::interval<time_t>;

#endif //TASK_H