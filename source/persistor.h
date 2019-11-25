#ifndef PERSISTOR_H
#define PERSISTOR_H

#include <memory>
#include "json.hpp"
#include <iomanip>
#include <iostream>
#include <variant>
#include <functional>
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>
#include "test_prot.h"
using json = nlohmann::json;
using std::string;
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
    // template<typename Tsubtype>
    // void save ( const string & index_name, const json & content_file) noexcept {
    //     cout << "save" << endl;
    // }
    // template<typename Tsubtype>
    // void load (const string & index_name, json & content_file) noexcept{
    //     Tsubtype table;
    //     test_prot::Users usr;
    //     usr.
    //     for(const auto& row : db.run(sqlpp::select(all_of(table)).from(table).unconditionally()))
    //     {
    //         row
    //     	std::cerr << "row.name: " << row.name <<  std::endl;
    //     };
    //     cout << "load" << endl;
    // }
};



#endif //PERSISTOR_H
