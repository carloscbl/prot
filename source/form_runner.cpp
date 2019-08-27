
#include "form_runner.h"

form_runner::form_runner(shared_ptr<user> user_, form_t &form_, command_processor & cp)
:user_(user_),
form_(form_),
cp(cp)
{

    //fp = make_unique<form_parser>(state);
    //Create thread
    //Check serialiced session or create a new one
    //Perform or traverse the questionary

    //run();
    // thread th([this](){
    // });
}

form_runner::~form_runner() {}

string form_runner::get_unique_id_session() const noexcept
{
    return user_->get_name() + form_.name;
}

//FIX: this is a mess composing responses...
const json form_runner::run(const json &request_json) noexcept
{
    const auto &state = get_session();
    form_parser fp(form_.get_json(), *state); //,*state
    unique_ptr<next_question_data> response;
    if (request_json.is_null())
    {
        response = fp.get_initial_question();
    }
    else
    {
        cout << request_json.dump(4) << endl;
        response = fp.form_next_in_pipeline(request_json["answer"]);
    }
    user_running_forms[get_unique_id_session()] = fp.get_state();
    json response_j;
    response_j["next_question"] = response->question_str;
    //TODO: Add pass the taskstory and the parsed variables to the user scheduler

    if (response->taskstory_json.empty()){
        return response_j;
    }

    if (response->taskstory_json.size() == 1)
    {
        command_expr_evaluator command(response->taskstory_json.cbegin().value()["command"], response->form_variables);

        auto co = command.get_command();
        string strcommand = co.render();

        if (strcommand.empty())
        {
            cout << "bad parse of command" << endl;
        }
        else
        {
            provisional_scheduler_RAII provisional = this->user_->get_scheduler().get_provisional();
            provisional.print_out();
            tasker & tasker_ = static_cast<tasker&>(this->user_->get_tasker());
            //Group composed
            taskstory_commit_RAII commiter(response->taskstory_name, static_cast<tasker&>(tasker_));
            task_t created_task_by_command;
            ///// Now start send commands
            cp.perform_command(strcommand);
            //Refresh variables
            //TODO: Define, response->form_variables[response->taskstory_name] = 
            //Loop

            ///// End send commands
            /////Last chance to gather all grouped
            auto group = commiter.get_group();
            if(group){
                created_task_by_command = group->at(0);
                cout << created_task_by_command->id << endl;
            }
            /////
            commiter.commit(); //Disolves group && activate the tasks
            provisional.add_single(move(created_task_by_command));
        }
    }else{
        provisional_scheduler_RAII provisional = this->user_->get_scheduler().get_provisional();
        tasker & tasker_ = static_cast<tasker&>(this->user_->get_tasker());
        taskstory_commit_RAII commiter(response->taskstory_name, tasker_);
        bool any_wrong = false;
        
        for ( const auto & [k,v] : response->taskstory_json.items())
        {
            cout << v.dump(4) << endl;
            /*
            As we need to be able to reload the task, we need to provide to the task its own parsed command, but we will make it via render from the task
            but we need to transform dual param from command expr evaluator, to string as the params ar given bi
            */
            command_expr_evaluator command(v["command"].get<string>(), response->form_variables);
            auto co = command.get_command();
            string strcommand = co.render();
            
            if (strcommand.empty())
            {
                cout << "bad parse of command" << endl; //We should stop all
                any_wrong = true;
                break;
            }
            else
            {
                provisional.print_out();
                string tag_name = v["task_tag"].get<string>();

                cp.perform_command(strcommand + " -g " + response->taskstory_name + " -t " + tag_name);

                task_t last_command_made = commiter.get_group()->at(tag_name);
                json publish = *last_command_made;
                response->form_variables[tag_name] = publish;
            }
        }
        task_t created_task_by_command;
        auto group_vector = commiter.get_group();
        if(group_vector && !any_wrong){
            queue<task_t> taskstory;
            for(auto & [k,v]: response->taskstory_json.items()){
                string tag = v["task_tag"].get<string>();
                taskstory.push(group_vector->at(tag));
            }

            /////Add all at the same time
            if(provisional.add_group(move(taskstory))){
                commiter.commit(); //Disolves group && activate the tasks
            }
        }
    }

    return response_j;
}

task_t form_runner::command_to_task(string & taskstory_command, variables_t & variables)
{
    //We need to be able to get a solved command with the variables, and then publish the new ones from
    command_expr_evaluator expr(taskstory_command, variables);

    auto co = expr.get_command();
    string strcommand = co.render();

    if (strcommand.empty())
    {
        cout << "bad parse of command" << endl;
    }
    else
    {
        //Here we send the command string that returns our task, here we relay in the implementation for getting new task via command
    }
    return task_t();
}

shared_ptr<form_state> form_runner::get_session() const noexcept
{
    auto session_id = get_unique_id_session();
    const auto &&state = form_runner::user_running_forms.find(session_id);
    if (state != form_runner::user_running_forms.end())
    {
        return state->second;
    }
    else
    {
        auto new_state = make_shared<form_state>();
        form_runner::user_running_forms[session_id] = new_state;
        return new_state;
    }
}