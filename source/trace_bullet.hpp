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
    json j = {{"username","carloscbl"}};
    from_json(j,us);
    //db(insert_into(usr).set( usr.json = json(us).dump()));

    //
    test_prot::Forms form_;
    //uint64_t id = usr.id;
    json jform = form::get_register().begin()->second->get_json();
    db(insert_into(form_).set( 
        form_.formJson = jform.dump(),
        form_.name = jform["form"]["form.name"].get<string>(),
        form_.developer = 8
    ));
    // test_prot::Users usrs;
	// for(const auto& row : db.run(sqlpp::select(all_of(usrs)).from(usrs).unconditionally()))
}

void load_user_from_db(string username){
    auto & db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    for(const auto& row : db.run(sqlpp::select(all_of(usr)).from(usr).unconditionally()))
    {
        std::cerr << "row.json: " << row.json <<  std::endl;
    };
}


#endif //TRACE_BULLET_H