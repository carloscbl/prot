#include "app_parser.h"


app_parser::app_parser(const json &j) : j(j)
{

    subsections = map<string, unique_ptr<form_subsection_ADT>>();
    for (const auto &sect : subsection_names)
    {
        subsections[sect] = make_unique<form_subsection_ADT>(j, sect);
    }

    for (const auto &[k, v] : j.items())
    {
        const auto &it = find(subsection_names.begin(), subsection_names.end(), k);
        if (it != subsection_names.end())
        {
            continue;
        }
        discover[k] = make_unique<form_subsection_ADT>(j, k);
    }
}
app_parser::app_parser(const json &j, const app_state &fs) : app_parser(j)
{
    current_answer = fs.current_answer;
    current_id = fs.current_id;
    next_branch_id = fs.next_branch_id;
}

template <typename T>
strategy_return make_get_next_branch(const json &question_obj, any arg)
{
    return answer_branches<T>(question_obj, arg).get_next_branch();
}

std::unordered_map<std::type_index, function<strategy_return(const json &, any)>> type_names{
    {std::type_index(typeid(int)), make_get_next_branch<int>},
    {std::type_index(typeid(string)), make_get_next_branch<string>},
    {std::type_index(typeid(double)), make_get_next_branch<double>},
    {std::type_index(typeid(float)), make_get_next_branch<float>},
    {std::type_index(typeid(json)), make_get_next_branch<json>},
};

strategy_return app_parser::enroute_json_type(const json &question_obj, const json & answer_input)
{

    string expected_answer_type = question_obj["type_user_input"].get<string>();
    if  (expected_answer_type != answer_input["type"].get<string>()){
        return strategy_return{};
    }

    std::any converted_answer;
    auto conversor = prot::conversors_map.find(expected_answer_type);
    if (conversor != prot::conversors_map.end())
    {
        converted_answer = conversor->second(answer_input); // For a matrix this will result in the very same input json
        if (converted_answer.has_value())
        {
            std::type_index index(converted_answer.type());// json, string, int etc
            auto func = type_names.find(index);
            if (func != type_names.end())
            {
                auto result = func->second(question_obj, converted_answer);
                return result;
            }
        }
    }
    return strategy_return{};
}

unique_ptr <next_question_data_and_taskstory_input> app_parser::get_next(const json & answer_input)
{
    json question;
    if (this->next_branch_id == static_cast<int>(e_branches::RESTART))
    {
        //Means is first time
        question = find_questions_by_id(static_cast<int>(e_branches::FIRST)).value();
    }
    else
    {
        question = find_questions_by_id(this->next_branch_id).value();
    }
    auto strategy_returned = enroute_json_type(question, answer_input);
    this->next_branch_id = strategy_returned.if_branch;
    //cout << "taskstory " << strategy_returned.taskstory_id << endl;
    string next_question;
    if(this->next_branch_id < 0){
        next_question = "END"; //TODO get why END
    }else{
        auto nextQ = find_questions_by_id(this->next_branch_id);
        if (nextQ.has_value())
        {

            next_question = nextQ.value()["question"].get<string>();
        }
        else
        {
            next_question = "END"; // UNIMPLEMENTED
        }
    }
    json taskstory_ = json::value_t::null;

    if (strategy_returned.taskstory_id.has_value())
    {
        taskstory_ = question["taskstories"][strategy_returned.taskstory_id.value()];
    }

    unique_ptr <next_question_data_and_taskstory_input> nqd = make_unique<next_question_data_and_taskstory_input>();
    nqd->next_question_text = next_question;
    nqd->taskstory_name = strategy_returned.taskstory_id.value_or("");
    nqd->raw_taskstory = taskstory_;
    nqd->user_input = strategy_returned.validated_user_input;
    nqd->current_question_obj = question;
    return nqd;
}

std::optional<json> app_parser::find_questions_by_id(int id) const noexcept
{
    for (const auto &[k, v] : this->subsections.at("questions")->section)
    {
        if (v["id"] == id)
        {
            return v;
        }
    }
    return std::nullopt;
}

template <typename T>
answer_branches<T>::answer_branches(const json &question_obj, const any &answer, function<T(T)> answer_transformation_strategy_)
    : question_obj(question_obj),
      answer(answer)
{
    if (answer_transformation_strategy_ != nullptr)
    {
        this->answer_transformation_strategy = answer_transformation_strategy_;
    }
    enroute(question_obj["answers_branches"]);
}

form_subsection_ADT::form_subsection_ADT(const json &j, string sec_name) : section_name(sec_name)
{
    const auto &section_json = j[section_name];
    for (auto &[key, value] : section_json.items())
    {
        section[key] = value;
    }
}

void form_subsection_ADT::print_section()
{
    cout << "Section: " << this->section_name << endl;
    for (const auto &[k, v] : section)
    {
        cout << k << "--" << v << endl;
    }
}

template <typename T>
void answer_branches<T>::print_out()
{
    cout
        << any_cast<T>(this->answer) << " : "
        << this->next_branch_result.value_or(default_brach_error)
        << endl;
}

template <>
void answer_branches<string>::enroute(const json &j)
{
    //for mapped strategies
    for (const auto &[k, v] : j.items())
    {
        //We test for every structure in order
        const auto &it = kind_branch_t_map.find(k); //matching json structure
        if (it == kind_branch_t_map.end())
        {
            cout << "non existing map implementation! for kind_branch_t_map and: " << k << endl;
            return;
        }

        const auto & opt = it->second(v, any(this->answer));
        //cout << "the next is: " << opt.value().taskstory_id << endl;
        if (opt.has_value())
        {

            next_branch_result = opt;
            return;
        }
    }
}

template <typename T>
void answer_branches<T>::enroute(const json &j)
{
    //for mapped strategies
    for (const auto &[k, v] : j.items())
    {
        //We test for every structure in order
        const auto &it = kind_branch_t_map.find(k); //matching json structure
        if (it == kind_branch_t_map.end())
        {
            cout << j.dump(4) << endl;
            cout << "Not implemented or not finded implementation" << endl;
            return;
        }

        const auto &opt = it->second(v, any(this->answer));
        //cout << "the next is: " << opt.value().if_branch << endl;
        if (opt.has_value())
        {
            next_branch_result = opt;
            break;
        }
    }
}

optional<string> get_taskstory_id(const json &j)
{
    const auto &task_id = j.find("taskstory_id");
    if (task_id != j.end() && !task_id.value().is_null())
    {
        return task_id.value();
    }
    else
    {
        return nullopt;
    }
}

int get_if_branch(const json &j)
{
    const auto &branch_id = j.find("if_branch");
    if (branch_id != j.end())
    {
        return branch_id.value();
    }
    else
    {
        return static_cast<int>(e_branches::ERROR_JSON);
    }
}

template <typename T>
std::optional<strategy_return> answer_branches<T>::predefined_boolean_yes_no_affirmative_yes(const json &j, string arg)
    const noexcept
{
    const static unordered_set<string> possible_affirmative{
        "yes", "true", "good", "fine", "affirmative"};
    string lowered = arg;
    boost::algorithm::to_lower(lowered);
    if (possible_affirmative.find(lowered) != possible_affirmative.end())
    {
        return strategy_return{
            .if_branch = j["true"].get<int>(),
            .taskstory_id = get_taskstory_id(j)
        };
    }
    else if (j.contains("else"))
    {
        return strategy_return{
            .if_branch = j["else"].get<int>(),
            .taskstory_id = get_taskstory_id(j)
        };
    }
    else
    {
        return nullopt;
    }
}

template <typename T>
std::optional<strategy_return> answer_branches<T>::ranges(const json &ranges_array, int arg)
    const noexcept
{
    for (const auto &[k, v] : ranges_array.items())
    {
        //Match value to get the "if_branch"
        const auto &range = v["range"];
        const auto &values = range["values"];
        if (arg < values["<"] && arg > values[">"])
        {
            //Meet the range!
            strategy_return sr{
                .if_branch = range["if_branch"],
                .taskstory_id = get_taskstory_id(range)};
            return sr;
        }
    }
    return nullopt;
}

template <typename T>
std::optional<strategy_return> answer_branches<T>::custom(const json &j, string arg)
    const noexcept
{
    //////////#### CUSTOM SELECTORS ####/////////
    //const auto & modulated_answer = any_cast<string>(this->answer); // = answer_transformation_strategy(answer);
    const auto &modulated_answer = arg;
    //Match custom selectors
    const auto &match = j.find(modulated_answer);
    if (match != j.end())
    {
        //"Yes":2, "YEEEESSS":2 , "No":5
        //cout << j.dump(4) << endl;
        //cout << match.value() << endl;
        strategy_return sr{
            .if_branch = get_if_branch(match.value()),
            .taskstory_id = get_taskstory_id(j)
        };
        return sr;
    }
    else if (j.find("else") != j.end())
    {
        return strategy_return{
            .if_branch = get_if_branch(j["else"]),
            .taskstory_id = get_taskstory_id(j)
        };
    }
    else
    {
        return std::nullopt;
    }
}

template <typename T>
std::optional<strategy_return> answer_branches<T>::any_strategy(const json &j, string arg)
    const noexcept
{
    const auto & modulated_answer = arg;
    optional<string> taskstory_id = get_taskstory_id(j);
    if (!modulated_answer.empty())
    {
        return strategy_return{
            j["true"].get<int>(),
            taskstory_id
        };
    }
    else if (j.contains("else"))
    {
        return strategy_return{
            j["else"].get<int>(),
            taskstory_id
        };
    }
    else
    {
        return nullopt;
    }
}

bool validate_matrix_input (const json & matrix_input){
    // things to do with the matrix thing
    // We need to influence the taskstory TODO: pass it by strategy return
    // We need to extract the info, validate and group
    auto cols = matrix_input["metadata"]["cols"].get<size_t>();
    auto rows = matrix_input["metadata"]["rows"].get<size_t>();
    if(rows != matrix_input["data_input_from_user"].size()){ return false; }
    for(auto [k,v] :  matrix_input["data_input_from_user"].items()){
        if(cols != v.size()){ return false; }
    }
    return true;
}

bool select_type(const json & input){
    auto type = input["type"].get<string>();
    if (type =="MATRIX"){
        return validate_matrix_input(input);
    }
    else if (type =="VECTOR")
    {
        /* code */
    }
    else if (type =="SELECT")
    {
        /* code */
    }
    else if (type =="DURATION")
    {
        /* code */
    }
    else{

    }
    return false;
}


template <typename T>
std::optional<strategy_return> answer_branches<T>::any_strategy(const json &j, json arg)
    const noexcept
{
    // So here we should process all the need for the whole set of possibilities for a json structure...
    if ( !select_type(arg) ){ return nullopt; }
    optional<string> taskstory_id = get_taskstory_id(j);
    if (!arg.empty())
    {
        return strategy_return{
            j["true"].get<int>(),
            taskstory_id,
            arg
        };
    }
    else if (j.contains("else"))
    {
        return strategy_return{
            j["else"].get<int>(),
            taskstory_id
        };
    }
    else
    {
        return nullopt;
    }
}


void to_json(nlohmann::json& new_json, const app_state& ref_state){
    new_json = nlohmann::json{
        {"current_id",     ref_state.current_id},
        {"current_answer",     ref_state.current_answer},
        {"next_branch_id",     ref_state.next_branch_id},
    };
}

void from_json(const nlohmann::json& ref_json, app_state& new_state){
    ref_json.at("current_id").get_to(new_state.current_id);
    ref_json.at("current_answer").get_to(new_state.current_answer);
    ref_json.at("next_branch_id").get_to(new_state.next_branch_id);
}