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
#include "duration.h"
#include "when.h"
#include "json.hpp"
#include <boost/icl/interval.hpp>
#include "task_frequency.h"
#include "task_restrictions.h"


using std::vector;
using std::string;
using prot::duration;
namespace task_space{


    struct pair_interval
    {
        time_t end;
        time_t start;
    };


    class task;
    void from_json(const nlohmann::json& j, task_space::task& p);
    class task : public CRUD_actionable<task>
    {
    private:
        string name;
        string description;
        string stamp;
        string m_user;
        string task_group;
        string tag;
        prot::duration m_duration;
        frequency m_frequency;
        restrictions m_restrictions;
        when m_when;
        pair_interval interval;
        
        CRUD_plus_actions_map tasks_map;

        map_local_functions setters{
            {'n', &task::set_name},
            {'d', &task::set_description},
            {'u', &task::set_user},
        };
        friend void task_space::from_json(const nlohmann::json& j, task_space::task& p);

    public:
        string id;
        task();
        virtual ~task();

        void set_name        (string name_)         { this->name = name_; }
        void set_task_group  (string task_group_)   { this->task_group = task_group_; }
        void set_tag         (string tag_)          { this->tag = tag_; }
        void set_description (string description_)  { this->description = description_; }
        void set_stamp       (string stamp_)        { this->stamp = stamp_;}
        void set_user        (string user_)         { this->m_user = user_; }
        void set_interval(time_t start, time_t end) {
                                                     this->interval.start = start;
                                                     this->interval.end = end;
                                                     //print_();
        }

        const string        & get_name()        const noexcept { return name;        }
        const string        & get_tag()         const noexcept { return tag;        }
        const string        & get_description() const noexcept { return description; }
        const string        & get_stamp()       const noexcept { return stamp;       }
        const string        & get_m_user()      const noexcept { return m_user;      }
        const string        & get_task_group()  const noexcept { return task_group;  }
        const pair_interval & get_interval()    const noexcept { return interval; }
        const restrictions  & get_restrictions()const noexcept { return m_restrictions; }
        const frequency     & get_frequency()   const noexcept { return m_frequency; }
        const prot::duration& get_duration()    const noexcept { return m_duration; }
        const when          & get_when()        const noexcept { return m_when; }

        void print_remain() const;

        void remain(map<char, string> params) const;
        void print_() { cout << id << ":" << tag << ":" << name << ":" << description << ":" << endl <<ctime(&interval.start) << ctime(&interval.end) << endl; }

    };

    void to_json(nlohmann::json& j, const pair_interval& p);
    void to_json(nlohmann::json& j, const task_space::task& p);

}

using task_t = shared_ptr<task_space::task>;
using time_interval = boost::icl::interval<time_t>;

#endif //TASK_H
