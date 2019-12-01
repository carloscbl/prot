#ifndef TRACE_BULLET_H
#define TRACE_BULLET_H
#include <string>
#include <map>
#include <optional>
#include <functional>
#include "persistor.h"
#include "user.h"
#include "json.hpp"
#include "time_utils.hpp"

using nlohmann::json;
using std::function;
using std::optional;
using std::string;

//This is not supported directly by the library, and sql syntax is specific
//Sparse things to support
/*
1 create user OK
2 read & E user OK 
3 delete user OK

3 CRUDE form OK

4 new instalation OK READ ok
5 user uninstall

6 CRUDE tasks
7 get tasks in interval

9 new questionary session
10 get/resume session
11 store responded answers and tag for bigdata

get user tasker scheduler and tasks asociations
*/

template <typename T>
auto get_data_member = []() {};

template <>
auto get_data_member<test_prot::Users> = []() { return test_prot::Users{}.username; };
template <>
auto get_data_member<test_prot::Forms> = []() { return test_prot::Forms{}.name; };

template <typename T>
bool gen_exists(string unique_val)
{
    auto &db = mysql_db::get_db_lazy().db;
    T table;
    if (!db(select(table.json).from(table).where(get_data_member<T>() == unique_val)).empty())
    {
        return false; //Already exists;
    }
    return true;
}

form *create_form(const json &valid_form)
{
    using test_prot::Forms;
    if (!gen_exists<test_prot::Forms>(form::get_form_name(valid_form)))
    {
        return nullptr;
    }

    auto &db = mysql_db::get_db_lazy().db;

    test_prot::Forms form_;
    form protform(valid_form);
    db(insert_into(form_).set(
        form_.json = valid_form.dump(),
        form_.name = form::get_form_name(valid_form),
        form_.developer = 1 // TODO
        ));
    return form::get_forms_register().at(protform.get_form_name()).get();
}

form *read_form(const string &form_name)
{
    auto &db = mysql_db::get_db_lazy().db;

    test_prot::Forms form_;
    const auto &result = db(sqlpp::select(all_of(form_)).from(form_).where(form_.name == form_name).limit(1U));
    if (result.empty())
    {
        return nullptr;
    }

    const auto &row = result.front();
    form protform(row.json); //We create it implictly or refresh it

    return form::get_forms_register().at(protform.get_form_name()).get();
}

void delete_form(const string &form_name)
{
    auto &db = mysql_db::get_db_lazy().db;

    test_prot::Forms form_;
    db(remove_from(form_).where(form_.name == form_name));
    form::remove_form(form_name);
}

void read_db_json()
{
    auto &db = mysql_db::get_db_lazy().db;
    std::stringstream sql;
    sql << "SELECT json->>\"$.username\" FROM users";

    auto statement = sqlpp::custom_query(sqlpp::verbatim(sql.str()))
                         .with_result_type_of(sqlpp::select(sqlpp::value("username").as(sqlpp::alias::a)));

    for (const auto &row : db(statement))
    {
        std::cout << "ID: " << row.a << std::endl;
    }
}

//This class is intended to advance needs until they are correctly categorized
unique_ptr<user> create_user(string username, json js)
{
    using test_prot::Users;
    if (!gen_exists<test_prot::Users>(username))
    {
        return nullptr;
    }

    auto &db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;

    auto us = make_unique<user>();
    json j = {{"username", "carloscbl"}};
    from_json(j, *us);
    db(insert_into(usr).set(usr.username = username, usr.json = json(*us).dump()));
    return us;
}

unique_ptr<user> read_user(string username)
{
    if (!gen_exists<test_prot::Users>(username))
    {
        return nullptr;
    }
    auto &db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    const auto &row = db(sqlpp::select(all_of(usr)).from(usr).unconditionally().limit(1U)).front();

    json juser(row.json);
    auto us = make_unique<user>();
    from_json(juser, *us);
    return us;
}

void delete_user(const string &username)
{
    auto &db = mysql_db::get_db_lazy().db;

    test_prot::Users usr;
    db(remove_from(usr).where(usr.username == username));
    user::users.erase(username);
}

bool create_instalation(const string &username, const string &form_name)
{
    auto &db = mysql_db::get_db_lazy().db;

    test_prot::Users usr;
    test_prot::Forms form_;

    // Exists? TODO JOIN both to get existent one
    const auto &usr_res = db(sqlpp::select(usr.id).from(usr).where(usr.username == username).limit(1U));
    const auto &form_res = db(sqlpp::select(form_.id).from(form_).where(form_.name == form_name).limit(1U));
    if (usr_res.empty() || form_res.empty())
    {
        return false;
    }
    test_prot::UsersForms instls;
    const auto &user_form_res = db(sqlpp::select(all_of(instls)).from(instls).where(instls.iduser == usr_res.front().id and instls.idform == form_res.front().id).limit(1U));

    if (!user_form_res.empty())
    {
        return false;
    }
   
    db(insert_into(instls).set(
        instls.iduser = usr_res.front().id,
        instls.idform = form_res.front().id));

    //We shouldn't load anything that is not required to perform
    //user::users.at(username)->instaled_forms[form_name] = form::get_forms_register().at(form_name).get();
    return true;
}

bool delete_instalation(const string &username, const string &form_name){
    auto &db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    test_prot::UsersForms usr_forms;
    test_prot::Forms form_;
    
    db(remove_from(usr_forms).using_(usr,form_,usr_forms).where( 
        usr_forms.iduser == usr.id 
        and usr.username == username 
        and usr_forms.idform == form_.id
        and form_.name == form_name
        ));
    return true;
}

vector<string> read_instalations(const string &username, optional<string> form_name = nullopt)
{
    auto &db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    test_prot::Forms form_;
    test_prot::UsersForms usr_forms;
    vector<string> formsresult;
    for (const auto &row : db(select(all_of(form_)).from(usr.join(usr_forms).on(usr.id == usr_forms.iduser).join(form_).on(form_.id == usr_forms.idform)).where(usr.username == username)))
    {
        formsresult.push_back(row.name);
    }
    return formsresult;
}

//https://github.com/rbock/sqlpp11/wiki/Select
void join()
{
    auto &db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    test_prot::Forms form_;
    for (const auto &row : db(select(all_of(usr)).from(usr.join(form_).on(usr.id == form_.developer)).unconditionally()))
    {
        cout << row.id << " " << row.json << endl;
    }
}

#endif //TRACE_BULLET_H