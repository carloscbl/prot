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
 * Interval.h
 *
 * 
 */

#ifndef Interval_H_
#define Interval_H_


#include <nlohmann/json.hpp>

namespace org {
namespace openapitools {
namespace server {
namespace model {

/// <summary>
/// 
/// </summary>
class  Interval
{
public:
    Interval();
    virtual ~Interval();

    void validate();

    /////////////////////////////////////////////
    /// Interval members

    /// <summary>
    /// 
    /// </summary>
    int32_t getStart() const;
    void setStart(int32_t const value);
    /// <summary>
    /// 
    /// </summary>
    int32_t getEnd() const;
    void setEnd(int32_t const value);

    friend void to_json(nlohmann::json& j, const Interval& o);
    friend void from_json(const nlohmann::json& j, Interval& o);
protected:
    int32_t m_Start;

    int32_t m_End;

};

}
}
}
}

#endif /* Interval_H_ */
