#include "task_frequency.h"
#include "cassert"
#include "json.hpp"


task_frequency & task_frequency::get_frequency_lazy_unique_instance(){
    static task_frequency frequency_;
    return frequency_;
}

void frequency::set_frequency(const string frequency_){
    if(task_frequency::get_frequency_lazy_unique_instance().is_valid(frequency_)){
        m_frequency = frequency_; 
        get_json();
    }else{
        cout << frequency_ << " is not valid frequency, check api to check is valid" << endl;
        assert(false);
    }
}

bool frequency::get_json(){
    json j = task_frequency::get_frequency_lazy_unique_instance().api_json;

    auto freq_it = j.find(m_frequency);
    if(freq_it == j.end()){ return false; }

    json freq = freq_it.value();
    json period_j = freq.at("period");
    string str = period_j.begin().key();
    auto match = conversors.find(str);
    if(match == conversors.end()){ return false; }

    this->period = match->second(freq.at("period").at(str).get<int64_t>());
    this->is = *freq.at("is").get<vector<string>>().begin();

    return true;
}
