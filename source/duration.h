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
        std::map<std::string,std::function<void(duration & ,unsigned long long)>> conversors{
            {"seconds", &duration::convert<std::chrono::seconds>},
            {"minutes", &duration::convert<std::chrono::minutes>},
            {"hours",   &duration::convert<std::chrono::hours>},
            {"days",    &duration::convert<days>},
            {"weeks",   &duration::convert<weeks>},
            {"months",  &duration::convert<months>},
            {"years",   &duration::convert<years>},
        };
        template <typename T_unit>
        void convert(unsigned long long time){
            m_duration += std::chrono::duration_cast<std::chrono::seconds>(T_unit(time));
        }
    };
    void from_json(const nlohmann::json& j, duration& p);
}
#endif //DURATION_H
