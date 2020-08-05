#include "type_conversor.h"


optional<std::string> prot::json_to_string(const json & value){
    switch (value.type())
    {
    case json::value_t::boolean:
        return fmt::format("{}",value.get<bool>());
    case json::value_t::number_float:
        return fmt::format("{}",value.get<float>());
        // return std::to_string(value.get<float>());
    case json::value_t::number_integer:
        return fmt::format("{}",value.get<int>());
    case json::value_t::number_unsigned:
        return fmt::format("{}",value.get<size_t>());
    case json::value_t::string:
        return value.get<std::string>();
    default:
        return nullopt;
    }
}
