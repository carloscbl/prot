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
 * Inline_object_2.h
 *
 * 
 */

#ifndef Inline_object_2_H_
#define Inline_object_2_H_


#include "Object.h"
#include <nlohmann/json.hpp>

namespace org {
namespace openapitools {
namespace server {
namespace model {

/// <summary>
/// 
/// </summary>
class  Inline_object_2
{
public:
    Inline_object_2();
    virtual ~Inline_object_2();

    void validate();

    /////////////////////////////////////////////
    /// Inline_object_2 members

    /// <summary>
    /// 
    /// </summary>
    Object getAppObj() const;
    void setAppObj(Object const& value);
    bool appObjIsSet() const;
    void unsetApp_obj();

    friend void to_json(nlohmann::json& j, const Inline_object_2& o);
    friend void from_json(const nlohmann::json& j, Inline_object_2& o);
protected:
    nlohmann::json m_App_obj;
    bool m_App_objIsSet;
};

}
}
}
}

#endif /* Inline_object_2_H_ */
