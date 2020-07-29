#ifndef APP_PARSER_H
#define APP_PARSER_H

#include <string>
#include <map>
#include <iostream>
#include <any>
#include <functional>
#include <array>
#include <memory>
#include <boost/algorithm/string.hpp>
#include <optional>
#include <typeindex>
#include <typeinfo>
#include <unordered_set>
#include <string_view>
#include "json.hpp"

using std::any;
using std::any_cast;
using std::string;
using std::optional;
using std::function;
using std::map;
using std::vector;
using std::unique_ptr;
using std::make_unique;

// if_branch 
enum class if_branch : int
{
    RESTART = 0,
    FIRST = 1,
    END = -1,
    EARLY_END = -2,
    UNIMPLEMENTED = -3,
    USER_INPUT_ERROR = -4,
    NOT_SUPPORTED = -5,
    ERROR_JSON = -9999,
};

using json = nlohmann::json;
using enum_of_json_t = nlohmann::detail::value_t;
using e_branches = if_branch;


class form_subsection_ADT{
public:
    string section_name;
    form_subsection_ADT(const json & j,string sec_name);
    void print_section();
    map<string,json> section;
};

struct strategy_return{
public:
    int if_branch = static_cast<int>(e_branches::ERROR_JSON); //Id of the the next question
    optional<string> taskstory_id;// Name of the taskstory to perform
    json validated_user_input;
};

template <typename T>
class answer_branches{
private:
    using answer_t = T;
    using answer_type_t = string;

    std::optional<strategy_return> next_branch_result = std::nullopt;
    const json & question_obj;
    const std::any & answer;
    function<T(T)> answer_transformation_strategy = [](T s) -> T {return s;}; // General answer conversion strategy

    std::optional<strategy_return> ranges(const json & ranges_array,int arg) const noexcept;
    std::optional<strategy_return> predefined_boolean_yes_no_affirmative_yes(const json & j, string arg)const noexcept;
    std::optional<strategy_return> custom (const json & j, string arg)const noexcept;
    std::optional<strategy_return> any_strategy (const json & j, string arg)const noexcept;
    std::optional<strategy_return> any_strategy (const json & j,json  arg)const noexcept;

    //C++17, with inline you can use header :O
    const map<std::string_view, function<std::optional<strategy_return>(const json & current_selector, std::any )>> kind_branch_t_map{
        {"custom",[this](const json & j,std::any s){ return custom(j,std::any_cast<string>(s));}},  // String mandatory
        {"ranges",[this](const json & j,std::any s){ return ranges(j,std::any_cast<int>(s));}},     // int mandatory
        {"any",[this](const json & j,std::any s)   { 
                if (std::type_index(typeid(json)) == std::type_index(s.type())){
                    return any_strategy(j,std::any_cast<json>(s));
                }
                return any_strategy(j,std::any_cast<string>(s));
            }}, // can be any type mandatory
        {"predefined_boolean_yes_no_affirmative_yes", 
            [this](const json & j,std::any s){ return predefined_boolean_yes_no_affirmative_yes(j,std::any_cast<string>(s));}}, // String mandatory
    };

    void enroute(const json & j);

public:
    static const int default_brach_error = static_cast<int>(e_branches::ERROR_JSON);
    answer_branches(const json & question_obj, const std::any & answer, function<T(T)> answer_transformation_strategy_ = nullptr);
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

struct next_question_data_and_taskstory_input
{
    json current_question_obj; // Holds the solved question and to be excuted tasktories
    string next_question_text; // result of the state machine to be the next question text to show to the user
    json raw_taskstory;
    string taskstory_name; // Name of the taskstory branch to be executed
    map<string,json> form_variables; // Variables of the context user + app, to be used on expansion of taskstory
    json user_input; // This will be passed but have no use for now maybe in future
    unique_ptr<json> expanded_taskstory = nullptr;
};


//This class handles the formation of a executable machine of states for the user answers flow, and its correct storage and publish
//Which souns like a to much from a point of design, but lets refactor this ion the future
//Lets try to keep us from add more indirection, to find a more direct and concise solution
struct form_state{
    uint64_t id = 0;
    int current_id = static_cast<int>(e_branches::FIRST);;
    string current_answer;
    int next_branch_id = static_cast<int>(e_branches::RESTART);;
};

void to_json(nlohmann::json& new_json, const form_state& ref_state);
void from_json(const nlohmann::json& ref_json, form_state& new_state);

class app_parser{
private:
    const json & j;
    map<string,unique_ptr<form_subsection_ADT>> subsections;
    map<string,unique_ptr<form_subsection_ADT>> discover;
    int current_id = static_cast<int>(e_branches::FIRST);
    string current_answer;
    int next_branch_id = static_cast<int>(e_branches::RESTART);
    map<string,json> variables;

    strategy_return enroute_json_type(const json & question_obj, const json & answer);

    unique_ptr <next_question_data_and_taskstory_input> get_next(const json & answer);

    std::optional<json> find_questions_by_id(int id) const noexcept;
    
    unique_ptr <next_question_data_and_taskstory_input> form_traverse(const json & answer_input){
        //int id = current_id;
        //cout << "A: " << answer << endl;
        return get_next(answer_input);
    }

    inline bool is_traversable_id(int id){
        if(id != static_cast<int>(e_branches::ERROR_JSON) ) return true;
        else {return false;}
    }

    void form_publisher_vars(){
        for(auto & section : {"form"}){
            for (auto [k,v]: subsections[section]->section){
                variables[k] = v;
                //cout << k << v << endl;
            }
        }
    }

    void form_ready(){}

    void user_import_preferences(){
        variables["user.user"] = "carlos";
    }

public:

    unique_ptr <next_question_data_and_taskstory_input> form_next_in_pipeline(const json & answer_input){
        //The follow order
        form_ready();
        auto next_question = form_traverse(answer_input);
        form_publisher_vars();

        user_import_preferences(); //This overrides default form vars, that are configurables
        
        next_question->form_variables = this->variables;
        return next_question;
    }
    app_parser(const json & j);
    app_parser(const json & j,const form_state & fs);
    const vector<string> subsection_names{
        "form", "questions"
    };

    const string get_name(){
        return subsections["form"]->section["form.name"].get<string>();
    }
    
    unique_ptr <next_question_data_and_taskstory_input> get_initial_question() const noexcept{
        auto initial_question = make_unique<next_question_data_and_taskstory_input> ();
        initial_question->next_question_text = find_questions_by_id(static_cast<int>(e_branches::FIRST)).value()["question"].get<string>();
        return initial_question;
    }
    unique_ptr <next_question_data_and_taskstory_input> get_current_question() const noexcept{
        auto initial_question = make_unique<next_question_data_and_taskstory_input> ();
        if(static_cast<int>(e_branches::RESTART) == next_branch_id ){
            return get_initial_question();
        }
        initial_question->next_question_text = find_questions_by_id(next_branch_id).value()["question"].get<string>();
        return initial_question;
    }
    unique_ptr<form_state> get_state() const noexcept{
        auto ptr = make_unique<form_state>();
        ptr->current_id = this->current_id;
        ptr->current_answer = this->current_answer;
        ptr->next_branch_id = this->next_branch_id;
        return ptr;
    }
};
#endif //APP_PARSER_H