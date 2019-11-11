#include "when.h"
#include <cassert>

void from_json(const nlohmann::json& j, when& p){
    j.at("after").get_to(p.after);

    auto min_delay = j.find("minimum_delay");
    if( min_delay != j.end() ){
        min_delay.value().get_to(p.minimum_delay);
    }
    auto max_delay = j.find("maximum_delay");
    if( max_delay != j.end() ){
        max_delay.value().get_to(p.maximum_delay);
        assert(p.maximum_delay.m_duration > p.minimum_delay.m_duration);
    }
}
void to_json(nlohmann::json& j, const when& p) {
    j = nlohmann::json{
        {"after", p.after},
        {"minimum_delay", p.minimum_delay},
    };
    if(p.maximum_delay.m_duration.count() != 0){
        j["maximum_delay"] = p.maximum_delay;
    }
}