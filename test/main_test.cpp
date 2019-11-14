//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_RUNNER
#include "Catch2/catch.hpp"
#include "user.h"
#include "form_runner.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>
#include <cstring>
#include <functional>
#include <vector>
#include <memory>
#include "command_processor.h"
#include "task.h"
#include "form.h"
#include "iactionable.h"
#include "form_reader.h"
#include "form_parser.h"
#include "form_collector.h"
#include "request.h"
#include "test.h"
#include <cstdlib>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

int main(int argc, char *argv[])
{
    // global setup...
    persistor::set_persistor(make_unique<disk_storage>());

    if(const char* env_p = std::getenv("PATH")){
        std::cout << "Your PATH is: " << env_p << '\n';
    }
    //http://mongocxx.org/mongocxx-v3/tutorial/
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};

    bsoncxx::builder::stream::document document{};

    auto collection = conn["testdb"]["testcollection"];
    document << "hello"
             << "world";

    collection.insert_one(document.view());
    auto cursor = collection.find({});

    for (auto &&doc : cursor)
    {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }


    //Its importan launch the program always from prot/build to take the relative paths
    form_collector fc;
    command_processor cp;
    user user_;
    tasker tasker_("std");

    std::vector<std::string> forms_paths;
    forms_paths = fc.get_forms_paths();

    form form_;
    request request_(cp);
    test test_(cp);
    cp.register_actionable("task", &tasker_);
    cp.register_actionable("form", &form_);
    cp.register_actionable("req", &request_);
    cp.register_actionable("user", &user_);
    cp.register_actionable("test", &test_);

    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////

    //////////////////////////////////////////////////////
    //// COMMANDS ARE NOW PERFORMABLE
    //////////////////////////////////////////////////////

    //// FORMS PATHS
    for_each(forms_paths.begin(), forms_paths.end(), [&cp](const string &s) {
        cp.perform_command("form add -P " + s);
    });

    //// DEFAULT USER
    cp.perform_command("user add -u carlos -p 123456 ");

    int result = Catch::Session().run(argc, argv);

    // global clean-up...

    return result;
}
