#ifndef COMMAND_EXPR_EVALUATOR_H
#define COMMAND_EXPR_EVALUATOR_H
#include <string>
#include <map>
#include <regex>
#include <iostream>
#include "json.hpp"

using namespace std;

using json = nlohmann::json;

class command{
protected:
    string command;
    vector<string> placement;
    map<string,string> parameters;

    const string command_placement_regex = R"(^(?<command>\S+) (?<positional>\S+)(?<params>.*))";
    const string arguments_regex = R"( ?(--(.+?) |-(.) )["'](?<string>.+)["']|{(?<expr>.+?)}|(?<word>\S+))";
public:
    void parse(const string & command_str);
};



// class task_command: public command{
// public:
//     task_command(const string command, const vector<string> placement,const map<string,string> parameters);
// };

// task_command::task_command(const map<string,string> parameters)
// :command("form"),
// parameters(parameters)
// {}

class command_expr_evaluator
{
public:
    command_expr_evaluator(const json & taskstory);
};


//This one to get the command
//^\S+ (?<positional>\S+)

//This regex to get grouped the args by type
//" ?(--(.+?) |-(.) )[\"\'](?<string>.+)[\"\']|{(?<expr>.+?)}|(?<word>\\S+)"

#endif //COMMAND_EXPR_EVALUATOR_H