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
 * Inline_response_200_1.h
 *
 * 
 */

#ifndef Inline_response_200_1_H_
#define Inline_response_200_1_H_


#include <string>
#include "Object.h"
#include <nlohmann/json.hpp>

namespace org {
namespace openapitools {
namespace server {
namespace model {

/// <summary>
/// 
/// </summary>
class  Inline_response_200_1
{
public:
    Inline_response_200_1();
    virtual ~Inline_response_200_1();

    void validate();

    /////////////////////////////////////////////
    /// Inline_response_200_1 members

    /// <summary>
    /// 
    /// </summary>
    std::string getCurrentQuestion() const;
    void setCurrentQuestion(std::string const& value);
    /// <summary>
    /// 
    /// </summary>
    std::string getHints() const;
    void setHints(std::string const& value);
    bool hintsIsSet() const;
    void unsetHints();
    /// <summary>
    /// 
    /// </summary>
    std::string getDataType() const;
    void setDataType(std::string const& value);
    /// <summary>
    /// 
    /// </summary>
    nlohmann::json getTypeDetails() const;
    void setTypeDetails(nlohmann::json const& value);
    bool typeDetailsIsSet() const;
    void unsetType_details();

    friend void to_json(nlohmann::json& j, const Inline_response_200_1& o);
    friend void from_json(const nlohmann::json& j, Inline_response_200_1& o);
protected:
    std::string m_Current_question;

    std::string m_Hints;
    bool m_HintsIsSet;
    std::string m_Data_type;

    nlohmann::json m_Type_details;
    bool m_Type_detailsIsSet;
};

}
}
}
}

#endif /* Inline_response_200_1_H_ */
