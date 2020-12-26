#pragma once

#include "scheduler.h"


namespace prot::scheduler::find_functions{

inline bool first_tasks_optional_designated(prot::scheduler::find_params f_p, task_t tsk){
    if(tsk->get_task_id() == f_p.task_id){
        if(f_p.designated_period.has_value() 
        && tsk->get_designated_period().has_value() 
        && f_p.designated_period.value() == tsk->get_designated_period().value().value){
            return true;
        }
    }
    return false;
}

}
