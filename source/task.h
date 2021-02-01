#ifndef TASK_H
#define TASK_H

#include <string>
#include <functional>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <iostream>
#include <memory>
#include <ctime>
#include <cmath>
#include "duration.h"
#include "when.h"
#include "json.hpp"
#include <boost/icl/interval.hpp>
#include "task_frequency.h"
#include "task_restrictions.h"
#include "persistor.h"
#include "orm_prot.h"
#include "type_conversor.h"



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
    void from_json_auto_task(const nlohmann::json& ref_json, task_space::task& new_task);
    void from_json_user_task(const nlohmann::json& ref_json, task_space::task& new_task);
    void from_json(const nlohmann::json& ref_json, task_space::pair_interval& new_interval);
    class task
    {
    private:
        string name;
        string description;
        string m_user_id;
        string task_group;
        string task_id;
        prot::duration m_duration;
        frequency m_frequency;
        restrictions m_restrictions;
        when m_when;
        pair_interval interval;
        struct designated_period{
            string type_period;
            size_t value;
        };
        optional<designated_period> m_designated_period = nullopt;
        optional<uint64_t> m_session_id = nullopt;

        friend void task_space::from_json(const nlohmann::json& ref_json, task_space::task& new_task);
        friend void task_space::from_json_auto_task(const nlohmann::json& ref_json, task_space::task& new_task);
        friend void task_space::from_json_user_task(const nlohmann::json& ref_json, task_space::task& new_task);

        string id = "";
        uint64_t user_apps_id = 0;
    public:
        nlohmann::json inner_json;
        task();
        virtual ~task();

        inline void set_id           (string id_)            { this->id = id_; }
        inline void set_name         (string name_)            { this->name = name_; }
        inline void set_task_group   (string task_group_)      { this->task_group = task_group_; }
        inline void set_task_id      (string task_id_)         { this->task_id = task_id_; }
        inline void set_description  (string description_)     { this->description = description_; }
        inline void set_user         (string user_)            { this->m_user_id = user_; }
        inline void set_user_apps_id (uint64_t uf_id)          { this->user_apps_id = uf_id; }
        inline void set_json         (nlohmann::json & json_)  { this->inner_json = json_; }
        inline void set_session_id   (optional<uint64_t> session_id)  { this->m_session_id = session_id; }
        inline void set_interval(time_t start, time_t end)     {
                                                     this->interval.start = start;
                                                     this->interval.end = end;
        }

        inline string                get_id()          const noexcept { return id; }
        inline uint64_t              get_user_apps_id()const noexcept{ return user_apps_id; }
        inline const string        & get_name()        const noexcept { return name.empty() ? get_task_id() : name;}
        inline const string        & get_task_id()         const noexcept { return task_id;        }
        inline const string        & get_description() const noexcept { return description; }
        inline const string        & get_m_user()      const noexcept { return m_user_id;      }
        inline const string        & get_task_group()  const noexcept { return task_group;  }
        inline const pair_interval & get_interval()    const noexcept { return interval; }
        inline const restrictions  & get_restrictions()const noexcept { return m_restrictions; }
        inline const frequency     & get_frequency()   const noexcept { return m_frequency; }
        inline const prot::duration& get_duration()    const noexcept { return m_duration; }
        inline const when          & get_when()        const noexcept { return m_when; }
        inline const nlohmann::json& get_json()        const noexcept { return inner_json; }
        inline const optional<designated_period> & get_designated_period() const noexcept { return m_designated_period; }
        inline const optional<uint64_t> & get_session_id() const noexcept { return m_session_id; }


        void print_() { cout << id << ":" << task_id << ":" << name << ":" << description << ":" << endl <<ctime(&interval.start) << ctime(&interval.end) << endl; }

    };

    void to_json(nlohmann::json& new_json, const task_space::pair_interval& ref_interval);
    void to_json(nlohmann::json& new_json, const task_space::task& ref_task);
    using task_t = shared_ptr<task_space::task>;
    optional<std::chrono::system_clock::time_point> next_period_start(const task_t task);
    optional<seconds> get_period_ratio(const task & task);

}

using task_t = shared_ptr<task_space::task>;
using time_interval = boost::icl::interval<time_t>;

#endif //TASK_H
