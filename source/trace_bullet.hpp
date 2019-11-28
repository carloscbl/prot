#ifndef TRACE_BULLET_H
#define TRACE_BULLET_H
#include <string>
#include <map>
#include <optional>
#include <functional>
#include "persistor.h"
#include "user.h"
#include "json.hpp"

using nlohmann::json;
using std::string;
using std::optional;
using std::function;

//This is not supported directly by the library, and sql syntax is specific
//Sparse things to support
/*
1 new user OK
2 get user OK

3 CRUD form

4 new instalation
5 user uninstall

6 CRUD tasks
7 get tasks in interval

9 new questionary session
10 get/resume session
11 store responded answers and tag for bigdata

get user tasker scheduler and tasks asociations
*/

unique_ptr<form> create_form( json valid_form ){
    auto & db = mysql_db::get_db_lazy().db;
}

void read_db_json(){
    auto & db = mysql_db::get_db_lazy().db;
    std::stringstream sql;
    sql << "SELECT json->>\"$.username\" FROM users";

    auto statement = sqlpp::custom_query(sqlpp::verbatim(sql.str()))
    .with_result_type_of(sqlpp::select(sqlpp::value("username").as(sqlpp::alias::a)));

    for(const auto & row : db(statement))
    {
        std::cout << "ID: " << row.a << std::endl;
    }
}


template<typename T>
bool gen_exists(string unique_val, std::function<string(T const&)> data_member_accessor){
    auto & db = mysql_db::get_db_lazy().db;
    T table;
    if (!db(select(table.json).from(table).where( data_member_accessor(table) == unique_val )).empty()){
        return false; //Already exists;
    }
    return true;
}

bool user_exists(string username){
    auto & db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    if (!db(select(usr.json).from(usr).where(usr.username == username )).empty()){
        return false; //Already exists;
    }
    return true;
}

//This class is intended to advance needs until they are correctly categorized
unique_ptr<user> new_user(string username, json js){
    using test_prot::Users;
    function<string(test_prot::Users)> data_member = &Users::username;
    gen_exists<test_prot::Users>(username, data_member);
    if(!user_exists(username)){
        return nullptr;
    }

    auto & db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    
    auto us = make_unique<user>();
    json j = {{"username","carloscbl"}};
    from_json(j, *us);
    db(insert_into(usr).set( usr.username = username, usr.json = json(*us).dump()));
    return us;
}

unique_ptr<user> get_user(string username){
    if(!user_exists(username)){
        return nullptr;
    }
    auto & db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    const auto& row = db(sqlpp::select(all_of(usr)).from(usr).unconditionally().limit(1U)).front();

    json juser ( row.json );
    auto us = make_unique<user>();
    from_json(juser, *us);
    return us;
}


// void fill_db(){
//     auto & db = mysql_db::get_db_lazy().db;
//     test_prot::Users usr;
    
//     user us;
//     json j = {{"username","carloscbl"}};
//     from_json(j,us);
//     db(insert_into(usr).set( usr.json = json(us).dump()));

//     //
//     test_prot::Forms form_;
//     // //uint64_t id = usr.id;
//     json jform = form::get_register().begin()->second->get_json();
//     db(insert_into(form_).set( 
//         form_.formJson = jform.dump(),
//         form_.name = jform["form"]["form.name"].get<string>(),
//         form_.developer = 1
//     ));
//     // test_prot::Users usrs;
// 	for(const auto& row : db.run(sqlpp::select(all_of(form_)).from(form_).unconditionally().limit(1U))){
//         int id2 =row.id;
//         cout << row.id << " " << row.name << " " << row.developer << endl;
//     }
// }


//https://github.com/rbock/sqlpp11/wiki/Select
void join(){
    auto & db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    test_prot::Forms form_;
    for(const auto & row : db(select(all_of(usr)).from(usr.join(form_).on(usr.id == form_.developer)).unconditionally())){
        cout << row.id << " " << row.json << endl;
    }
}


#endif //TRACE_BULLET_H