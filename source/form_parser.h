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


template <typename T>
class answer_branches{
private:
    using answer_t = T;
    using answer_type_t = string;

    optional<int> next_branch_result = std::nullopt;
    const json & question_obj;
    const any & answer;
    function<T(T)> answer_transformation_strategy = [](T s) -> T {return s;};

    optional<int> ranges(const json & ranges_array,int arg){
        for(const auto & [k,v] : ranges_array.items()){
            //Match value to get the "if_branch"
            const auto & range = v["range"];
            const auto & values = range["values"];
            if(arg < values["<"] && arg > values[">"]){
                //Meet the range!
                return range["if_branch"];
            }
        }
        return nullopt;
    }

    optional<int> predefined_boolean_yes_no_affirmative_yes(const json & j, string arg){
        const static unordered_set<string> possible_affirmative{
            "yes","true", "good","fine","affirmative"
        };
        string lowered = arg;
        boost::algorithm::to_lower(lowered);
        if( possible_affirmative.find(lowered) != possible_affirmative.end()){
            return j["true"].get<int>();
        }else if(j.contains("else")){
            return j["else"].get<int>();
        }else{
            return nullopt;
        }
    }
    //C++17, with inline you can use header :O
    const map<string_view, function<optional<int>(const json & current_selector,any)>> kind_branch_t_map{
        {"ranges",[this](const json & j,any s){ return ranges(j,any_cast<int>(s));}},
        {"predefined_boolean_yes_no_affirmative_yes", 
            [this](const json & j,any s){ return predefined_boolean_yes_no_affirmative_yes(j,any_cast<string>(s));}},
    };



    void enroute(const json & j);

public:
    static const int default_brach_error = static_cast<int>(e_branches::ERROR_JSON);
    answer_branches(const json & question_obj, const any & answer, function<T(T)> answer_transformation_strategy_ = nullptr);
    int get_next_branch(){return next_branch_result.value_or(static_cast<int>(e_branches::ERROR_JSON));}
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
    int enroute_json_type(const json & question_obj, const string & answer);
    string get_next(const string & answer);

    optional<json> find_questions_by_id(int id) noexcept;
    
    string form_traverse(const string & answer){
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

    void form_publisher_vars(){}

    void form_command_tasks(){}

    void form_ready(){}

    string form_pipeline(const string & answer){
        form_ready();
        return form_traverse(answer);
        form_publisher_vars();
        form_command_tasks();
    }

public:
    form_parser(const json & j);
    const array<string,5> subsection_names{
        "form","expressions","variables", "configurables", "questions"
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