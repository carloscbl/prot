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
// #include "app.h"
// #include "app_parser.h"
// #include <cstdlib>
// #include <sqlpp11/sqlpp11.h>
// #include <sqlpp11/mysql/mysql.h>
// #include "orm_prot.h"
// #include "db_ops.hpp"
#include "json.hpp"
#include <any>
#include <map>
#include <functional>
#include <fmt/core.h>
#include <fmt/format.h>
#include "type_conversor.h"

using nlohmann::json;
using std::function;
using std::map;
using std::nullopt;
using std::optional;


// namespace mysql = sqlpp::mysql;
int main(int argc, char *argv[])
{
    // fmt::print("Hello, world!\n");
    // std::string raw = "Hello, {prot_idx}! The answer is {number}. Goodbye.\n";
    // fmt::dynamic_format_arg_store<fmt::format_context> store;
    // store.push_back(fmt::arg("prot_idx", "@Pepe3000"));
    // store.push_back(fmt::arg("aanumber2", 5000)); // So we can exceed arguments but never less than needed
    // store.push_back(fmt::arg("number", 3000));
    // std::string interpolation = fmt::vformat(raw, store);

    // std::cout << interpolation << std::endl;

    // json aa;
    // aa["pepe"] = 35;
    // aa["pepeo"] = 35.5;
    // aa["pepeo1"] = false;
    // aa["arr"] = {"kk","xx","pp"};
    // std::any ab = aa["pepe"];

    // std::cout << prot::json_to_string(aa["pepe"]).value() << std::endl;
    // std::cout << prot::json_to_string(aa["pepeo"]).value() << std::endl;
    // std::cout << prot::json_to_string(aa["pepeo1"]).value() << std::endl;
    // // fmt::print(ab.type().name());
    // if(json::value_t::number_integer == aa["pepe"].type()){
    //     std::cout << prot::json_to_string(aa["pepe"]).value() << std::endl;

    // }
    // for (auto& [key, value] : aa["arr"].items()) {
    //     std::cout << key << " : " << value << "\n";
    // }

    // for (auto&  value: aa["arr"]) {
    //     std::cout << value << "\n";
    // }



    int result = Catch::Session().run(argc, argv);
    return result;
}
