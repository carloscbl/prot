#include "task_frequency.h"
#include "cassert"


task_frequency & task_frequency::get_frequency_lazy_unique_instance(){
    static task_frequency frequency_;
    return frequency_;
}

void frequency::set_frequency(const string frequency_){
    if(task_frequency::get_frequency_lazy_unique_instance().is_valid(frequency_)){
        m_frequency = frequency_; 
    }else{
        cout << frequency_ << " is not valid frequency, check api to check is valid" << endl;
        assert(false);
    }
}

void frequency::get_json(){
    
}
