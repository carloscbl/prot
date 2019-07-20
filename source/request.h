#ifndef REQUEST_H
#define REQUEST_H
#include "CRUD_actionable.h"
#include "user.h"
#include "iuser.h"
#include "form.h"
#include "form_runner.h"

class request : public CRUD_actionable<request>
{
private:
    CRUD_plus_actions_map request_map{
        {"add",    [](map<char, string> s) {}},
        {"remove", [](map<char, string> s) {}},
        {"update", [](map<char, string> s) {}},
        {"test", [this](map<char, string> s) {test();}},
        {"list", [](map<char, string> s) {}},
    };

  //Modulator params like -h -n -P
  map_local_functions setters{ 
    //{'t', &request::test},
  };
public:
    request(/* args */);
    ~request();

    void send_action(std::string action, map<char, string> params) override;
    void test();
};

request::request():CRUD_actionable(this->request_map,setters)
{
}

request::~request()
{
}

void request::test(){
    user_minimal_data md{
        "Carlos","123456"
    };
    user user_(md);

    auto aa = form::forms.cbegin();

    //form_runner fr (user_,  );

    cout << aa->second->name << endl;
}

void request::send_action(std::string action, std::map<char,std::string> params){
    if(request_map.find(action) != request_map.end()){
        request_map[action](params);
    }else{
        cout << action << " :Does not match. Not provided correct arguments" <<endl;
    }
}


#endif //REQUEST_H
