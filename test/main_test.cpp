//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_RUNNER
#include "Catch2/catch.hpp"
// #include "user.h"
// #include "cloud_app_runner.h"
// #include <iostream>
// #include <ctime>
// #include <fstream>
// #include <map>
// #include <algorithm>
// #include <string>
// #include <cstring>
// #include <functional>
// #include <vector>
// #include <memory>
// #include "task.h"
// #include "form.h"
// #include "app_parser.h"
// #include <cstdlib>
// #include <sqlpp11/sqlpp11.h>
// #include <sqlpp11/mysql/mysql.h>
// #include "test_prot.h"
// #include "db_ops.hpp"
#include "json.hpp"
#include <any>
#include <map>
#include <functional>
#include <fmt/core.h>
#include <fmt/format.h>

using nlohmann::json;
using std::function;
using std::map;

optional<std::string> json_to_string(const json & value){
    switch (value.type())
    {
    case json::value_t::boolean:
        return std::to_string(value.get<bool>());
    case json::value_t::number_float:
        return std::to_string(value.get<float>());
    case json::value_t::number_integer:
        return std::to_string(value.get<int>());
    case json::value_t::number_unsigned:
        return std::to_string(value.get<size_t>());
    case json::value_t::string:
        return value.get<std::string>();
    default:
        return nullopt;
    }
}

// namespace mysql = sqlpp::mysql;
int main(int argc, char *argv[])
{
    fmt::print("Hello, world!\n");
    std::string raw = "Hello, {name}! The answer is {number}. Goodbye. {name}\n";
    fmt::print(raw, fmt::arg("name", "World"), fmt::arg("number", 42));
    auto aresgg = fmt::make_format_args(fmt::arg("name", "Pepe"),fmt::arg("number", 45));
    fmt::print(fmt::vformat(raw, {aresgg}));
    fmt::dynamic_format_arg_store<fmt::format_context> store;
    store.push_back(fmt::arg("name", "Pepe3000"));
    store.push_back(fmt::arg("aanumber2", 5000)); // So we can exceed arguments but never less than needed
    store.push_back(fmt::arg("number", 3000));
    std::string interpolation = fmt::vformat(raw, store);

    std::cout << interpolation << std::endl;

    json aa;
    aa["pepe"] = 35;
    std::any ab = aa["pepe"];

    // fmt::print(ab.type().name());
    if(json::value_t::number_unsigned == aa["pepe"].type()){
        std::cout << aa["pepe"] << std::endl;

    }
    // fmt::print(aa["pepe"].type());
    // fmt::print("pepe vale {num}", fmt::arg("num",  aa["pepe"].type ));


    // std::string result = fmt::vformat("{} and {} and {}", aresgg);
    // check dynamic_format_arg_store
    // fmt::print(raw, fmt::arg("name", "World"));
    // fmt::print(raw, fmt::arg("number", 42));
    // read_db_json();
    // //delete_user("pepe");
    // create_user("carloscbl");
    // create_user("pepe");
    
    // json jj = form::get_forms_register().at("Washer easer")->get_json();
    // create_form(jj, "carloscbl");
    // create_instalation("carloscbl", "Washer easer");
    // create_instalation("carloscbl", "Lemon");
    // create_instalation("carloscbl", "Pepoform");
    // //fill_db();
    // task tk;
    // tk.set_name("lemonsss_task");
    // task tk2;
    // tk2.set_name("pepe_task");
    // create_task({{"carloscbl",true},{"pepe",false}},tk);
    // create_task({{"pepe",false}},tk2);
    // join();
    // auto tasksss =  read_tasks("pepe");

    // for (auto &&[k,v] : tasksss)
    // {
    //     cout << v->get_name() << endl;
    // }
    

    // read_instalations("carloscbl");
    // delete_instalation("carloscbl", "Lemon");
    // read_instalations("carloscbl");
    // //delete_user("pepe");


    int result = Catch::Session().run(argc, argv);
    return result;
}
