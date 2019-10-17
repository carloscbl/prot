#include "when.h"

void from_json(const nlohmann::json& j, when& p){
    j.at("after").get_to(p.after);

    auto min_delay = j.find("minimum_delay");
    if( min_delay != j.end() ){
        min_delay.value().get_to(p.minimum_delay);
    }
    auto max_delay = j.find("maximum_delay");
    if( max_delay != j.end() ){
        max_delay.value().get_to(p.maximum_delay);
    }
}