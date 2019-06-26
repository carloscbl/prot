#include "command_expr_evaluator.h"

void command::parse(const string & command_str){
    regex rex ( command_placement_regex ,regex_constants::ECMAScript | regex_constants::icase);
    smatch what;

    if( regex_search(command_str, what, rex ) ){
        std::cout << "Match found\n";
        std::cout << "command: " << what[1].str() << endl;
        std::cout << "positional: " << what[2].str() << endl;
        std::cout << "parameters: " << what[3].str() << endl;
        regex rex2 ( arguments_regex );// ([\"\'](.*?)[\"\']|{(.+?)}|(\S+)+?))
        smatch what2;
        string s = "-u {user.user} --stumpo 'asasd asdasd' -d 'Gather Clothes' --stampao 5555";

        std::sregex_iterator next(s.begin(), s.end(), rex2);
        std::sregex_iterator end;
        while (next != end) {
            std::smatch match = *next;
            parameters.push_back(dual_param(match));

            cout << "-------" << endl;
            
            next++;
        }

    } else {
        std::cout << "Match not found\n";
    }
}

command_expr_evaluator::command_expr_evaluator(const json & taskstory, map<string,json> & variables)
:variables(variables)
{
    command co;
    cout << taskstory.cbegin().value()["command"] << endl;

    co.parse(taskstory.cbegin().value()["command"].get<string>());
    for (auto &&i : co.parameters)
    {
        if(i.is_expression){
            evaluate(i);
        }
    }
}

dual_param command_expr_evaluator::evaluate(dual_param & non_formated_param) const noexcept{
    const auto var = this->variables.find(non_formated_param.argument);
    if(var != variables.end()){
        cout << var->second << endl;

    }

    return non_formated_param;
}