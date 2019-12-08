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


#include "User_params.h"

namespace org {
namespace openapitools {
namespace server {
namespace model {

User_params::User_params()
{
    m_Time_zone = "";
    m_Time_zoneIsSet = false;
    
}

User_params::~User_params()
{
}

void User_params::validate()
{
    // TODO: implement validation
}

void to_json(nlohmann::json& j, const User_params& o)
{
    j = nlohmann::json();
    if(o.timeZoneIsSet())
        j["time_zone"] = o.m_Time_zone;
}

void from_json(const nlohmann::json& j, User_params& o)
{
    if(j.find("time_zone") != j.end())
    {
        j.at("time_zone").get_to(o.m_Time_zone);
        o.m_Time_zoneIsSet = true;
    } 
}

std::string User_params::getTimeZone() const
{
    return m_Time_zone;
}
void User_params::setTimeZone(std::string const& value)
{
    m_Time_zone = value;
    m_Time_zoneIsSet = true;
}
bool User_params::timeZoneIsSet() const
{
    return m_Time_zoneIsSet;
}
void User_params::unsetTime_zone()
{
    m_Time_zoneIsSet = false;
}

}
}
}
}

