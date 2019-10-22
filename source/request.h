#ifndef REQUEST_H
#define REQUEST_H
#include "CRUD_actionable.h"
#include "user.h"
#include "iuser.h"
#include "form.h"
#include "form_runner.h"
#include "command_processor.h"

class request : public CRUD_actionable<request>
{
private:
    CRUD_plus_actions_map request_map{
        {"add", [](map<char, string> s) {}},
        {"test", [this](map<char, string> s) {
             json j;
             if (!s.empty())
             {
                 j["answer"] = s.cbegin()->second;
             }
             evaluate_form(j);
         }},
         {"test_movement", [this](map<char, string> s) {
             json j;
             if (!s.empty())
             {
                 j["answer"] = s.cbegin()->second;
             }
             evaluate_test_movement(j);
         }},
        {"list", [](map<char, string> s) {}},
    };

    //Modulator params like -h -n -P
    map_local_functions setters{
        {'t', &request::evaluate_form},
    };
    command_processor & cp;

public:
    request(command_processor & cp);
    ~request() = default;

    void evaluate_form(const json qa_request);
    void evaluate_test_movement(const json qa_request);
};

request::request(command_processor & cp) : CRUD_actionable(this->request_map, setters),cp(cp)
{
}

//As this call should be threaded, and handle a life time singulary special each instance we need to set a good set of rules
//A timer for maximum life time
//A saved session with a id and persistence, to refer on callbacks and to recover session if it is no longer in memory or in another instance
//We also need a valid user that should be valid outside
//Good idea to wrap this call with a restrictor, to obligate to instanciate a user or send a valid user from outside to avoid repeat same checks
void request::evaluate_form(const json qa_request)
{ //Wanted by value
    const auto & carlos = user::users["carlos"];

    auto answer = qa_request.find("answer");
    if (!qa_request.is_null() && answer != qa_request.end())
    {
        //cout << qa_request["answer"] << endl;
    }
    const auto & form = form::get_register().at("Washer easer");

    form_runner fr(carlos, *form);

    auto &response = fr.run(qa_request);

    //cout << response.dump(4) << endl;
}

void request::evaluate_test_movement(const json qa_request){

}


#endif //REQUEST_H
