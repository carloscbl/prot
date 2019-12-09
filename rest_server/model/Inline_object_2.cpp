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


#include "Inline_object_2.h"

namespace org {
namespace openapitools {
namespace server {
namespace model {

Inline_object_2::Inline_object_2()
{
    m_Json_str = "";
    m_Json_strIsSet = false;
    m_ = "";
    m_IsSet = false;
    
}

Inline_object_2::~Inline_object_2()
{
}

void Inline_object_2::validate()
{
    // TODO: implement validation
}

void to_json(nlohmann::json& j, const Inline_object_2& o)
{
    j = nlohmann::json();
    if(o.jsonStrIsSet())
        j["json_str"] = o.m_Json_str;
    if(o.IsSet())
        j[""] = o.m_;
}

void from_json(const nlohmann::json& j, Inline_object_2& o)
{
    if(j.find("json_str") != j.end())
    {
        j.at("json_str").get_to(o.m_Json_str);
        o.m_Json_strIsSet = true;
    } 
    if(j.find("") != j.end())
    {
        j.at("").get_to(o.m_);
        o.m_IsSet = true;
    } 
}

std::string Inline_object_2::getJsonStr() const
{
    return m_Json_str;
}
void Inline_object_2::setJsonStr(std::string const& value)
{
    m_Json_str = value;
    m_Json_strIsSet = true;
}
bool Inline_object_2::jsonStrIsSet() const
{
    return m_Json_strIsSet;
}
void Inline_object_2::unsetJson_str()
{
    m_Json_strIsSet = false;
}
std::string Inline_object_2::get() const
{
    return m_;
}
void Inline_object_2::set(std::string const& value)
{
    m_ = value;
    m_IsSet = true;
}
bool Inline_object_2::IsSet() const
{
    return m_IsSet;
}
void Inline_object_2::unset()
{
    m_IsSet = false;
}

}
}
}
}

