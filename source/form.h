//
// Created by carlos on 2/03/19.
//

#ifndef FORM_H
#define FORM_H

#include <string>
#include <map>
#include <functional>
#include <memory>
#include "form_reader.h"
#include "json.hpp"

#include "CRUD_actionable.h"

using namespace std;

//the form class stores the form in raw internally and process it to be able to present it externally
class form : public CRUD_actionable<form>
{
private:
    using form_register = map<string, unique_ptr<form>>;
    inline static form_register forms;
    //Components
    unique_ptr<form_reader> json_reader;

    //This map, handles the posible actions to be performed from outside commands
    //Positional params
    CRUD_plus_actions_map form_map{
        {"add", [this](map<char, string> s) {
             //Creates a new form and store it with index id and passing a states machine graph
             unique_ptr<form> form_ = make_unique<form>();
             form_->add(s);
             form::forms[form_->name] = move(form_);
         }},
        {"remove", [](map<char, string> s) {}},
        {"update", [](map<char, string> s) {}},
        {"list", [](map<char, string> s) {
             for (const auto &form : form::forms)
             {
                 cout << form.second->name << endl;
             }
         }},
    };
    //Modulator params like -h -n -P
    map_local_functions setters{
        {'P', &form::set_path},
    };

    void set_path(const string &s);
    void pipelined_json_data_setter(const string &json_path);

public:
    bool ready_to_compute = false;
    string id;
    string name;                   //name of the form
    map<string, string> taskstory; //The taskstory handles all the posible commands for this form

    form();
    form(const std::string &path);
    virtual ~form();

    const json &get_json() const noexcept { return json_reader->get_json(); }

    static inline const form_register &get_register() noexcept { return form::forms; }

    static const string get_form_name(const json &j) { return j["form"]["form.name"].get<string>(); }
};
using form_t = std::shared_ptr<form>;
#endif //FORM_H
