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
 * Inline_object_1.h
 *
 * 
 */

#ifndef Inline_object_1_H_
#define Inline_object_1_H_


#include "Object.h"
#include <nlohmann/json.hpp>

namespace org {
namespace openapitools {
namespace server {
namespace model {

/// <summary>
/// 
/// </summary>
class  Inline_object_1
{
public:
    Inline_object_1();
    virtual ~Inline_object_1();

    void validate();

    /////////////////////////////////////////////
    /// Inline_object_1 members

    /// <summary>
    /// 
    /// </summary>
    Object getFormObj() const;
    void setFormObj(Object const& value);
    bool formObjIsSet() const;
    void unsetForm_obj();

    friend void to_json(nlohmann::json& j, const Inline_object_1& o);
    friend void from_json(const nlohmann::json& j, Inline_object_1& o);
protected:
    nlohmann::json m_Form_obj;
    bool m_Form_objIsSet;
};

}
}
}
}

#endif /* Inline_object_1_H_ */
