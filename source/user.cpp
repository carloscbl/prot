#include "user.h"
#include "tasker.h"
user::user(const user_minimal_data &m_data): minimal_data(m_data){
    init();
}

user::user(){
    init();
}
void user::init(){
    scheduler_ = make_unique<scheduler>();
    tasker_ = make_shared<tasker>(this->get_name());
    taskers_global[this->minimal_data.username] = tasker_;
}

void to_json(nlohmann::json& new_json, const user& ref_task){
    new_json = nlohmann::json{
    {"id", ref_task.get_id()},
    {"username",ref_task.get_name()},
    };
}

void from_json(const nlohmann::json& ref_json, user& new_user){
    cout << ref_json.dump(4) << endl;
    ref_json.at("username").get_to(new_user.minimal_data.username);
}