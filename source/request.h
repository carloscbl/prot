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
        {"test", [this](map<char, string> s) {
            json j ;
            if(s.size() > 0){
                j["answer"] = s;
            }
            test(j);
        }},
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
    void test(const json qa_request);
};

request::request():CRUD_actionable(this->request_map,setters)
{
}

request::~request()
{
}

void request::test(const json qa_request){//Wanted by value
    user_minimal_data md{
        "Carlos","123456"
    };
    user user_(md);
    
    auto answer = qa_request.find("answer");
    if(!qa_request.is_null() && answer != qa_request.end()){
        cout << qa_request["answer"] << endl;
    }
    auto aa = form::get_register().cbegin();

    form_runner fr (user_,  *aa->second);

    auto & js = fr.run();

    cout << js.dump(4) << endl;
}

void request::send_action(std::string action, std::map<char,std::string> params){
    if(request_map.find(action) != request_map.end()){
        request_map[action](params);
    }else{
        cout << action << " :Does not match. Not provided correct arguments" <<endl;
    }
}


#endif //REQUEST_H
