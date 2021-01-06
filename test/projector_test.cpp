#include "Catch2/catch.hpp"
#include "json.hpp"
#include "user.h"
#include "app.h"
#include "cloud_app_runner.h"

#include <fstream>
#include <iostream>
#include "json.hpp"
// #include "time_utils.hpp"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "app_projector.h"
namespace fs = boost::filesystem;
using namespace fs;

using user_t = unique_ptr<user>;
using app_t = unique_ptr<app>;
using json_t = unique_ptr<json>;
inline app_t get_app(string name = "test Daily workout history"){
    auto name_file = name;
    boost::filesystem::path full_path(boost::filesystem::current_path());
    boost::erase_all(name_file, " ");
    boost::to_lower(name_file);
    cout << name_file << endl;
    cout << name << endl;
    string path = "../test/iofiles_test/test_app_"+name_file+".json";
    cout << path << endl;
    std::ifstream test_app_json_file(path, std::fstream::in);
    json test_app_json;
    test_app_json_file >> test_app_json;
    app_t app_ = make_unique<app>(test_app_json);
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
    user_t us = make_unique<user>(user_minimal_data{name});
    REQUIRE( us->get_name() == name );
    return us;
}

TEST_CASE( "testprojector", "[projector]" ) {

    app_t app_ = get_app();
    user_t usr_ = get_user("pepephone");

    // discovered_components dc (
    //     row.iduser,
    //     uint64_t(row.idapp.value()),
    //     std::move(qaHistory),
    //     tp,
    //     uint64_t(row.id.value())
    // );

    auto history = get_history_file();
    // app_projector ap ();
    cloud_app_runner car ( *usr_, *app_ ); // needs a non sessined mode
    car.programatic_run_injecting_history_answers((*history)["history"]);
    cout << car.scheduled_tasks->size() << endl;
    REQUIRE(car.scheduled_tasks->size() > 1);

}