#include "duration.h"

void from_json(const nlohmann::json& j, duration& p){
    //Iterates over all the timers and picks the one finded
    for (auto &[k,conversor] : p.conversors)
    {
        auto match = j.find(k);
        if(match != j.end()){
            conversor(p, match.value().get<unsigned long long>());
        }
    }
    //assert(p.m_duration != 0);
}