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

//the app class stores the app in raw internally and process it to be able to present it externally
class app 
{
private:
    const json m_app;
    uint32_t id = 0;
public:

    app( const json & valid_app );
    virtual ~app();

    const json &get_json() const noexcept { return m_app; }

    void set_id(uint32_t AppId){ this->id = AppId; }
    uint32_t get_id(){ return this->id; }


    static const string get_app_name(const json &j) { return j["app"]["app.name"].get<string>(); }
    const string get_app_name() const { return m_app["app"]["app.name"].get<string>(); }
};


#endif //FORM_H
