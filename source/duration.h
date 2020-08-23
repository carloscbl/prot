#ifndef DURATION_H
#define DURATION_H

#include <map>
#include <string>
#include <chrono>
#include <functional>
#include "json.hpp"
#include "time_utils.hpp"

namespace prot{
    class duration{
    public:
        duration():m_duration(0){}
        std::chrono::seconds m_duration;
        friend void to_json(nlohmann::json& j, const duration& p);
        friend void from_json(const nlohmann::json& j, duration& p);
    };
    const inline std::map<std::string,std::function<std::chrono::seconds(unsigned long long)>> time_conversors{
        {"seconds", &duration_to_seconds<std::chrono::seconds>},
        {"minutes", &duration_to_seconds<std::chrono::minutes>},
        {"hours",   &duration_to_seconds<std::chrono::hours>},
        {"days",    &duration_to_seconds<days>},
        {"weeks",   &duration_to_seconds<weeks>},
        {"months",  &duration_to_seconds<months>},
        {"years",   &duration_to_seconds<years>},
    };
    void from_json(const nlohmann::json& j, duration& p);
    void to_json(nlohmann::json& j, const duration& p);
}
#endif //DURATION_H
