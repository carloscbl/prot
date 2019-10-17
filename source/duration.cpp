#include "duration.h"
#include <cassert>

void prot::from_json(const nlohmann::json& j, prot::duration& p){
    //Iterates over all the timers and picks the one found
    for (auto &[k,conversor] : p.conversors)
    {
        auto match = j.find(k);
        if(match != j.end()){
            conversor(p, match.value().get<int64_t>());
        }
    }
    assert(p.m_duration.count() != 0);
}