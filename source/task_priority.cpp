#include "task_priority.h"
#include "cassert"


task_priority & task_priority::get_priority_lazy_unique_instance(){
    static task_priority priority_;
    return priority_;
}

void priority::set_priority(const string priority_){
    if(task_priority::get_priority_lazy_unique_instance().is_valid(priority_)){
        m_priority = priority_; 
    }else{
        cout << priority_ << " is not valid priority, check api to check is valid" << endl;
        assert(false);
    }
}
