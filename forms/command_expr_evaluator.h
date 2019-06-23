#ifndef COMMAND_EXPR_EVALUATOR_H
#define COMMAND_EXPR_EVALUATOR_H
#include <string>
#include <map>
#include "json.hpp"

using namespace std;

using json = nlohmann::json;

class command{
protected:
    const string & command;
    const vector<string> placement;
    const map<string,string> parameters;
};

class task_command: public command{
public:
    task_command(const string command, const vector<string> placement,const map<string,string> parameters);
};

task_command::task_command(const map<string,string> parameters)
:command("form"),
parameters(parameters)
{}

class command_expr_evaluator
{
private:
    /* data */
public:
    command_expr_evaluator(const json & taskstory);
    virtual ~command_expr_evaluator();
};

command_expr_evaluator::command_expr_evaluator()
{
    task_command()
}

command_expr_evaluator::~command_expr_evaluator()
{
}
//This one to get the command
//^\S+ (?<positional>\S+)

//This regex to get grouped the args by type
//" ?(--(.+?) |-(.) )[\"\'](?<string>.+)[\"\']|{(?<expr>.+?)}|(?<word>\\S+)"

#endif //COMMAND_EXPR_EVALUATOR_H