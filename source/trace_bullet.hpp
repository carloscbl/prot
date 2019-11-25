#ifndef TRACE_BULLET_H
#define TRACE_BULLET_H
#include <string>
#include <map>
#include "persistor.h"
#include "user.h"
#include "json.hpp"

using nlohmann::json;
using std::string;



//This class is intended to advance needs until they are correctly categorized

void fill_db(){
    auto & db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    user us;
    json j = {"username","carloscbl"};
    from_json(j,us);
    db(insert_into(usr).set( usr.json = json(us).dump()));
}

void load_user_from_db(string username){
    auto & db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    for(const auto& row : db.run(sqlpp::select(all_of(usr)).from(usr).unconditionally()))
    {
        std::cerr << "row.name: " << row.json <<  std::endl;
    };
}


#endif //TRACE_BULLET_H