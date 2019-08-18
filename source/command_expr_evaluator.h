#ifndef COMMAND_EXPR_EVALUATOR_H
#define COMMAND_EXPR_EVALUATOR_H

#include <string>
#include <map>
#include <regex>
#include <functional>
#include <iostream>
#include "json.hpp"

using namespace std;

using json = nlohmann::json;
class command_expr_evaluator;
class command;



/*
This class represents the duality between a command indicator and the param
eg: prot positional [--stamp 1566137097] -> both and determines if it is a expression
*/
class dual_param
{
private:
    friend command_expr_evaluator;
    friend command;
    int matches = 0;
    string type;
    string argument;
    bool is_expression = false;
    enum class kind
    {
        text,
        word,
        expression
    };
    kind arg_kind;

    void ftypeTwoMinus(const string &parameter) noexcept
    {
        //Remove the -- and save to type
        this->type.assign(parameter.begin() + 2, parameter.end());
    }

    void ftypeOneMinus(const string &parameter) noexcept
    {
        //Remove the - and save to type
        this->type.assign(parameter.begin() + 1, parameter.end());
    }

    void expression(const string &parameter) noexcept
    {
        //Here we have to substitute the argument
        //Published Var
        //User Var
        //Tagged Var
        //Crawled Var --> Future
        //Relative Var --> From Already processed tasks from taskstories (or scheduled task --> Future )
        //We need this Easy to debug so full trazability would be nice, but caution
        is_expression = true;
        arg_kind = kind::expression;
        this->argument = parameter;
    }

    void text(const string &parameter) noexcept
    {
        arg_kind = kind::text;
        this->argument = parameter;
    }

    void word(const string &parameter) noexcept
    {
        arg_kind = kind::word;
        this->argument = parameter;
    }

    map<int, function<void(string parameter)>> type_position_in_regex{
        {1, [this](const string &s) { ftypeTwoMinus(s); }},
        {2, [this](const string &s) { ftypeOneMinus(s); }},
        {3, [this](const string &s) { text(s); }},
        {4, [this](const string &s) { expression(s); }},
        {5, [this](const string &s) { word(s); }},
    };

public:
    dual_param(const std::smatch &match)
    {
        if (match[0].matched)
        {
            for (auto &&[k, v] : type_position_in_regex)
            {
                if (!match.str(k).empty())
                {
                    v(match.str(k));
                    matches++;
                    if (matches > 2)
                    {
                        cout << "parsing error, got more matches than 2" << endl;
                    }
                }
            }
        }
        cout << type << ":" << argument << endl;
    }

    dual_param(const string type, const string argument)
        : type(type),
          argument(argument),
          is_expression(false),
          arg_kind(kind::text)
    {
    }
};

//Represents the command over its evolution
class command
{
protected:
    friend command_expr_evaluator;
    string command;
    vector<string> placement;
    vector<dual_param> parameters;

    bool is_well_evaluated_command = true;

    const string command_placement_regex = "^(\\S+) (\\S+)(.*)";
    const string arguments_regex = R"( ?(?:(--\S+?)|(-[a-z])) (?:[\"\'](.*?)[\"\']|\{(.+?)\}|(\S+)+?))";

public:
    void parse(const string &command_str);
    string render() const noexcept;
};

//This class iterate over all the params
//Enables the resolution of variables
class command_expr_evaluator
{
private:
    map<string, json> &variables;
    command co;
    //This "enroute" varaibles, functions or undefined by calling the correct mapping and retreving and setting the correct value or nullopt
    optional<dual_param> evaluate(dual_param &non_formated_param) const noexcept;

public:
    command_expr_evaluator(const string &command_str, map<string, json> &variables);
    command_expr_evaluator() = delete;
};

#endif //COMMAND_EXPR_EVALUATOR_H