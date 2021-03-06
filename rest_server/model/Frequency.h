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
 * Frequency.h
 *
 * 
 */

#ifndef Frequency_H_
#define Frequency_H_


#include <string>
#include <nlohmann/json.hpp>

namespace org {
namespace openapitools {
namespace server {
namespace model {

/// <summary>
/// 
/// </summary>
class  Frequency
{
public:
    Frequency();
    virtual ~Frequency();

    void validate();

    /////////////////////////////////////////////
    /// Frequency members

    /// <summary>
    /// 
    /// </summary>
    std::string getFrequency() const;
    void setFrequency(std::string const& value);

    friend void to_json(nlohmann::json& j, const Frequency& o);
    friend void from_json(const nlohmann::json& j, Frequency& o);
protected:
    std::string m_Frequency;

};

}
}
}
}

#endif /* Frequency_H_ */
