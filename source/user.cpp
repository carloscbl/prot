#include "user.h"
#include "tasker.h"
user::user(const user_minimal_data &m_data): CRUD_actionable(this->user_actions_map, setters),minimal_data(m_data){
    init();
}

user::user():CRUD_actionable(this->user_actions_map, setters){
    init();
}
void user::init(){
    scheduler_ = make_unique<scheduler>();
    tasker_ = make_shared<tasker>();
    taskers_global[this->minimal_data.username] = tasker_;
}