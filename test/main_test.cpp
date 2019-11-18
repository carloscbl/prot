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
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>
#include "../api/generated/test_prot.h"

namespace mysql = sqlpp::mysql;
int main(int argc, char *argv[])
{
    // global setup...
    persistor::set_persistor(make_unique<disk_storage>());

    if(const char* env_p = std::getenv("PATH")){
        std::cout << "Your PATH is: " << env_p << '\n';
    }
    auto config = std::make_shared<mysql::connection_config>();
 	config->user = "root";//from env
 	config->database = "test_prot";//from env
	config->debug = true; //from env
    config->password = "example"; //from env
    config->host = "127.0.0.1";//Get from env
    config->port = 3306;//from env
	mysql::connection db(config);
    {
	    test_prot::Users usrs;
        db(sqlpp::insert_into(usrs).set(usrs.name = "BeBe"));
    }
    test_prot::Users usrs;
	for(const auto& row : db.run(sqlpp::select(all_of(usrs)).from(usrs).unconditionally()))
	{
		std::cerr << "row.name: " << row.name <<  std::endl;
	};

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
