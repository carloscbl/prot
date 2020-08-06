#include "duration.h"
#include <cassert>

void prot::from_json(const nlohmann::json& j, prot::duration& p){
    //Iterates over all the timers and picks the one found
    for (auto &[k,conversor] : time_conversors)
    {
        auto match = j.find(k);
        if(match != j.end()){
            conversor(p, match.value().get<int64_t>());
        }
    }
    assert(p.m_duration.count() >= 0);//Maybe they want to start inmediately?
}

void prot::to_json(nlohmann::json& j, const prot::duration& p){
    j = nlohmann::json{
        {"seconds", p.m_duration.count()}
    };
}