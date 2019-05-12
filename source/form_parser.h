#ifndef FORM_PARSER_H
#define FORM_PARSER_H
#include <string>
#include "json.hpp"

using json = nlohmann::json;
class form_parser{
private:
    const json & j;
public:
    form_parser(const json & j);
};

#endif //FORM_PARSER_H