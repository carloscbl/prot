#include "task_restrictions.h"

void restrictions::set_restrictions(const set<string> & restrictions_){
    for (const auto &restr : restrictions_)
    {
        if(task_restrictions::get_restrictions_lazy_unique_instance().is_valid(restr)){
            m_restrictions.insert(restr);
        }else{
            cout << restr << " is a invalid restriction, check restrictions api" << endl;
        }
    }
}

task_restrictions & task_restrictions::get_restrictions_lazy_unique_instance(){
    static task_restrictions restr;
    return restr;
}

vector<json_interval> restrictions::get_all_from_to() const{
    const json & j = task_restrictions::get_restrictions_lazy_unique_instance().api_json;
    vector<json_interval> intervals;
    for (const auto & restriction : m_restrictions){
        auto match = j.find(restriction);
        if(match != j.end()){
            intervals.push_back(json_interval{
                .from = hours(match.value()["from"].get<int>()),
                .to = hours(match.value()["to"].get<int>()),
                .restriction_name = restriction,
            });
        }
    }

    return intervals;
}