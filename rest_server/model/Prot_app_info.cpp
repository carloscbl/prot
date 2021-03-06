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


#include "Prot_app_info.h"

namespace org {
namespace openapitools {
namespace server {
namespace model {

Prot_app_info::Prot_app_info()
{
    m_App_id = 0;
    m_App_name = "";
    m_App_unstructured_infoIsSet = false;
    
}

Prot_app_info::~Prot_app_info()
{
}

void Prot_app_info::validate()
{
    // TODO: implement validation
}

void to_json(nlohmann::json& j, const Prot_app_info& o)
{
    j = nlohmann::json();
    j["app_id"] = o.m_App_id;
    j["app_name"] = o.m_App_name;
    if(o.appUnstructuredInfoIsSet())
        j["app_unstructured_info"] = o.m_App_unstructured_info;
}

void from_json(const nlohmann::json& j, Prot_app_info& o)
{
    j.at("app_id").get_to(o.m_App_id);
    j.at("app_name").get_to(o.m_App_name);
    if(j.find("app_unstructured_info") != j.end())
    {
        j.at("app_unstructured_info").get_to(o.m_App_unstructured_info);
        o.m_App_unstructured_infoIsSet = true;
    } 
}

int32_t Prot_app_info::getAppId() const
{
    return m_App_id;
}
void Prot_app_info::setAppId(int32_t const value)
{
    m_App_id = value;
}
std::string Prot_app_info::getAppName() const
{
    return m_App_name;
}
void Prot_app_info::setAppName(std::string const& value)
{
    m_App_name = value;
}
nlohmann::json Prot_app_info::getAppUnstructuredInfo() const
{
    return m_App_unstructured_info;
}
void Prot_app_info::setAppUnstructuredInfo(nlohmann::json const& value)
{
    m_App_unstructured_info = value;
    m_App_unstructured_infoIsSet = true;
}
bool Prot_app_info::appUnstructuredInfoIsSet() const
{
    return m_App_unstructured_infoIsSet;
}
void Prot_app_info::unsetApp_unstructured_info()
{
    m_App_unstructured_infoIsSet = false;
}

}
}
}
}

