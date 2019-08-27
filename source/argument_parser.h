
#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include <string>
#include <map>
#include <regex>
using namespace std;
/*
mark expresions with {...}
{plus(now(),minus(50,35))}

Get the arguments for the most left external : "\((.*)\)"
Detect expr o var
starts with expr-> ^(.*\(.+\))$
wont match non expr
plus(now(),minus(50,35))
day.wait

Decompose to see how many args
(.+?(?:\(\)|\(.+\)))(?:,|$) ->> you can be sure, only got the first
if not empty continue
now(),minus(50,35)
now()
now(50,yesss(aassa))
now(50,46,yes(aaas)),aaa,bbbbb
*/
class argument_parser
{
private:
    
    const string arguments_regex = R"( ?(?:(--\S+?)|(-[a-z])) (?:[\"\'](.*?)[\"\']|\{(.+?)\}|(\S+)+?))";
public:
    argument_parser(/* args */);
    ~argument_parser();
};

argument_parser::argument_parser(/* args */)
{
}

argument_parser::~argument_parser()
{
}


void parse(const string &command_str)
{
    regex rex(command_placement_regex, regex_constants::ECMAScript | regex_constants::icase);
    smatch what;

    if (regex_search(command_str, what, rex))
    {
        this->command = what[1].str();
        this->placement.push_back(what[2].str());
        regex rex2(arguments_regex);
        smatch what2;
        // string s = "task add -u {user.user} -d 'Collect the washed clothes' --stamp {next_add_task_story_stamp__washer_start__time.minutes.WaitToCollect}";
        // string s2 = "-u {user.user} --lemons {author.name} --stumpo 'asasd asdasd' -d 'Gather Clothes' --stampao 5555";

        std::sregex_iterator next(command_str.begin(), command_str.end(), rex2);
        std::sregex_iterator end;
        while (next != end)
        {
            std::smatch match = *next;
            parameters.push_back(dual_param(match));
            next++;
        }
    }
    else
    {
        std::cout << "Match not found\n";
    }
}
#endif //ARGUMENT_PARSER_H
