#include "Catch2/catch.hpp"
#include "expanded_taskstory_t.h"
#include <string>
#include <fstream>
#include <iostream>
#include "json.hpp"
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
using namespace fs;

TEST_CASE( "test task expansion", "[expanded_tasktory]" ) {
    boost::filesystem::path full_path(boost::filesystem::current_path());
    std::cout << "Current path is : " << full_path << std::endl;
    std::ifstream matrix_input_file("../test/iofiles_test/test_matrix_input_example.json", std::fstream::in);
    std::ifstream test_app_json_file("../test/iofiles_test/test_app_dailyworkout.json", std::fstream::in);
    json matrix_input;
    json test_app_json;
    matrix_input_file >> matrix_input;
    test_app_json_file >> test_app_json;

    // std::cout << matrix_input.dump(4) << std::endl;
    // std::cout << test_app_json.dump(4) << std::endl;
    auto question_test = test_app_json["questions"].at(2);
    next_question_data_and_taskstory_input nqdati;
    nqdati.next_question_text="bla bla";
    nqdati.taskstory_name="normal_flow";
    nqdati.raw_taskstory=question_test["taskstories"]["normal_flow"];
    nqdati.user_input=matrix_input;
    nqdati.current_question_obj=question_test;

    expand_taskstory_t et (nqdati);

    REQUIRE(et.expand_and_set()); // check expansion!
    // fmt::print("Is array? {}\n",nqdati.expanded_taskstory->is_array());
    // for(auto& [k,v]: nqdati.expanded_taskstory->items()){
    //     std::cout << v.dump(4) << std::endl;
    // }
    REQUIRE(nqdati.wildcard_expanded_taskstory->size() == 2); // check expansion!
    REQUIRE(nqdati.wildcard_expanded_taskstory->at(0).size() == 3); // check expansion!
    REQUIRE(nqdati.wildcard_expanded_taskstory->at(6).size() == 3); // check expansion!
    REQUIRE(nqdati.non_wildcard_expanded_taskstory->size() == 1); // check expansion!
    REQUIRE(true);
}

// Test next, what happens when you use an empty string "" in the user imput
// what happens when durating is ill formed
// test int, float, bool
// test wrong metadata values
// test inputs with bad format
