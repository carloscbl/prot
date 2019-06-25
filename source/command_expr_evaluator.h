#ifndef COMMAND_EXPR_EVALUATOR_H
#define COMMAND_EXPR_EVALUATOR_H
#include <string>
#include <map>
#include <regex>
#include <functional>
#include <iostream>
#include "json.hpp"
//#include <boost/xpressive/xpressive.hpp>


using namespace std;
//using namespace boost::xpressive;

using json = nlohmann::json;


class command{
protected:
    string command;
    vector<string> placement;
    map<string,string> parameters;

    const string command_placement_regex = "^(\\S+) (\\S+)(.*)";
    const string arguments_regex = " ?(--(.+?) |-(.) )[\"\'](?P<string>.+)[\"\']|{(?P<expr>.+?)}|(\\S+)";
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

class dual_param{
    string type;
    string argument;
    int matches = 0;
public:
    dual_param(const std::smatch & match){
        if(match[0].matched){
            for (auto && [k,v] : type_position_in_regex)
            {
                if(!match.str(k).empty()){
                    v(match.str(k));
                    matches++;
                    //cout << k << " " << match.str(k)<< " " ;
                }
            }
            //cout << endl;
        }
    }

    void expression(const string & parameter) noexcept{

    }

    void text(const string & parameter) noexcept{

    }

    void word(const string & parameter) noexcept{

    }

    void ftypeOneMinus(const string & parameter) noexcept{
        //Remove the - and save to type
        this->type.assign(parameter.begin()+1 ,parameter.end());
    }

    void ftypeTwoMinus(const string & parameter) noexcept{
        //Remove the -- and save to type
        this->type.assign(parameter.begin()+2 ,parameter.end());
    }

    map<int,function<void(string parameter)>> type_position_in_regex{
        {1,[this](const string & s){ftypeTwoMinus(s);}},
        {2,[this](const string & s){ftypeOneMinus(s);}},
        {3,[this](const string & s){text(s);}},
        {4,[this](const string & s){expression(s);}},
        {5,[this](const string & s){word(s);}},
    };
};

class command_expr_evaluator
{
public:
    command_expr_evaluator(const json & taskstory);
};


#endif //COMMAND_EXPR_EVALUATOR_H