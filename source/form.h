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


using namespace std;
using nlohmann::json;

//the form class stores the form in raw internally and process it to be able to present it externally
class form 
{
private:
    using form_register = map<string, unique_ptr<form>>;
    inline static form_register forms;
    json m_form;
public:
    bool ready_to_compute = false;
    string id;
    string name;                   //name of the form
    map<string, string> taskstory; //The taskstory handles all the posible commands for this form

    form();
    form(const std::string &path);
    virtual ~form();

    const json &get_json() const noexcept { return m_form; }

    static inline const form_register &get_register() noexcept { return form::forms; }

    static const string get_form_name(const json &j) { return j["form"]["form.name"].get<string>(); }
};
void from_json(const nlohmann::json& ref_json, form& new_form);

#endif //FORM_H
