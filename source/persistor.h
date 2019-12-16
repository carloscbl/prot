#ifndef PERSISTOR_H
#define PERSISTOR_H

#include <memory>
#include "json.hpp"
#include <iomanip>
#include <iostream>
#include <variant>
#include <functional>
#include <optional>
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>
#include "test_prot.h"
using json = nlohmann::json;
using std::string;
using std::optional;
using std::endl;
using std::cout;
using std::variant;

/*design
    this class is intended to be an abstraction of serialization to json,
    regardless the configuration to persist on disk or network db like mongodb
*/
using std::shared_ptr;
using std::unique_ptr;



namespace mysql = sqlpp::mysql;

class mysql_db {
private:
    inline static unique_ptr<mysql_db> current_db;
public:
    static shared_ptr<mysql::connection_config> get_db_config();
    mysql_db():db(get_db_config()){
    };
    mysql::connection db;
    static mysql_db & get_db_lazy();

};



#endif //PERSISTOR_H
