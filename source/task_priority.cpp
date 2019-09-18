#include "task_priority.h"
#include "cassert"

task_priority::task_priority()
{
    std::ifstream file(priority_definitions_file, std::fstream::in);
    file >> priorities;
}

task_priority::~task_priority()
{

}

bool task_priority::check_priority(const string & mapping_str) const{
    return priorities.find(mapping_str) != priorities.end();
}

task_priority & task_priority::get_priority_lazy_unique_instance(){
    static task_priority priority_;
    return priority_;
}

void priority::set_priority(const string priority_){
    if(task_priority::get_priority_lazy_unique_instance().check_priority(priority_)){
        m_priority = priority_; 
    }else{
        cout << priority_ << " is not valid priority, check api to check is valid" << endl;
        assert(false);
    }
}
