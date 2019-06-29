#ifndef FORM_PARSER_H
#define FORM_PARSER_H
#include <string>
#include <map>
#include <iostream>
#include <any>
#include <array>
#include <memory>
#include <boost/algorithm/string.hpp>
#include <optional>
#include <unordered_set>
#include "json.hpp"
#include "user.h"
#include "command_expr_evaluator.h"

using namespace std;

enum class branches_ids : int
{
    START = 0,
    FIRST = 1,
    END = -1,
    ERROR_JSON = -9999,
};
using json = nlohmann::json;
using enum_of_json_t = nlohmann::detail::value_t;
using e_branches = branches_ids;


class form_subsection_ADT{
public:
    string section_name;
    form_subsection_ADT(const json & j,string sec_name);
    void print_section();
    map<string,json> section;
};

struct strategy_return{
    int if_branch = static_cast<int>(e_branches::ERROR_JSON); //Id of the the next question
    string taskstory_id;// Name of the taskstory to perform
};

template <typename T>
class answer_branches{
private:
    using answer_t = T;
    using answer_type_t = string;

    std::optional<strategy_return> next_branch_result = std::nullopt;
    const json & question_obj;
    const any & answer;
    function<T(T)> answer_transformation_strategy = [](T s) -> T {return s;};

    std::optional<strategy_return> ranges(const json & ranges_array,int arg){
        for(const auto & [k,v] : ranges_array.items()){
            //Match value to get the "if_branch"
            const auto & range = v["range"];
            const auto & values = range["values"];
            if(arg < values["<"] && arg > values[">"]){
                //Meet the range!
                strategy_return sr{
                    .if_branch = range["if_branch"],
                    .taskstory_id = range["taskstory_id"]
                };
                return sr;
            }
        }
        return nullopt;
    }

    std::optional<strategy_return> predefined_boolean_yes_no_affirmative_yes(const json & j, string arg){
        const static unordered_set<string> possible_affirmative{
            "yes","true", "good","fine","affirmative"
        };
        string lowered = arg;
        boost::algorithm::to_lower(lowered);
        if( possible_affirmative.find(lowered) != possible_affirmative.end()){
            return strategy_return{
                j["true"].get<int>(),
                j["taskstory_id"]
            };
        }else if(j.contains("else")){
            return strategy_return{
                j["else"].get<int>(),
                j["taskstory_id"]
            };
        }else{
            return nullopt;
        }
    }
    //C++17, with inline you can use header :O
    const map<string_view, function<std::optional<strategy_return>(const json & current_selector,any)>> kind_branch_t_map{
        {"ranges",[this](const json & j,any s){ return ranges(j,any_cast<int>(s));}},
        {"predefined_boolean_yes_no_affirmative_yes", 
            [this](const json & j,any s){ return predefined_boolean_yes_no_affirmative_yes(j,any_cast<string>(s));}},
    };

    void enroute(const json & j);

public:
    static const int default_brach_error = static_cast<int>(e_branches::ERROR_JSON);
    answer_branches(const json & question_obj, const any & answer, function<T(T)> answer_transformation_strategy_ = nullptr);
    strategy_return get_next_branch(){return next_branch_result.value();}
    void print_out();
};

class taskstory{
    string command;
    string name;
};

class taskstorys{
    int branch_id;
    vector<taskstory> taskstory_;
};

class questions{
    int id;
    string question;
    string answer_tag;
    string type_user_input;
    vector<string> mode;
    
    map<int,taskstory> taskstorys;
    questions(const json & j);
};

struct next_question_data
{
    string question_str;
    const json taskstory_json;
};
//This class handles the formation of a executable machine of states for the user answers flow, and its correct storage and publish
//Which souns like a to much from a point of design, but lets refactor this ion the future
//Lets try to keep us from add more indirection, to find a more direct and concise solution
class form_parser{
private:
    const json & j;
    map<string,unique_ptr<form_subsection_ADT>> subsections;
    map<string,unique_ptr<form_subsection_ADT>> discover;
    int current_id = static_cast<int>(e_branches::FIRST);
    string current_answer;
    int next_branch_id = static_cast<int>(e_branches::START);
    strategy_return enroute_json_type(const json & question_obj, const string & answer);
    next_question_data get_next(const string & answer);

    std::optional<json> find_questions_by_id(int id) noexcept;
    
    next_question_data form_traverse(const string & answer){
        //int id = current_id;
        cout << "A: " << answer << endl;
        return get_next(answer);
    }

    inline bool is_traversable_id(int id){
        if(id != static_cast<int>(e_branches::ERROR_JSON) ) return true;
        else {return false;}
    }

    const string get_initial_ansewer()noexcept{
        return find_questions_by_id(static_cast<int>(e_branches::FIRST)).value()["question"].get<string>();
    }

    map<string,json> variables;

    void form_publisher_vars(){
        for(auto & section : {"variables","form" ,"bindings"}){
            for (auto [k,v]: subsections[section]->section){
                variables[k] = v;
                cout << k << v << endl;
            }
        }
    }

    void form_ready(){}

    void perform_taskstory(const json & taskstory){

        command_expr_evaluator cee (taskstory, variables);
        //cout << taskstory.dump(4) << endl;
    }

    void user_import_preferences(){
        variables["user.user"] = "carloscbl";

    }

    string form_pipeline(const string & answer){
        //The follow order should be consider
        //1-Ready to get the bare minimum to run
        //2-Publish every preavailable variable
        //3-Traverse once and Get the results, tasktories(commands)
        //4-Execute the commands, via interpreter or direct
        //5-Send back the next question
        form_ready();
        auto next_question = form_traverse(answer);
        form_publisher_vars();

        user_import_preferences(); //This overrides default form vars, that are configurables
        
        if(next_question.taskstory_json != json::value_t::null){
            perform_taskstory(next_question.taskstory_json);
        }
        
        return next_question.question_str;
    }

public:
    form_parser(const json & j);
    const array<string,5> subsection_names{
        "form","bindings","variables", "configurables", "questions"
    };
    const string get_name(){
        return subsections["form"]->section["form.name"].get<string>();
    }
    void test_form_run(){
        //Cicle different answers in order
        const string Q = "Q: ";
        cout << Q << get_initial_ansewer() << endl;
        cout << Q << form_pipeline("YES") << endl;
        cout << Q << form_pipeline("25") << endl;
    }
    void form_run(user user){
        //Good idea to thread pool this call 
        //Here is the point of concurrence, once a form is readed and loaded, and a user is responding questions
        string answer = fetch_and_send_out_first_answer();
        form_pipeline(answer);
    }
    string fetch_and_send_out_first_answer(){
        return "";
    }
};


#endif //FORM_PARSER_H