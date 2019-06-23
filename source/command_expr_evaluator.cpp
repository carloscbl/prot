#include "command_expr_evaluator.h"

void command::parse(const string & command_str){
    std::regex self_regex(command_placement_regex, std::regex_constants::ECMAScript | std::regex_constants::icase);
    std::smatch matches;
     if (std::regex_search(command_str, matches, self_regex)) {
         std::cout << "Match found\n";

        for (size_t i = 0; i < matches.size(); ++i) {
            std::cout << i << ": '" << matches[i].str() << "'\n";
        }
    } else {
        std::cout << "Match not found\n";
    }
}

command_expr_evaluator::command_expr_evaluator(const json & taskstory)
{
    command co;
    cout << taskstory.cbegin().value()["command"] << endl;

    co.parse(taskstory.cbegin().value()["command"].get<string>());
}
