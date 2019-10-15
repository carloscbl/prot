#ifndef DURATION_H
#define DURATION_H

#include <map>
#include <string>
#include <chrono>
#include <functional>
#include "json.hpp"

class duration{
public:
    duration():m_duration(0){}
    std::chrono::seconds m_duration;
    std::map<std::string,std::function<void(duration & ,unsigned long long)>> conversors{
        {"minutes", &duration::minutes},
        {"seconds", &duration::seconds},
        {"hours", &duration::hours},
    };
    void seconds(unsigned long long secs ){
        m_duration += std::chrono::seconds(secs);
    }
    void minutes(unsigned long long mins ){
        m_duration += std::chrono::duration_cast<std::chrono::seconds>(std::chrono::minutes(mins));
    }
    void hours(unsigned long long hours){
        m_duration += std::chrono::duration_cast<std::chrono::seconds>(std::chrono::hours(hours));
    }
};
void from_json(const nlohmann::json& j, duration& p);
#endif //DURATION_H
