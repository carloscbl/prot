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
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/alias_provider.h>
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>
#include <sqlpp11/alias_provider.h>
#include <sqlpp11/connection.h>
#include <sqlpp11/functions.h>
#include <sqlpp11/select.h>

using nlohmann::json;
using std::function;
using std::optional;
using std::string;
//using namespace
using namespace sqlpp;
namespace mysql = sqlpp::mysql;

//This is not supported directly by the library, and sql syntax is specific
//Sparse things to support
/*
1 create user OK
2 read & E user OK
3 delete user OK

3 CRUDE form OK

4 new instalation OK READ ok
5 user uninstall OK Not need for update

6 CRUDE tasks
7 get tasks in interval

9 new questionary session
10 get/resume session
11 store responded answers and tag for bigdata

get user tasker scheduler and tasks asociations
*/

template <typename T>
auto get_id_member = []() {};

template <>
auto get_id_member<test_prot::Users> = []() { return test_prot::Users{}.id; };

//-----------------------

template <typename T>
auto get_data_member = []() {};

template <>
auto get_data_member<test_prot::Users> = []() { return test_prot::Users{}.username; };
template <>
auto get_data_member<test_prot::Forms> = []() { return test_prot::Forms{}.name; };
template <>
auto get_data_member<test_prot::Tasks> = []() { return test_prot::Tasks{}.id; };

template <typename T>
inline bool gen_exists(string unique_val)
{
    auto &db = mysql_db::get_db_lazy().db;
    T table;
    if (!db(select(table.json).from(table).where(get_data_member<T>() == unique_val)).empty())
    {
        return true; //Already exists;
    }
    return false;
}

template <typename T>
inline optional<uint64_t> get_id(string unique_val)
{
    auto &db = mysql_db::get_db_lazy().db;
    T table;
    const auto &result = db(select(get_id_member<T>().as(alias::a)).from(table).where(get_data_member<T>() == unique_val));
    if (result.empty())
    {
        return nullopt; //Already exists;
    }
    return result.front().a;
}

inline form *create_form(const json &valid_form, const string &username)
{
    using test_prot::Forms;
    if (gen_exists<test_prot::Forms>(form::get_form_name(valid_form)))
    {
        return nullptr;
    }

    auto &db = mysql_db::get_db_lazy().db;
    auto user_id = get_id<test_prot::Users>(username);
    if (!user_id.has_value())
    {
        return nullptr;
    }

    test_prot::Forms form_;
    form protform(valid_form);
    db(insert_into(form_).set(
        form_.json = valid_form.dump(),
        form_.name = form::get_form_name(valid_form),
        form_.developer = user_id.value() // TODO
        ));
    return form::get_forms_register().at(protform.get_form_name()).get();
}

inline form *read_form(const string &form_name)
{
    auto &db = mysql_db::get_db_lazy().db;

    test_prot::Forms form_;
    const auto &result = db(sqlpp::select(all_of(form_)).from(form_).where(form_.name == form_name).limit(1U));
    if (result.empty())
    {
        return nullptr;
    }

    const auto &row = result.front();
    form protform(json::parse(row.json.text)); //We create it implictly or refresh it

    return form::get_forms_register().at(protform.get_form_name()).get();
}

inline map<uint64_t,string> read_forms_by_developer(const string & developer){
    auto &db = mysql_db::get_db_lazy().db;

    test_prot::Forms form_;
    test_prot::Users usr;

    map<uint64_t,string> dev_forms;
    for( const auto &result : db( sqlpp::select(all_of(form_)).from(form_.join(usr).on(form_.developer == usr.id)).where(usr.username == developer ) )){
        dev_forms[result.id] = result.name;
    }
    return dev_forms;
}

inline optional<pair<uint64_t,string>> read_form_by_id(const int32_t &id)
{
    auto &db = mysql_db::get_db_lazy().db;

    test_prot::Forms form_;
    const auto &result = db( sqlpp::select(all_of(form_)).from(form_).where(form_.id == id ).limit(1U) );
    if (result.empty())
    {
        return nullopt;
    }

    const auto &row = result.front();
    return make_pair(row.id,row.name);
}

inline map<uint64_t,string> read_form_names()
{
    auto &db = mysql_db::get_db_lazy().db;

    test_prot::Forms form_;
    map<uint64_t,string> forms_names;

    for (const auto &result : db(sqlpp::select(form_.id,form_.name).from(form_).unconditionally().limit(1000U)))
    {
        forms_names[result.id] = result.name;
    }

    return forms_names;
}

inline void delete_form(const string &form_name)
{
    auto &db = mysql_db::get_db_lazy().db;

    test_prot::Forms form_;
    db(remove_from(form_).where(form_.name == form_name));
    form::remove_form(form_name);
}

inline void read_db_json()
{
    auto &db = mysql_db::get_db_lazy().db;
    std::stringstream sql;
    sql << "SELECT json->>\"$.username\" FROM users";
    {
        auto statement = sqlpp::custom_query(sqlpp::verbatim(sql.str()))
                            .with_result_type_of(sqlpp::select(sqlpp::value("username").as(sqlpp::alias::a)));
        for (const auto &row : db(statement))
        {
            std::cout << "ID: " << row.a << std::endl;
        }

    }
//     test_prot::Users usr;

//     auto statement = select(select(sqlpp::verbatim(R"--(json->>"$.username")--").as(sqlpp::alias::a)))
//    .from(usr)
//    .unconditionally();

//     for (const auto &row : db(statement))
//     {
//         std::cout << "ID: " << row.a << std::endl;
//     }
}

//This class is intended to advance needs until they are correctly categorized
inline unique_ptr<user> create_user(string username)
{
    using test_prot::Users;
    if (gen_exists<test_prot::Users>(username))
    {
        return nullptr;
    }

    auto &db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    test_prot::Taskers tasker_;
    test_prot::Schedulers sche;

    auto us = make_unique<user>();
    json j = {{"username", username}};
    from_json(j, *us);
    const auto &result = db(insert_into(usr).set(usr.username = username, usr.json = json(*us).dump()));
    us->set_id(result);
    db(insert_into(tasker_).set(tasker_.user = result));
    db(insert_into(sche).set(sche.user = result));
    return us;
}

inline unique_ptr<user> read_user(string username)
{
    if (!gen_exists<test_prot::Users>(username))
    {
        return nullptr;
    }
    auto &db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    const auto &row = db(sqlpp::select(all_of(usr)).from(usr).where( usr.username == username).limit(1U)).front();

    json juser = json::parse(row.json.text);
    auto us = make_unique<user>();
    from_json(juser, *us);
    us->set_id(row.id);
    return us;
}

inline bool delete_user(const string &username)
{
    auto &db = mysql_db::get_db_lazy().db;

    test_prot::Users usr;
    const auto & result = db(remove_from(usr).where(usr.username == username));
    if(result > 0){
        return true;
    }else{
        return false;
    }

}

inline bool create_instalation(const string &username, const string &form_name)
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

inline bool delete_instalation(const string &username, const uint64_t form_id)
{
    auto &db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    test_prot::UsersForms usr_forms;
    test_prot::Forms form_;

    db(remove_from(usr_forms).using_(usr, form_, usr_forms).where(usr_forms.iduser == usr.id and usr.username == username and usr_forms.idform == form_.id and form_.id == form_id));
    return true;
}

inline bool delete_instalation(const string &username, const string &form_name)
{
    auto &db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    test_prot::UsersForms usr_forms;
    test_prot::Forms form_;

    db(remove_from(usr_forms).using_(usr, form_, usr_forms).where(usr_forms.iduser == usr.id and usr.username == username and usr_forms.idform == form_.id and form_.name == form_name));
    return true;
}

inline map<uint64_t,string> read_instalations(const string &username, optional<uint64_t> form_id = nullopt)
{
    auto &db = mysql_db::get_db_lazy().db;
    test_prot::Users usr;
    test_prot::Forms form_;
    test_prot::UsersForms usr_forms;
    map<uint64_t,string> formsresult;

    if(form_id.has_value()){
        for (const auto &row : db(select(all_of(form_)).from(usr.join(usr_forms).on(usr.id == usr_forms.iduser).join(form_).on(form_.id == usr_forms.idform))
        .where(usr.username == username and form_.id == form_id.value() ) ))
        {
            formsresult[row.id] = row.name;
        }
        return formsresult;
    }

    for (const auto &row : db(select(all_of(form_)).from(usr.join(usr_forms).on(usr.id == usr_forms.iduser).join(form_).on(form_.id == usr_forms.idform)
    ).where(usr.username == username )))
    {
        formsresult[row.id] = row.name;
    }
    return formsresult;
}

//Users to asociate a task and boolean true to be scheduled not only added to tasker
inline bool create_task(const set<pair<string, bool>> &usernames_bindings_optional_scheduler, task &task_)
{
    auto &db = mysql_db::get_db_lazy().db;

    test_prot::Tasks tks;
    const auto &tsk_res = db(insert_into(tks).set(
        tks.name = task_.get_name(),
        tks.json = json(task_).dump(),
        tks.group = task_.get_task_group(),
        tks.start = sqlpp::tvin(system_clock::from_time_t(task_.get_interval().start)),
        tks.end = sqlpp::tvin(system_clock::from_time_t(task_.get_interval().end))));
    if (tsk_res < 1)
    {
        // Not insertion
        return false;
    }
    task_.set_id(tsk_res);
    for_each(usernames_bindings_optional_scheduler.begin(), usernames_bindings_optional_scheduler.end(), [&](const pair<string, bool> &binding) {
        test_prot::Taskers tasker_;
        test_prot::Schedulers sche;
        test_prot::Users usr;

        const auto result = db(sqlpp::select(all_of(usr), tasker_.idtasker, sche.id.as(alias::a))
                                   .from(usr.join(tasker_).on(tasker_.user == usr.id).join(sche).on(sche.user == usr.id))
                                   .where(usr.username == binding.first)
                                   .limit(1U));
        if (result.empty())
        {
            return ;
        }
        //const auto &uid = result.front().id;
        const auto &tasker_id = result.front().idtasker;
        const auto &sche_id = result.front().a;
        //Returns last insert
        test_prot::TasksTaskers tksTkrs;
        //Inserted so we need the binding
        //const auto &res_task_tasker =
         db(insert_into(tksTkrs).set(
            tksTkrs.idtask = tsk_res,
            tksTkrs.idtasker = tasker_id));
        if (binding.second)
        {
            test_prot::TasksSchedulers tskSche;
            //const auto &res_task_tasker =
             db(insert_into(tskSche).set(
                tskSche.idtask = tsk_res,
                tskSche.idscheduler = sche_id));
        }
    });
    return true;
}

inline unique_ptr<task> read_task(const string &username, const uint64_t task_id)
{
    auto &db = mysql_db::get_db_lazy().db;
    test_prot::Taskers tasker_;
    test_prot::TasksTaskers tskTkr;
    test_prot::Tasks tsk;
    test_prot::Users usr;
    const auto & select = sqlpp::select(all_of(tsk))
                                .from(usr.join(tasker_).on(tasker_.user == usr.id).join(tskTkr).on(tskTkr.idtasker == tasker_.idtasker).join(tsk).on(tsk.id == tskTkr.idtask))
                                .where(usr.username == username and tsk.id == task_id);
    auto resu = db(select);
    if(resu.empty()){
        return nullptr;
    }
    const auto & row = resu.front();
    json js  = json::parse(row.json.value()); //bad parsing
    auto tk = make_unique<task>();
    from_json(js,*tk);
    tk->set_id(row.id);
    return tk;
}

inline map<uint64_t,unique_ptr<task>> read_tasks(const string &username)
{
    auto &db = mysql_db::get_db_lazy().db;
    test_prot::Taskers tasker_;
    test_prot::TasksTaskers tskTkr;
    test_prot::Tasks tsk;
    test_prot::Users usr;
    const auto & select = sqlpp::select(all_of(tsk))
                                .from(usr.join(tasker_).on(tasker_.user == usr.id).join(tskTkr).on(tskTkr.idtasker == tasker_.idtasker).join(tsk).on(tsk.id == tskTkr.idtask))
                                .where(usr.username == username);
    map<uint64_t,unique_ptr<task>> vtasks;
    for (const auto & row : db(select))
    {
        cout << row.json.text << endl;
        cout << row.json.value() << endl;
        json js  = json::parse(row.json.value()); //bad parsing

        auto tk = make_unique<task>();
        from_json(js,*tk);
        tk->set_id(row.id);
        vtasks[row.id] = move(tk);
    }
    return vtasks;
}

inline void delete_task(const uint64_t task_id)
{
    auto &db = mysql_db::get_db_lazy().db;
    test_prot::Tasks tsk;
    db(remove_from(tsk).where(tsk.id == task_id));
}

inline bool delete_task(const string &username, const uint64_t task_id)
{
    auto &db = mysql_db::get_db_lazy().db;
    test_prot::Taskers tasker_;
    test_prot::TasksTaskers tskTkr;
    test_prot::Tasks tsk;
    test_prot::Users usr;
    
    if(db(remove_from(tsk).using_(usr, tasker_,tskTkr,tsk).where( 
    tasker_.user == usr.id 
    and usr.username == username 
    and tskTkr.idtask == tsk.id
    and tsk.id == task_id))){
        return true;
    }
    return false;
}

inline void update_task( task &new_task , const uint64_t task_id )
{
    auto &db = mysql_db::get_db_lazy().db;
    test_prot::Tasks tsk;
    const auto & result = db(update(tsk).set(
        tsk.name = new_task.get_name(),
        tsk.json = json(new_task).dump(),
        tsk.group = new_task.get_task_group(),
        tsk.start = sqlpp::tvin(system_clock::from_time_t(new_task.get_interval().start)),
        tsk.end = sqlpp::tvin(system_clock::from_time_t(new_task.get_interval().end))
    ).where(tsk.id == task_id));
    if(result <= 0){ return; }
    new_task.set_id(result);

}

// void create_session(const string &username, const string &form_name)
// {
//     auto &db = mysql_db::get_db_lazy().db;
// }

// form_state read_session(const string &username, const string &form_name, const form_state &fs)
// {
//     auto &db = mysql_db::get_db_lazy().db;
// }

// void update_session(const string &username, const string &form_name, const form_state &fs)
// {
//     auto &db = mysql_db::get_db_lazy().db;
// }

// void delete_session(const string &username, const string &form_name)
// {
//     auto &db = mysql_db::get_db_lazy().db;
// }

//https://github.com/rbock/sqlpp11/wiki/Select
inline void join()
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