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
    dual_param(const std::smatch & match){
        if(match[0].matched){
            for (auto && [k,v] : type_position_in_regex)
            {
                if(match)
            }
            
            
            cout << "DOUBLE- " << match.str(1) ;
            cout << "SINGLE- " << match.str(2) ;
            cout << "''- " << match.str(3) ;
            cout << "{}- " << match.str(4) ;
            cout << "WORD- " << match.str(5) << endl;
            //cout << i.str() << endl;

        }
    }

    void expression(string parameter){

    }
    void text(string paramater){

    }
    void word(string paramater){

    }
    void ftype(string paramater){

    }

    map<int,function<void(string parameter)>> type_position_in_regex{
        {1,[this](string s){ftype(s);}},
        {2,[this](string s){ftype(s);}},
        {3,[this](string s){text(s);}},
        {4,[this](string s){expression(s);}},
        {5,[this](string s){word(s);}},
    };


};

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