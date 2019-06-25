#include "command_expr_evaluator.h"

void command::parse(const string & command_str){
    regex rex ( command_placement_regex ,regex_constants::ECMAScript | regex_constants::icase);
    smatch what;

    if( regex_search(command_str, what, rex ) ){
        std::cout << "Match found\n";
        std::cout << "command: " << what[1].str() << endl;
        std::cout << "positional: " << what[2].str() << endl;
        std::cout << "parameters: " << what[3].str() << endl;
        regex rex2 ( R"( ?(?:(--\S+?)|(-[a-z])) (?:[\"\'](.*?)[\"\']|(\{.+?\})|(\S+)+?))" );// ([\"\'](.*?)[\"\']|{(.+?)}|(\S+)+?))
        smatch what2;
        string s = "-u {user.user} --stumpo 'asasd asdasd' -d 'Gather Clothes' --stampao 5555";

        std::sregex_iterator next(s.begin(), s.end(), rex2);
        std::sregex_iterator end;
        while (next != end) {
            std::smatch match = *next;
            for (auto &&i : match)
            {
                if(i.matched){
                    cout << "DOUBLE- " << match.str(1) ;
                    cout << "SINGLE- " << match.str(2) ;
                    cout << "''- " << match.str(3) ;
                    cout << "{}- " << match.str(4) ;
                    cout << "WORD- " << match.str(5) << endl;
                    //cout << i.str() << endl;

                }
            }
            cout << "-------" << endl;
            
            next++;
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
