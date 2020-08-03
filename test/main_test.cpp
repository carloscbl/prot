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
#include <fmt/core.h>

using nlohmann::json;


// namespace mysql = sqlpp::mysql;
int main(int argc, char *argv[])
{
    fmt::print("Hello, world!\n");
    std::string raw = "Hello, {name}! The answer is {number}. Goodbye. {name}";
    fmt::print(raw, fmt::arg("name", "World"), fmt::arg("number", 42));
    auto aresgg = fmt::make_format_args(fmt::arg("name", "Pepe"),fmt::arg("number", 45));
    fmt::print(fmt::vformat(raw, {aresgg}));
    fmt::dynamic_format_arg_store<fmt::format_context> store;
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
