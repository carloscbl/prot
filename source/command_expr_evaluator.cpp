#include "command_expr_evaluator.h"
#include <sstream>

void command::parse(const string &command_str)
{
    regex rex(command_placement_regex, regex_constants::ECMAScript | regex_constants::icase);
    smatch what;

    if (regex_search(command_str, what, rex))
    {
        this->command = what[1].str();
        this->placement.push_back(what[2].str());
        regex rex2(arguments_regex);
        smatch what2;
        string s = "task add -u {user.user} -d 'Collect the washed clothes' --stamp {next_add_task_story_stamp__washer_start__time.minutes.WaitToCollect}";
        string s2 = "-u {user.user} --lemons {author.name} --stumpo 'asasd asdasd' -d 'Gather Clothes' --stampao 5555";

        std::sregex_iterator next(s.begin(), s.end(), rex2);
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

string command::render() const noexcept
{
    if (!this->is_well_evaluated_command)
        return "Not well evaluated";
    std::stringstream ss;
    ss << command << " ";
    for (auto &&i : placement)
    {
        ss << i;
    }
    for (auto &&i : parameters)
    {
        ss << " ";
        if (i.type.size() > 1)
        {
            ss << "--";
        }
        else
        {
            ss << "-";
        }
        ss << i.type << " ";
        if (i.arg_kind == dual_param::kind::text)
        {
            ss << "'" << i.argument << "'";
        }
        else
        {
            ss << i.argument;
        }
    }
    return ss.str();
}

command_expr_evaluator::command_expr_evaluator(const json &taskstory, map<string, json> &variables)
    : variables(variables)
{
    command co;
    cout << taskstory.cbegin().value()["command"] << endl;

    co.parse(taskstory.cbegin().value()["command"].get<string>());
    for (auto &&i : co.parameters)
    {
        if (i.is_expression)
        {
            auto dual_p = evaluate(i);
            if (dual_p.has_value())
            {
                i = dual_p.value();
            }
            else
            {
                //This is a not a valid command
                co.is_well_evaluated_command = false;
                cout << "this command can't be resolved, non mapped: " << i.argument << endl;
                break;
            }
        }
    }

    if (co.is_well_evaluated_command)
    {
        cout << co.render() << endl;
    }
}

optional<dual_param> command_expr_evaluator::evaluate(dual_param &non_formated_param) const noexcept
{
    const auto &var = this->variables.find(non_formated_param.argument);
    if (var != variables.end())
    {
        if (var->second.type() == json::value_t::string) //Plain variable
        {
            cout << var->second << endl;
            return dual_param(non_formated_param.type, var->second);
        }
        else
        {
            //Probably it is an object if not it is a straight value
            //We expect for now an binding like structure
            if (var->second.type() == json::value_t::object)
            {
                const json &js = var->second;
                const auto &match = js.find("function");
                if (match != js.cend())
                {
                    cout << "Found a binding to: " << non_formated_param.argument
                         << " : " << match.value() << endl;
                    //Now is the real moment to evaluate
                    //
                }
                else
                {
                    cout << "Non a binding structure: " << var->second << endl;
                }
            }
            else
            {
                cout << "NON SUPPORTED: " << var->second << endl;
            }
        }
    }
    return nullopt;
}

//This is used to handle the referenciation to a scheduled task from the same form via unique tag
//To this purpose we should get access form here to:
//1 the current user data
//2 its scheduler
//3 keep track of the tag in the scheduler to retrive the data
string next_add_task_story_stamp(const json js)
{
    return "";
}

map<string, function<string(json)>> bindings_map{
    {"next_add_task_story_stamp", [](const json &j) { return next_add_task_story_stamp(j); }},
    //{"next_add_task_story_stamp",[](const json & j){return next_add_task_story_stamp(j);}},
};
