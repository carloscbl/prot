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


#include "Task.h"

namespace org {
namespace openapitools {
namespace server {
namespace model {

Task::Task()
{
    m_Name = "";
    m_Description = "";
    m_DescriptionIsSet = false;
    m_Task_id = "";
    m_Task_idIsSet = false;
    m_DurationIsSet = false;
    m_Frequency = "";
    m_FrequencyIsSet = false;
    m_WhenIsSet = false;
    m_IntervalIsSet = false;
    m_Task_group = "";
    m_Task_groupIsSet = false;
    m_RestrictionsIsSet = false;
    m_User = "";
    m_UserIsSet = false;
    m_Id = "";
    m_IdIsSet = false;
    
}

Task::~Task()
{
}

void Task::validate()
{
    // TODO: implement validation
}

void to_json(nlohmann::json& j, const Task& o)
{
    j = nlohmann::json();
    j["name"] = o.m_Name;
    if(o.descriptionIsSet())
        j["description"] = o.m_Description;
    if(o.taskIdIsSet())
        j["task_id"] = o.m_Task_id;
    if(o.durationIsSet())
        j["duration"] = o.m_Duration;
    if(o.frequencyIsSet())
        j["frequency"] = o.m_Frequency;
    if(o.whenIsSet())
        j["when"] = o.m_When;
    if(o.intervalIsSet())
        j["interval"] = o.m_Interval;
    if(o.taskGroupIsSet())
        j["task_group"] = o.m_Task_group;
    if(o.restrictionsIsSet() || !o.m_Restrictions.empty())
        j["restrictions"] = o.m_Restrictions;
    if(o.userIsSet())
        j["user"] = o.m_User;
    if(o.idIsSet())
        j["id"] = o.m_Id;
}

void from_json(const nlohmann::json& j, Task& o)
{
    j.at("name").get_to(o.m_Name);
    if(j.find("description") != j.end())
    {
        j.at("description").get_to(o.m_Description);
        o.m_DescriptionIsSet = true;
    } 
    if(j.find("task_id") != j.end())
    {
        j.at("task_id").get_to(o.m_Task_id);
        o.m_Task_idIsSet = true;
    } 
    if(j.find("duration") != j.end())
    {
        j.at("duration").get_to(o.m_Duration);
        o.m_DurationIsSet = true;
    } 
    if(j.find("frequency") != j.end())
    {
        j.at("frequency").get_to(o.m_Frequency);
        o.m_FrequencyIsSet = true;
    } 
    if(j.find("when") != j.end())
    {
        j.at("when").get_to(o.m_When);
        o.m_WhenIsSet = true;
    } 
    if(j.find("interval") != j.end())
    {
        j.at("interval").get_to(o.m_Interval);
        o.m_IntervalIsSet = true;
    } 
    if(j.find("task_group") != j.end())
    {
        j.at("task_group").get_to(o.m_Task_group);
        o.m_Task_groupIsSet = true;
    } 
    if(j.find("restrictions") != j.end())
    {
        j.at("restrictions").get_to(o.m_Restrictions);
        o.m_RestrictionsIsSet = true;
    } 
    if(j.find("user") != j.end())
    {
        j.at("user").get_to(o.m_User);
        o.m_UserIsSet = true;
    } 
    if(j.find("id") != j.end())
    {
        j.at("id").get_to(o.m_Id);
        o.m_IdIsSet = true;
    } 
}

std::string Task::getName() const
{
    return m_Name;
}
void Task::setName(std::string const& value)
{
    m_Name = value;
}
std::string Task::getDescription() const
{
    return m_Description;
}
void Task::setDescription(std::string const& value)
{
    m_Description = value;
    m_DescriptionIsSet = true;
}
bool Task::descriptionIsSet() const
{
    return m_DescriptionIsSet;
}
void Task::unsetDescription()
{
    m_DescriptionIsSet = false;
}
std::string Task::getTaskId() const
{
    return m_Task_id;
}
void Task::setTaskId(std::string const& value)
{
    m_Task_id = value;
    m_Task_idIsSet = true;
}
bool Task::taskIdIsSet() const
{
    return m_Task_idIsSet;
}
void Task::unsetTask_id()
{
    m_Task_idIsSet = false;
}
Duration Task::getDuration() const
{
    return m_Duration;
}
void Task::setDuration(Duration const& value)
{
    m_Duration = value;
    m_DurationIsSet = true;
}
bool Task::durationIsSet() const
{
    return m_DurationIsSet;
}
void Task::unsetDuration()
{
    m_DurationIsSet = false;
}
std::string Task::getFrequency() const
{
    return m_Frequency;
}
void Task::setFrequency(std::string const& value)
{
    m_Frequency = value;
    m_FrequencyIsSet = true;
}
bool Task::frequencyIsSet() const
{
    return m_FrequencyIsSet;
}
void Task::unsetFrequency()
{
    m_FrequencyIsSet = false;
}
When Task::getWhen() const
{
    return m_When;
}
void Task::setWhen(When const& value)
{
    m_When = value;
    m_WhenIsSet = true;
}
bool Task::whenIsSet() const
{
    return m_WhenIsSet;
}
void Task::unsetWhen()
{
    m_WhenIsSet = false;
}
Interval Task::getInterval() const
{
    return m_Interval;
}
void Task::setInterval(Interval const& value)
{
    m_Interval = value;
    m_IntervalIsSet = true;
}
bool Task::intervalIsSet() const
{
    return m_IntervalIsSet;
}
void Task::unsetInterval()
{
    m_IntervalIsSet = false;
}
std::string Task::getTaskGroup() const
{
    return m_Task_group;
}
void Task::setTaskGroup(std::string const& value)
{
    m_Task_group = value;
    m_Task_groupIsSet = true;
}
bool Task::taskGroupIsSet() const
{
    return m_Task_groupIsSet;
}
void Task::unsetTask_group()
{
    m_Task_groupIsSet = false;
}
std::vector<std::string>& Task::getRestrictions()
{
    return m_Restrictions;
}
void Task::setRestrictions(std::vector<std::string> const& value)
{
    m_Restrictions = value;
    m_RestrictionsIsSet = true;
}
bool Task::restrictionsIsSet() const
{
    return m_RestrictionsIsSet;
}
void Task::unsetRestrictions()
{
    m_RestrictionsIsSet = false;
}
std::string Task::getUser() const
{
    return m_User;
}
void Task::setUser(std::string const& value)
{
    m_User = value;
    m_UserIsSet = true;
}
bool Task::userIsSet() const
{
    return m_UserIsSet;
}
void Task::unsetUser()
{
    m_UserIsSet = false;
}
std::string Task::getId() const
{
    return m_Id;
}
void Task::setId(std::string const& value)
{
    m_Id = value;
    m_IdIsSet = true;
}
bool Task::idIsSet() const
{
    return m_IdIsSet;
}
void Task::unsetId()
{
    m_IdIsSet = false;
}

}
}
}
}

