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
 * Prot_app_info.h
 *
 * 
 */

#ifndef Prot_app_info_H_
#define Prot_app_info_H_


#include <string>
#include <nlohmann/json.hpp>

namespace org {
namespace openapitools {
namespace server {
namespace model {

/// <summary>
/// 
/// </summary>
class  Prot_app_info
{
public:
    Prot_app_info();
    virtual ~Prot_app_info();

    void validate();

    /////////////////////////////////////////////
    /// Prot_app_info members

    /// <summary>
    /// 
    /// </summary>
    int32_t getAppId() const;
    void setAppId(int32_t const value);
    /// <summary>
    /// 
    /// </summary>
    std::string getAppName() const;
    void setAppName(std::string const& value);

    friend void to_json(nlohmann::json& j, const Prot_app_info& o);
    friend void from_json(const nlohmann::json& j, Prot_app_info& o);
protected:
    int32_t m_App_id;

    std::string m_App_name;

};

}
}
}
}

#endif /* Prot_app_info_H_ */
