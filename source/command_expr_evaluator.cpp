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

string command::render() const noexcept
{
    if (!this->is_well_evaluated_command)
        return string();
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

command_expr_evaluator::command_expr_evaluator(const string &command_str, map<string, json> &variables)
    : variables(variables)
{
    cout << command_str << endl;
    co.parse(command_str);
    for (auto &&i : co.parameters)
    {
        if (i.is_expression){
            auto dual_p = evaluate(i);
            if (dual_p.has_value()){ 
                i = dual_p.value(); //we need to be able to track previous params, to set relative expresions, like end 30 min after start param
            }
            else{
                //This is a not a valid command
                co.is_well_evaluated_command = false;
                cout << "this command can't be resolved, non mapped: " << i.argument << endl;
                break;
            }
        }
    }
    if (co.is_well_evaluated_command){
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
            return dual_param(non_formated_param.type, var->second.get<string>());
        }
        else
        {
            //Probably it is an object if not it is a straight value
            //We expect for now a binding like structure
            if (var->second.type() == json::value_t::object)
            {
                const json &js = var->second;
                const auto &match = js.find("function");
                if (match != js.cend())
                {
                    cout << "Found a binding to: " << non_formated_param.argument
                         << " : " << match.value() << endl;
                    
                    const auto & function = bindings_map.find(match.value());
                    if(function != bindings_map.end()){
                        auto opt = function->second(const_cast<command_expr_evaluator&>(*this), js["args"]);
                        return opt;
                    }else{
                        cout << "NOT IMPLEMENTATION MATCH" << match.value() << endl;
                    }
                    //Now is the real moment to evaluate
                    //Time to continue from here to parse expressions now that we have our relative funcs
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


binding_return_t command_expr_evaluator::next_add_task_story_stamp(const json & args)
{
    cout << "Evaluation via: "<< __func__ <<  endl;
    cout << "args: " << args.dump(4) << endl;
    string ref_ = args["prev_task_tag"].get<string>();

    auto task_tag_ref = this->variables.find(ref_);
    if(task_tag_ref != variables.end()){
        time_t ref_task_end = task_tag_ref->second["interval"]["end"].get<time_t>();
        cout << ref_task_end << endl;
        auto time = get_real_time(args["time"]);
        if(!time.has_value()){
            cout << "the 'time' reference is invalid" << endl;
            return nullopt;
        }

        return dual_param("s", ref_task_end  + time.value() ) ;
    }

    return nullopt;
}

optional<time_t> command_expr_evaluator::get_real_time(const json & time_ref){
    if(time_ref.type() == json::value_t::string){
        auto match = variables.find(time_ref.get<string>());
        if(match != variables.end()){
            return match->second.get<time_t>();
        }else{
            return nullopt;
        }
    }else if(time_ref.type() == json::value_t::number_integer){
        return time_ref.get<time_t>();
    }else{
        return nullopt;
    }
}

binding_return_t now(const json & args){
    return dual_param("")
}
