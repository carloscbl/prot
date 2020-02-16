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
#include "persistor.h"
#include "test_prot.h"

using std::vector;
using std::string;
using prot::duration;
namespace task_space{


    struct pair_interval
    {
        time_t end = 0;
        time_t start = 0;
    };


    class task;
    void from_json(const nlohmann::json& ref_json, task_space::task& new_task);
    void from_json(const nlohmann::json& ref_json, task_space::pair_interval& new_interval);
    class task : public CRUD_actionable<task> , public json_serializable<task>
    {
    private:
        string name;
        string description;
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
        friend void task_space::from_json(const nlohmann::json& ref_json, task_space::task& new_task);

        uint64_t id;
        uint64_t user_forms_id = 0;
    public:
        task();
        virtual ~task();

        inline void set_id          (uint64_t id_)         { this->id = id_; }
        inline void set_name        (string name_)         { this->name = name_; }
        inline void set_task_group  (string task_group_)   { this->task_group = task_group_; }
        inline void set_tag         (string tag_)          { this->tag = tag_; }
        inline void set_description (string description_)  { this->description = description_; }
        inline void set_user        (string user_)         { this->m_user = user_; }
        inline void set_user_forms_id(uint64_t uf_id)      { this->user_forms_id = uf_id; }
        inline void set_interval(time_t start, time_t end) {
                                                     this->interval.start = start;
                                                     this->interval.end = end;
        }

        inline uint64_t              get_id()          const noexcept { return id; }
        inline uint64_t              get_user_forms_id()const noexcept{ return user_forms_id; }
        inline const string        & get_name()        const noexcept { return name.empty() ? get_tag() : name;}
        inline const string        & get_tag()         const noexcept { return tag;        }
        inline const string        & get_description() const noexcept { return description; }
        inline const string        & get_m_user()      const noexcept { return m_user;      }
        inline const string        & get_task_group()  const noexcept { return task_group;  }
        inline const pair_interval & get_interval()    const noexcept { return interval; }
        inline const restrictions  & get_restrictions()const noexcept { return m_restrictions; }
        inline const frequency     & get_frequency()   const noexcept { return m_frequency; }
        inline const prot::duration& get_duration()    const noexcept { return m_duration; }
        inline const when          & get_when()        const noexcept { return m_when; }

        void print_remain() const;

        void remain(map<char, string> params) const;
        void print_() { cout << id << ":" << tag << ":" << name << ":" << description << ":" << endl <<ctime(&interval.start) << ctime(&interval.end) << endl; }

    };

    void to_json(nlohmann::json& new_json, const task_space::pair_interval& ref_interval);
    void to_json(nlohmann::json& new_json, const task_space::task& ref_task);

}

using task_t = shared_ptr<task_space::task>;
using time_interval = boost::icl::interval<time_t>;

#endif //TASK_H
