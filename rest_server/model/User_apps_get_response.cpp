/**
* prot
* No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
*
* The version of the OpenAPI document: 1.0
* 
*
* NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
* https://openapi-generator.tech
* Do not edit the class manually.
*/


#include "User_apps_get_response.h"

namespace org {
namespace openapitools {
namespace server {
namespace model {

User_apps_get_response::User_apps_get_response()
{
    m_App_id = 0;
    m_App_idIsSet = false;
    m_App_name = "";
    m_App_nameIsSet = false;
    m_Qa_historyIsSet = false;
    
}

User_apps_get_response::~User_apps_get_response()
{
}

void User_apps_get_response::validate()
{
    // TODO: implement validation
}

void to_json(nlohmann::json& j, const User_apps_get_response& o)
{
    j = nlohmann::json();
    if(o.appIdIsSet())
        j["app_id"] = o.m_App_id;
    if(o.appNameIsSet())
        j["app_name"] = o.m_App_name;
    if(o.qaHistoryIsSet())
        j["qa_history"] = o.m_Qa_history;
}

void from_json(const nlohmann::json& j, User_apps_get_response& o)
{
    if(j.find("app_id") != j.end())
    {
        j.at("app_id").get_to(o.m_App_id);
        o.m_App_idIsSet = true;
    } 
    if(j.find("app_name") != j.end())
    {
        j.at("app_name").get_to(o.m_App_name);
        o.m_App_nameIsSet = true;
    } 
    if(j.find("qa_history") != j.end())
    {
        j.at("qa_history").get_to(o.m_Qa_history);
        o.m_Qa_historyIsSet = true;
    } 
}

int32_t User_apps_get_response::getAppId() const
{
    return m_App_id;
}
void User_apps_get_response::setAppId(int32_t const value)
{
    m_App_id = value;
    m_App_idIsSet = true;
}
bool User_apps_get_response::appIdIsSet() const
{
    return m_App_idIsSet;
}
void User_apps_get_response::unsetApp_id()
{
    m_App_idIsSet = false;
}
std::string User_apps_get_response::getAppName() const
{
    return m_App_name;
}
void User_apps_get_response::setAppName(std::string const& value)
{
    m_App_name = value;
    m_App_nameIsSet = true;
}
bool User_apps_get_response::appNameIsSet() const
{
    return m_App_nameIsSet;
}
void User_apps_get_response::unsetApp_name()
{
    m_App_nameIsSet = false;
}
nlohmann::json User_apps_get_response::getQaHistory() const
{
    return m_Qa_history;
}
void User_apps_get_response::setQaHistory(nlohmann::json const& value)
{
    m_Qa_history = value;
    m_Qa_historyIsSet = true;
}
bool User_apps_get_response::qaHistoryIsSet() const
{
    return m_Qa_historyIsSet;
}
void User_apps_get_response::unsetQa_history()
{
    m_Qa_historyIsSet = false;
}

}
}
}
}

