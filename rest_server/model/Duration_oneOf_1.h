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
 * Duration_oneOf_1.h
 *
 * 
 */

#ifndef Duration_oneOf_1_H_
#define Duration_oneOf_1_H_


#include <nlohmann/json.hpp>

namespace org {
namespace openapitools {
namespace server {
namespace model {

/// <summary>
/// 
/// </summary>
class  Duration_oneOf_1
{
public:
    Duration_oneOf_1();
    virtual ~Duration_oneOf_1();

    void validate();

    /////////////////////////////////////////////
    /// Duration_oneOf_1 members

    /// <summary>
    /// 
    /// </summary>
    int32_t getSeconds() const;
    void setSeconds(int32_t const value);
    bool secondsIsSet() const;
    void unsetSeconds();
    /// <summary>
    /// 
    /// </summary>
    int32_t getMinutes() const;
    void setMinutes(int32_t const value);
    bool minutesIsSet() const;
    void unsetMinutes();
    /// <summary>
    /// 
    /// </summary>
    int32_t getHours() const;
    void setHours(int32_t const value);
    bool hoursIsSet() const;
    void unsetHours();
    /// <summary>
    /// 
    /// </summary>
    int32_t getDays() const;
    void setDays(int32_t const value);
    bool daysIsSet() const;
    void unsetDays();
    /// <summary>
    /// 
    /// </summary>
    int32_t getWeeks() const;
    void setWeeks(int32_t const value);
    bool weeksIsSet() const;
    void unsetWeeks();
    /// <summary>
    /// 
    /// </summary>
    int32_t getMonths() const;
    void setMonths(int32_t const value);
    bool monthsIsSet() const;
    void unsetMonths();
    /// <summary>
    /// 
    /// </summary>
    int32_t getYears() const;
    void setYears(int32_t const value);
    bool yearsIsSet() const;
    void unsetYears();

    friend void to_json(nlohmann::json& j, const Duration_oneOf_1& o);
    friend void from_json(const nlohmann::json& j, Duration_oneOf_1& o);
protected:
    int32_t m_Seconds;
    bool m_SecondsIsSet;
    int32_t m_Minutes;
    bool m_MinutesIsSet;
    int32_t m_Hours;
    bool m_HoursIsSet;
    int32_t m_Days;
    bool m_DaysIsSet;
    int32_t m_Weeks;
    bool m_WeeksIsSet;
    int32_t m_Months;
    bool m_MonthsIsSet;
    int32_t m_Years;
    bool m_YearsIsSet;
};

}
}
}
}

#endif /* Duration_oneOf_1_H_ */
