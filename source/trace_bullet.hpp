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

template<typename T>
auto get_data_member = [](){};

template<>
auto get_data_member<test_prot::Users> = [](){ return test_prot::Users{}.username; };
template<>
auto get_data_member<test_prot::Forms> = [](){ return test_prot::Forms{}.name; };


template<typename T>
bool gen_exists(string unique_val){
    auto & db = mysql_db::get_db_lazy().db;
    T table;
    if (!db(select(table.json).from(table).where( get_data_member<T>() == unique_val )).empty()){
        return false; //Already exists;
    }
    return true;
}

unique_ptr<form> create_form( const json & valid_form ){
    using test_prot::Forms;
    if(!gen_exists<test_prot::Forms>(valid_form["form"]["form.name"].get<string>() )){
        return nullptr;
    }

    auto & db = mysql_db::get_db_lazy().db;

    test_prot::Forms form_;
    auto protform = make_unique<form>();
    from_json( valid_form, *protform);
    
    db(insert_into(form_).set( 
        form_.json = valid_form.dump(),
        form_.name = valid_form["form"]["form.name"].get<string>(),
        form_.developer = 1
    ));
    return protform;
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

//This class is intended to advance needs until they are correctly categorized
unique_ptr<user> new_user(string username, json js){
    using test_prot::Users;
    if(!gen_exists<test_prot::Users>(username)){
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
    if(!gen_exists<test_prot::Users>(username)){
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