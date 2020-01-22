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
 * User.h
 *
 * 
 */

#ifndef User_H_
#define User_H_


#include "Tasker.h"
#include "User_params.h"
#include <string>
#include "Scheduler.h"
#include <nlohmann/json.hpp>

namespace org {
namespace openapitools {
namespace server {
namespace model {

/// <summary>
/// 
/// </summary>
class  User
{
public:
    User();
    virtual ~User();

    void validate();

    /////////////////////////////////////////////
    /// User members

    /// <summary>
    /// 
    /// </summary>
    std::string getUsername() const;
    void setUsername(std::string const& value);
    /// <summary>
    /// 
    /// </summary>
    User_params getParams() const;
    void setParams(User_params const& value);
    bool paramsIsSet() const;
    void unsetParams();
    /// <summary>
    /// 
    /// </summary>
    Tasker getTasker() const;
    void setTasker(Tasker const& value);
    bool taskerIsSet() const;
    void unsetTasker();
    /// <summary>
    /// 
    /// </summary>
    Scheduler getScheduler() const;
    void setScheduler(Scheduler const& value);
    bool schedulerIsSet() const;
    void unsetScheduler();
    /// <summary>
    /// 
    /// </summary>
    int32_t getUserId() const;
    void setUserId(int32_t const value);

    friend void to_json(nlohmann::json& j, const User& o);
    friend void from_json(const nlohmann::json& j, User& o);
protected:
    std::string m_Username;

    User_params m_Params;
    bool m_ParamsIsSet;
    Tasker m_Tasker;
    bool m_TaskerIsSet;
    Scheduler m_Scheduler;
    bool m_SchedulerIsSet;
    int32_t m_User_id;

};

}
}
}
}

#endif /* User_H_ */
