#include "when.h"

void from_json(const nlohmann::json& j, when& p){
        j.at("after").get_to(p.after);
}