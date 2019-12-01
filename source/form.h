//
// Created by carlos on 2/03/19.
//

#ifndef FORM_H
#define FORM_H

#include <string>
#include <map>
#include <functional>
#include <memory>
#include "json.hpp"


using namespace std;
using nlohmann::json;

//the form class stores the form in raw internally and process it to be able to present it externally
class form 
{
private:
    using form_register = map<string, unique_ptr<form>>;
    inline static form_register forms;
    const json m_form;
public:

    form( const json & valid_form );
    virtual ~form();

    const json &get_json() const noexcept { return m_form; }

    static inline const form_register &get_forms_register() noexcept { return form::forms; }
    static inline void remove_form(const string & form_name ) noexcept { form::forms.erase(form_name); }

    static const string get_form_name(const json &j) { return j["form"]["form.name"].get<string>(); }
    const string get_form_name() const { return m_form["form"]["form.name"].get<string>(); }
};

#endif //FORM_H
