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
