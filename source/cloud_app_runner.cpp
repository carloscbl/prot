
#include "cloud_app_runner.h"
#include "time_determinator.h"
#include "db_ops.hpp"
#include "time_utils.hpp"
#include "spdlog/spdlog.h"
// SPDLOG_INFO("Welcome to spdlog!");
// SPDLOG_ERROR("Welcome to spdlog!");

cloud_app_runner::cloud_app_runner(user & user_, app &app_)
    : user_(user_),
      app_(app_),
      user_apps_id(get_user_apps_id(this->user_.get_id() , this->app_.get_id()))
{

}

cloud_app_runner::cloud_app_runner(user & user_, app &app_, uint64_t user_apps_id)
    : user_(user_),
      app_(app_),
      user_apps_id(user_apps_id)
{

}

bool cloud_app_runner::store_qa_history_status( json addition) const {
    measure_execution_raii(__FUNCTION__);
    // need to detect an already "done": true, maybe we are reconfiguring
    // So we need to detect it and delete it -> always
    auto instl = db_op::read_user_instalations( this->user_.get_id(), this->app_.get_id());
    auto tuple = move(*instl.begin());
    json new_qa_history = json::object();
    auto qa_history_it = tuple->find("qa_history");
    if( qa_history_it != tuple->end() && !tuple->is_null()){
        // exists so we use it
        new_qa_history = qa_history_it.value();
    }
    new_qa_history.erase("done");

    auto add_history = addition.find("history");
    if(add_history != addition.end() ){
        for (auto &&i : add_history.value())
        {
            new_qa_history["history"].push_back(i);
        }
        addition.erase("history");
    }
    new_qa_history.merge_patch(addition);

    auto is_updated = db_op::update_user_instalations(this->user_.get_id(), this->app_.get_id(), new_qa_history);
    return is_updated;
}

// In the future maybe we should separate different cloud runners, interactive and projected, with common deps
void cloud_app_runner::projected_run(const json &history, size_t fw_projections) noexcept
{
    //projection 0 out of loop
    this->projected_scheduled_tasks = make_unique<vector<vector<task_t>>>();
    this->programatic_run_injecting_history_answers(history, 0);
    this->projected_scheduled_tasks->push_back(*this->scheduled_tasks);
    this->scheduled_tasks->clear();

    for (size_t fw_pj = 1; fw_pj <= fw_projections; fw_pj++)
    {
        auto prev_period_scheduled = get_prev_period_scheduled(fw_pj);
        this->programatic_run_injecting_history_answers(history, fw_pj, prev_period_scheduled);
        if(!this->scheduled_tasks.has_value()){
            continue;
        }
        this->projected_scheduled_tasks->push_back(*this->scheduled_tasks);
        this->scheduled_tasks->clear();
    }
}

const optional<json> cloud_app_runner::programatic_run_injecting_history_answers(const json &history, size_t fw_projection, const std::vector<ischeduler::task_t> * prev_period_scheduled) noexcept
{
    // measure_execution_raii(__FUNCTION__);
    unique_ptr<app_state> state = make_unique<app_state>();
    app_parser ap(app_.get_json(), *state); 
    
    for (auto && qa_obj : history)
    {
        // measure_execution_raii(__FUNCTION__);
        unique_ptr<next_question_data_and_taskstory_input> response;
        response = ap.app_next_in_pipeline(qa_obj["user_answer"]);
        if(response->next_question_text == "END"){
            return nullopt;
        }

        if (response->taskstory_name.empty())
        {
            continue;
        }
        ////////////////////////////////////////////////////////
        //Build schedulers and taskers for the given taskstory
        ////////////////////////////////////////////////////////
        try
        {
            expand_taskstory_t ep (*response);
            ep.expand_and_set();
        }
        catch(const std::exception& e)
        {
            SPDLOG_ERROR("Failed taskstory expansion {} ", e.what());
        }
        
        schedule_taskstory(*response, fw_projection, prev_period_scheduled);
        continue;
    }
    return json();
}

const json cloud_app_runner::run(const json &request_json) noexcept
{
    measure_execution_raii(__FUNCTION__);
    const auto &state = fetch_next_session();
    this->m_session_id = state->id;
    app_parser ap(app_.get_json(), *state); //,*state
    unique_ptr<next_question_data_and_taskstory_input> response;
    // auto current_question = ap.get_current_question();
    if (request_json.is_null()) // Get, for get current state, for example for resume questionary
    {
        response = ap.get_current_question();
    }
    else
    {
        response = ap.app_next_in_pipeline(request_json["answer"]);
    }
    if(response->next_question_text == "END"){
        delete_session(state->id);
        // App qa_history done!
        json add_qa_history = {
            {"done",true},
            {"history", { 
                    { 
                        {"question", response->current_question_obj["question"].get<string>()},
                        {"user_answer", request_json["answer"]}
                    } 
                }
            },
        };
        store_qa_history_status(add_qa_history);
    }else{
        update_session(state->id, *ap.get_state());
        if(!request_json.is_null()){
            json qa = {
                { 
                    {"question", response->current_question_obj["question"].get<string>()},
                    {"user_answer", request_json["answer"]}
                }
            };
            json add_qa_history = {
                //insert here the Q/A structure
                {"history", qa },
            };
            store_qa_history_status(add_qa_history);
        }
    }

    json response_j;
    response_j["next_question"] = response->next_question_text;
    response_j["data_type"] = response->current_question_obj["type_user_input"];
    response_j["type_details"] = response->current_question_obj["type_details"];
    //TODO: Add pass the taskstory and the parsed variables to the user scheduler

    if (response->taskstory_name.empty())
    {
        return response_j;
    }
    
    ////////////////////////////////////////////////////////
    //Build schedulers and taskers for the given taskstory
    ////////////////////////////////////////////////////////
    try
    {
        expand_taskstory_t ep (*response);
        ep.expand_and_set();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    if(schedule_taskstory(*response))
    {
        // if we have successfully completed the process means we have to apply domain logic
        // Delete same user_app tasks already scheduled
        // this->user_apps_id // To search for the apps
        // this->m_session_id  // To search for the apps
        auto tasks_to_delete =  search_tasks_by_not_this_session_id(this->m_session_id, this->user_apps_id);
        vector<uint64_t> to_delete_ids;
        to_delete_ids.reserve(tasks_to_delete.size());
        std::for_each(tasks_to_delete.cbegin(),tasks_to_delete.cend(), [&to_delete_ids](const auto & pair){
            fmt::print("storing to deleting task {}",pair.first);
            to_delete_ids.push_back( pair.first );
        });
        delete_task(to_delete_ids);
    }

    return response_j;
}

struct failure_report_t{
    vector<task_t> failures_report;
    task_t no_margin_invalidation;
};

// Would be fantastic the static dependency injection here
optional<days> get_next_period_by_prev_task(string task_id, const vector<task_t> * prev_period_scheduled)
{
    if(!prev_period_scheduled){
        return nullopt;
    }

    auto first_it = std::find_if(prev_period_scheduled->cbegin(),prev_period_scheduled->cend(), [task_id](task_t t) -> bool{
        return t->get_task_id() == task_id ? true : false;
    });
    if(first_it == prev_period_scheduled->cend()){
        return nullopt;
    }
    task_t prev_tsk = (*first_it);
    auto next_start = task_space::next_period_start(prev_tsk);

    auto future = std::chrono::floor<days>(next_start.value());
    auto now = std::chrono::floor<days>(std::chrono::system_clock::now());

    days next_period = std::chrono::floor<days>(future - now);
    return next_period;
}

const vector<task_t> * cloud_app_runner::get_prev_period_scheduled(size_t current_fw_pj) const
{
    if( current_fw_pj == 0 ){
        throw runtime_error("The current projection needs to be >= 1 cannot be used in the first iteration");
    }

    const size_t prev_pj = current_fw_pj - 1 ;
    if(!this->projected_scheduled_tasks || this->projected_scheduled_tasks->size() < prev_pj){
        return nullptr;
    }
    const auto & prev_period_scheduled = (*projected_scheduled_tasks)[prev_pj];
    return &prev_period_scheduled;
}

// DEPRECATE
optional<days> deduce_next_period(const json & j_task, task * task_, optional<std::chrono::time_point<system_clock>> start_from){
    task * tk = task_;
    if( !tk){
        from_json(j_task,*tk);
    }
    days next_period;
    auto future = std::chrono::floor<days>(start_from.value());
    auto now = std::chrono::floor<days>(std::chrono::system_clock::now());
    SPDLOG_DEBUG("days since epoch (future) {}", duration_cast<days>(seconds(future.time_since_epoch())).count());
    SPDLOG_DEBUG("days since epoch (now) {}" ,duration_cast<days>(seconds(now.time_since_epoch())).count());
    
    if (now > future){// Recomputing the next period if already expired
        SPDLOG_DEBUG("Trying to deduce next period...");
        auto ratio_opt = task_space::get_period_ratio(*tk);
        if(!ratio_opt.has_value()){ return nullopt; }
        days ratio = duration_cast<days>(ratio_opt.value());
        days offset = now - future; //24 - 22 = 2 days
        int period_multiplier = floorf64x(offset / ratio) + 1; // floor(2 / 7days) + 1 = 1
        future = (period_multiplier * ratio) + future ;
        if(now > future){
            SPDLOG_ERROR("ERROR Task cannot be schduled in the past! in(DAYS): {}", std::chrono::floor<days>(future - now).count());
            return nullopt;
        }
        SPDLOG_DEBUG("Success deducing!");
    }
    next_period = std::chrono::floor<days>(future - now);
    return next_period;
}

// DEPRECATE
bool cloud_app_runner::schedule_single_task(const json & j_task, optional<std::chrono::time_point<system_clock>> start_from, task * task_) const{
    SPDLOG_DEBUG("## schedule_single_task ##");
    transactional_group_scheduler_RAII provisional_scheduler = this->user_.get_scheduler().get_provisional();
    tasker &tasker_ = static_cast<tasker &>(this->user_.get_tasker());
    auto task_test = create_task_to_schedule(j_task);

    optional<days> projected_next_period_override_start_offset = nullopt;
    if(start_from.has_value()){
        projected_next_period_override_start_offset = deduce_next_period(j_task, task_, start_from);
        if(!projected_next_period_override_start_offset.has_value()){
            SPDLOG_ERROR("WRONG projected_next_period_override_start_offset");
            return false;
        }
    }
    
    time_determinator time_dt(task_test, provisional_scheduler);
    optional<bool> result = time_dt.build( days(0), projected_next_period_override_start_offset );
    if(!result.has_value() || !result.value()){
        SPDLOG_ERROR("Unable to determine slot for task");
        return false;
    }
    tasker_.commit_single_task(move(task_test));
    SPDLOG_DEBUG("COMMITED task");
    return true;
}

task_t cloud_app_runner::create_task_to_schedule(const json & j_task, size_t fw_projection) const{
    task_t task_test = make_shared<task>(j_task.get<task>());
    task_test->inner_json["app_id"] = this->app_.get_id();
    task_test->inner_json["fw_projection"] = fw_projection;
    task_test->set_user(this->user_.get_id());
    task_test->set_user_apps_id(user_apps_id);
    task_test->set_session_id(m_session_id);
    return task_test;
}

void cloud_app_runner::register_runner_scheduled_tasks (std::shared_ptr<std::map<std::string, task_t>> done_tasks){
    if(!done_tasks){
        return;
    }
    optional<vector<task_t>>  current = nullopt;
    if(this->scheduled_tasks.has_value()){
        current = this->scheduled_tasks.value();
    }else{
        current = vector<task_t>();
    }
    for_each(done_tasks->begin(), done_tasks->end(),[&current](const pair<string,task_t>& p){
        current.value().push_back(p.second);
    });
    if(! current.value().empty()){
        this->scheduled_tasks = current;
    }
}

bool cloud_app_runner::schedule_taskstory(next_question_data_and_taskstory_input & response, size_t fw_projection, const std::vector<ischeduler::task_t> * prev_period_scheduled ){

    failure_report_t report;
    const size_t max_it_retry_days = 365;// Max over a year... if not fail ## In the future with octo/quadtree or other strategies we can avoid much better iterations to just a few
    for (size_t retry_day_offset = 0; retry_day_offset < max_it_retry_days; retry_day_offset++)
    {
        transactional_group_scheduler_RAII provisional_scheduler = this->user_.get_scheduler().get_provisional();
        tasker &tasker_ = static_cast<tasker &>(this->user_.get_tasker());
        taskstory_commit_RAII commiter(response.taskstory_name, tasker_);

        bool complete = true;

        for (const auto &v : *response.non_wildcard_expanded_taskstory)
        {
            task_t task_test = create_task_to_schedule(v, fw_projection);
            optional<days> projected_next_period_override_start_offset;
            if(fw_projection){
                projected_next_period_override_start_offset = get_next_period_by_prev_task( task_test->get_task_id(), prev_period_scheduled );
            }
            time_determinator time_dt(task_test, provisional_scheduler, std::nullopt, fw_projection);
            //cout << "checking task: " << task_test->get_task_id() << " day:" << day << endl;
            optional<bool> result = time_dt.build(days(retry_day_offset), projected_next_period_override_start_offset);
            if(result.has_value() ){
                if(result.value()){
                    //task_test->save();
                    tasker_.add_to_group(move(task_test), response.taskstory_name);
                }else{
                    report.failures_report.push_back(task_test);
                    complete = false;
                }
            }else{//Now time to make it fail and control the failure
                report.no_margin_invalidation = task_test;
                return false;
            }
        }

        if(complete){
            auto done_tasks = commiter.get_group();
            register_runner_scheduled_tasks(done_tasks);
            commiter.commit(); //Disolves group && activate the tasks
            provisional_scheduler.commit();
            break;
        }
    }
    apply_wildcards(response, fw_projection, prev_period_scheduled);

    return true;
}

void cloud_app_runner::apply_wildcards(next_question_data_and_taskstory_input & response, size_t fw_projection, const std::vector<ischeduler::task_t> * prev_period_scheduled ){

    failure_report_t report;
    for (const auto &[k,v] : *response.wildcard_expanded_taskstory)
    {
        transactional_group_scheduler_RAII provisional_scheduler = this->user_.get_scheduler().get_provisional();
        tasker &tasker_ = static_cast<tasker &>(this->user_.get_tasker());
        taskstory_commit_RAII commiter(response.taskstory_name, tasker_);
        bool completed_period = true;
        for (const auto & wildcard_task : v){
            task_t task_test = create_task_to_schedule(wildcard_task.get<task>(), fw_projection);
            string task_id = task_test->get_task_id();
            optional<days> projected_next_period_override_start_offset;
            if(fw_projection){
                projected_next_period_override_start_offset = get_next_period_by_prev_task( task_test->get_task_id(), prev_period_scheduled );
            }
            time_determinator time_dt(task_test, provisional_scheduler, k, fw_projection);
            //cout << "checking task: " << task_test->get_task_id() << " day:" << day << endl;
            optional<bool> result = time_dt.build(days(0), projected_next_period_override_start_offset);
            if(result.has_value() ){
                if(result.value()){
                    //task_test->save();
                    // fmt::print("-added_to_group {}\n", task_id);
                    tasker_.add_to_group(move(task_test), response.taskstory_name);
                }else{
                    fmt::print("-Unable to schedule {}\n", task_id);
                    report.failures_report.push_back(task_test);
                    completed_period = false;
                    break;
                }
            }else{//Now time to make it fail and control the failure
                fmt::print("-Failed time determination {}\n", task_id);
                report.no_margin_invalidation = task_test;
                break;
                // return false;
            }
        }
        if(completed_period){
            auto done_tasks = commiter.get_group();
            register_runner_scheduled_tasks(done_tasks);
            commiter.commit(); //Disolves group && activate the tasks
            provisional_scheduler.commit();
        }
    }
}

shared_ptr<app_state> cloud_app_runner::get_session() const noexcept
{
    //This needs to come from db
    const auto &&state = read_session(this->user_.get_id() , this->app_.get_id() );
    if (!state){
        return create_session(this->user_.get_id() , this->app_.get_id() );
    }
    return state;
}

shared_ptr<app_state> cloud_app_runner::fetch_next_session() const noexcept
{
    auto session = get_session();
    if(session->next_branch_id < 0){
        return create_session(this->user_.get_id() , this->app_.get_id() );
    }
    return session;
}

