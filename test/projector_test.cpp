#include "Catch2/catch.hpp"
#include "json.hpp"
#include "user.h"
#include "app.h"
#include "cloud_app_runner.h"
#include "db_ops.hpp"

#include <fstream>
#include <iostream>
#include "json.hpp"
// #include "time_utils.hpp"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "app_projector.h"
#include "prot_specifics.hpp"
#include "time_utils.hpp"
namespace fs = boost::filesystem;
using namespace fs;

using user_t = unique_ptr<user>;
using app_t = unique_ptr<app>;
using json_t = unique_ptr<json>;


inline app_t get_app(const string & user_id, string name = "test Daily workout history"){
    auto name_file = name;
    boost::filesystem::path full_path(boost::filesystem::current_path());
    boost::erase_all(name_file, " ");
    boost::to_lower(name_file);
    string path = "../test/iofiles_test/test_app_"+name_file+".json";
    std::ifstream test_app_json_file(path, std::fstream::in);
    json test_app_json;
    test_app_json_file >> test_app_json;
    app_t app_ = create_app(test_app_json, user_id);
    if(!app_){
        app_ = read_app_by_unique_name(app::get_app_name(test_app_json));
    }
    REQUIRE(app_->get_app_name() == name);
    return app_;
}

inline json_t get_history_file(string file_name = "test_history.json"){
    boost::filesystem::path full_path(boost::filesystem::current_path());
    string path = "../test/iofiles_test/"+file_name;
    std::ifstream test_history_json_file(path, std::fstream::in);
    json_t history = make_unique<json>();
    test_history_json_file >> *history;
    return history;
}


inline user_t get_user(string name){
    auto usr = create_user(name,name);
    if(!usr){
        usr = db_op::read_user(name);
    }
    REQUIRE( usr->get_name() == name );
    return usr;
}

TEST_CASE( "testprojector", "[projector]" ) {
    measure_execution_raii exe("projector performance 2 projections");
    user_t usr_ = get_user("pepephone");
    app_t app_ = get_app(usr_->get_id());
    db_op::create_instalation_by_app_id(usr_->get_id(),app_->get_id());

    auto history = get_history_file();
    // app_projector ap ();
    shared_ptr<taskstory_commit_batched_raii> batch = make_shared<taskstory_commit_batched_raii>(usr_->get_tasker());
    cloud_app_runner car ( *usr_, *app_ ); // needs a non sessined mode
    car.projected_run((*history)["history"], 2, batch);
    for (auto &&i : *car.projected_scheduled_tasks)
    {
        REQUIRE(i.size() == 4);
    }
    batch->commit();
    REQUIRE(car.projected_scheduled_tasks->size() == 3);
}


TEST_CASE( "test projector performance", "[projector50]" ) {
    SPDLOG_INFO("running test projector performance ... ");
    measure_execution exe("projector performance 50 projections");
    user_t usr_ = get_user("pepephone");
    app_t app_ = get_app(usr_->get_id());
    db_op::create_instalation_by_app_id(usr_->get_id(),app_->get_id());

    auto history = get_history_file();
    // app_projector ap ();
    uint64_t total_tasks = 0;
    shared_ptr<taskstory_commit_batched_raii> batch = make_shared<taskstory_commit_batched_raii>(usr_->get_tasker());
    cloud_app_runner car ( *usr_, *app_ ); // needs a non sessined mode
    car.projected_run((*history)["history"], 50, batch);
    for (auto &&i : *car.projected_scheduled_tasks)
    {
        REQUIRE(i.size() == 4);
        total_tasks += 4;
    }
    batch->commit();
    REQUIRE(car.projected_scheduled_tasks->size() == 51);
    
    auto stop  =exe.stop();
    auto per_second = (float)total_tasks/stop.count();
    SPDLOG_INFO("{} tasks per second ", per_second);
    REQUIRE(per_second > 70);
    // 85 tasks per second
}


TEST_CASE( "test projector performance light", "[performance_projector]" ) {
    SPDLOG_INFO("running test projector performance ... ");
    measure_execution exe("projector performance");
    user_t usr_ = get_user("pepephone");
    app_t app_ = get_app(usr_->get_id());
    db_op::create_instalation_by_app_id(usr_->get_id(),app_->get_id());
    uint64_t total_tasks = 0;
    task_t t = make_unique<task>();
    t->set_user(usr_->get_id());
    measure_execution_raii r("sub");
    for (size_t i = 0; i < 200; i++)
    {
        t->set_id(prot::specifics::get_uuid());
        // _create_task_obj( *t);
        create_task({{usr_->get_id(), false}}, *t);
        total_tasks++;
    }
    
    auto stop  =exe.stop();
    auto per_second = (float)total_tasks/stop.count();
    SPDLOG_INFO("{} tasks per second ", per_second);
    REQUIRE(per_second > 701);
    // 85 tasks per second
}

TEST_CASE( "test projector performance heavy", "[performance_projector]" ) {
    SPDLOG_INFO("running test projector performance ... ");
    measure_execution exe("projector performance");
    uint64_t total_tasks = 0;
    user_t usr_ = get_user("pepephone");
    app_t app_ = get_app(usr_->get_id());
    db_op::create_instalation_by_app_id(usr_->get_id(),app_->get_id());
    measure_execution_raii r("sub");
    task_t t = make_unique<task>();
    t->set_user(usr_->get_id());
    t->set_description("12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230");
    for (size_t i = 0; i < 200; i++)
    {
        t->set_id(prot::specifics::get_uuid());
        _create_task_obj( *t);
        // create_task({{usr_->get_id(), false}}, *t);
        total_tasks++;
    }
    
    auto stop  =exe.stop();
    auto per_second = (float)total_tasks/stop.count();
    SPDLOG_INFO("{} tasks per second ", per_second);
    REQUIRE(per_second > 702);
    // 85 tasks per second
}

TEST_CASE( "test projector performance batched", "[performance_projector]" ) {
    SPDLOG_INFO("running test projector performance ... ");
    measure_execution exe("projector performance");
    uint64_t total_tasks = 0;
    user_t usr_ = get_user("pepephone");
    app_t app_ = get_app(usr_->get_id());
    db_op::create_instalation_by_app_id(usr_->get_id(),app_->get_id());
    measure_execution_raii r("sub");
    vector<task_t> batch;
    for (size_t i = 0; i < 1000; i++)
    {
        task_t t = make_unique<task>();
        t->set_user(usr_->get_id());
        t->set_description("12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230");
        t->set_id(prot::specifics::get_uuid());
        total_tasks++;
        batch.push_back(t);
    }
    db_op::_create_task_obj_batch(batch);
    
    auto stop  =exe.stop();
    auto per_second = (float)total_tasks/stop.count();
    SPDLOG_INFO("{} tasks per second ", per_second);
    REQUIRE(per_second > 70300);
    // 85 tasks per second
}

TEST_CASE( "test projector performance batched complete", "[performance_projector]" ) {
    SPDLOG_INFO("running test projector performance ... ");
    measure_execution exe("projector performance");
    uint64_t total_tasks = 0;
    user_t usr_ = get_user("pepephone");
    app_t app_ = get_app(usr_->get_id());
    db_op::create_instalation_by_app_id(usr_->get_id(),app_->get_id());
    measure_execution_raii r("sub");
    vector<db_op::create_tasks_user_bindings> batch;
    for (size_t i = 0; i < 1000; i++)
    {
        task_t t = make_unique<task>();
        t->set_user(usr_->get_id());
        t->set_description("12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230 12308120938lkafjlksdhfksh09134809384  dafjlsdjf3290402 kldj df9032409230");
        t->set_id(prot::specifics::get_uuid());
        db_op::create_tasks_user_bindings binds{
            {{usr_->get_id(), false}},
            t
        };
        total_tasks++;
        batch.push_back(binds);
    }
    db_op::create_task_bach_mono_user(batch);
    
    auto stop  =exe.stop();
    auto per_second = (float)total_tasks/stop.count();
    SPDLOG_INFO("{} tasks per second ", per_second);
    REQUIRE(per_second > 70300);
    // 85 tasks per second
}