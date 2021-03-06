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
/*
 * Task.h
 *
 * 
 */

#ifndef Task_H_
#define Task_H_


#include "Duration.h"
#include <string>
#include "When.h"
#include "Interval.h"
#include <vector>
#include <nlohmann/json.hpp>

namespace org {
namespace openapitools {
namespace server {
namespace model {

/// <summary>
/// 
/// </summary>
class  Task
{
public:
    Task();
    virtual ~Task();

    void validate();

    /////////////////////////////////////////////
    /// Task members

    /// <summary>
    /// 
    /// </summary>
    std::string getName() const;
    void setName(std::string const& value);
    /// <summary>
    /// 
    /// </summary>
    std::string getDescription() const;
    void setDescription(std::string const& value);
    bool descriptionIsSet() const;
    void unsetDescription();
    /// <summary>
    /// 
    /// </summary>
    std::string getTaskId() const;
    void setTaskId(std::string const& value);
    bool taskIdIsSet() const;
    void unsetTask_id();
    /// <summary>
    /// 
    /// </summary>
    Duration getDuration() const;
    void setDuration(Duration const& value);
    bool durationIsSet() const;
    void unsetDuration();
    /// <summary>
    /// 
    /// </summary>
    std::string getFrequency() const;
    void setFrequency(std::string const& value);
    bool frequencyIsSet() const;
    void unsetFrequency();
    /// <summary>
    /// 
    /// </summary>
    When getWhen() const;
    void setWhen(When const& value);
    bool whenIsSet() const;
    void unsetWhen();
    /// <summary>
    /// 
    /// </summary>
    Interval getInterval() const;
    void setInterval(Interval const& value);
    bool intervalIsSet() const;
    void unsetInterval();
    /// <summary>
    /// 
    /// </summary>
    std::string getTaskGroup() const;
    void setTaskGroup(std::string const& value);
    bool taskGroupIsSet() const;
    void unsetTask_group();
    /// <summary>
    /// 
    /// </summary>
    std::vector<std::string>& getRestrictions();
    void setRestrictions(std::vector<std::string> const& value);
    bool restrictionsIsSet() const;
    void unsetRestrictions();
    /// <summary>
    /// 
    /// </summary>
    std::string getUser() const;
    void setUser(std::string const& value);
    bool userIsSet() const;
    void unsetUser();
    /// <summary>
    /// 
    /// </summary>
    std::string getId() const;
    void setId(std::string const& value);
    bool idIsSet() const;
    void unsetId();

    friend void to_json(nlohmann::json& j, const Task& o);
    friend void from_json(const nlohmann::json& j, Task& o);
protected:
    std::string m_Name;

    std::string m_Description;
    bool m_DescriptionIsSet;
    std::string m_Task_id;
    bool m_Task_idIsSet;
    Duration m_Duration;
    bool m_DurationIsSet;
    std::string m_Frequency;
    bool m_FrequencyIsSet;
    When m_When;
    bool m_WhenIsSet;
    Interval m_Interval;
    bool m_IntervalIsSet;
    std::string m_Task_group;
    bool m_Task_groupIsSet;
    std::vector<std::string> m_Restrictions;
    bool m_RestrictionsIsSet;
    std::string m_User;
    bool m_UserIsSet;
    std::string m_Id;
    bool m_IdIsSet;
};

}
}
}
}

#endif /* Task_H_ */
