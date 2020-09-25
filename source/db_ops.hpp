#ifndef DB_OPS_H
#define DB_OPS_H

#include <string>
#include <map>
#include <optional>
#include <functional>
#include "persistor.h"
#include "user.h"
#include "json.hpp"
#include "time_utils.hpp"
#include "app_parser.h"
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


template <typename T>
auto get_id_member = []() {};

template <>
auto get_id_member<orm_prot::Users> = []() { return orm_prot::Users{}.id; };

//-----------------------

template <typename T>
auto get_data_member = []() {};

template <>
auto get_data_member<orm_prot::Users> = []() { return orm_prot::Users{}.username; };
template <>
auto get_data_member<orm_prot::Apps> = []() { return orm_prot::Apps{}.name; };
template <>
auto get_data_member<orm_prot::Tasks> = []() { return orm_prot::Tasks{}.id; };

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

inline unique_ptr<app> create_app(const json &valid_app, const string &username)
{
    using orm_prot::Apps;
    if (gen_exists<orm_prot::Apps>(app::get_app_name(valid_app)))
    {
        return nullptr;
    }

    auto &db = mysql_db::get_db_lazy().db;
    auto user_id = get_id<orm_prot::Users>(username);
    if (!user_id.has_value())
    {
        return nullptr;
    }

    orm_prot::Apps app_;
    unique_ptr<app> protapp = make_unique<app>(valid_app);
    const auto &result = db(insert_into(app_).set(
        app_.json = valid_app.dump(),
        app_.name = app::get_app_name(valid_app),
        app_.developer = user_id.value() // TODO
        ));
    protapp->set_id(result);
    return protapp;
}

inline unique_ptr<app> read_app(const string &app_name)
{
    auto &db = mysql_db::get_db_lazy().db;

    orm_prot::Apps app_;
    const auto &result = db(sqlpp::select(all_of(app_)).from(app_).where(app_.name == app_name).limit(1U));
    if (result.empty())
    {
        return nullptr;
    }

    const auto &row = result.front();
    unique_ptr<app> protapp = make_unique<app>(json::parse(row.json.text)); //We create it implictly or refresh it
    protapp->set_id(result.front().id);

    return protapp;
}

inline map<uint64_t,string> read_apps_by_developer(const string & developer){
    auto &db = mysql_db::get_db_lazy().db;

    orm_prot::Apps app_;
    orm_prot::Users usr;

    map<uint64_t,string> dev_apps;
    for( const auto &result : db( sqlpp::select(all_of(app_)).from(app_.join(usr).on(app_.developer == usr.id)).where(usr.username == developer ) )){
        dev_apps[result.id] = result.name;
    }
    return dev_apps;
}

inline optional<pair<uint64_t,string>> read_app_by_id(const int32_t &id)
{
    auto &db = mysql_db::get_db_lazy().db;

    orm_prot::Apps app_;
    const auto &result = db( sqlpp::select(all_of(app_)).from(app_).where(app_.id == id ).limit(1U) );
    if (result.empty())
    {
        return nullopt;
    }

    const auto &row = result.front();
    return make_pair(row.id,row.name);
}

inline map<uint64_t,string> read_app_names()
{
    auto &db = mysql_db::get_db_lazy().db;

    orm_prot::Apps app_;
    map<uint64_t,string> apps_names;

    for (const auto &result : db(sqlpp::select(app_.id,app_.name).from(app_).unconditionally().limit(1000U)))
    {
        apps_names[result.id] = result.name;
    }

    return apps_names;
}

inline void delete_app(const string &app_name)
{
    auto &db = mysql_db::get_db_lazy().db;

    orm_prot::Apps app_;
    db(remove_from(app_).where(app_.name == app_name));
    app::remove_app(app_name);
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
//     orm_prot::Users usr;

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
    using orm_prot::Users;
    if (gen_exists<orm_prot::Users>(username))
    {
        return nullptr;
    }

    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::Users usr;
    orm_prot::Taskers tasker_;
    orm_prot::Schedulers sche;

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
    if (!gen_exists<orm_prot::Users>(username))
    {
        return nullptr;
    }
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::Users usr;
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

    orm_prot::Users usr;
    const auto & result = db(remove_from(usr).where(usr.username == username));
    if(result > 0){
        return true;
    }else{
        return false;
    }

}

inline bool create_instalation(const string &username, const string &app_name)
{
    auto &db = mysql_db::get_db_lazy().db;

    orm_prot::Users usr;
    orm_prot::Apps app_;

    // Exists? TODO JOIN both to get existent one
    const auto &usr_res = db(sqlpp::select(usr.id).from(usr).where(usr.username == username).limit(1U));
    const auto &app_res = db(sqlpp::select(app_.id).from(app_).where(app_.name == app_name).limit(1U));
    if (usr_res.empty() || app_res.empty())
    {
        return false;
    }
    orm_prot::UsersApps instls;
    const auto &user_app_res = db(sqlpp::select(all_of(instls)).from(instls).where(instls.iduser == usr_res.front().id and instls.idapp == app_res.front().id).limit(1U));

    if (!user_app_res.empty())
    {
        return false;
    }

    db(insert_into(instls).set(
        instls.iduser = usr_res.front().id,
        instls.idapp = app_res.front().id));

    //We shouldn't load anything that is not required to perapp
    //user::users.at(username)->instaled_apps[app_name] = app::get_apps_register().at(app_name).get();
    return true;
}

inline bool delete_instalation(const string &username, const uint64_t app_id)
{
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::Users usr;
    orm_prot::UsersApps usr_apps;
    orm_prot::Apps app_;

    db(remove_from(usr_apps).using_(usr, app_, usr_apps).where(usr_apps.iduser == usr.id and usr.username == username and usr_apps.idapp == app_.id and app_.id == app_id));
    return true;
}

inline bool delete_instalation(const string &username, const string &app_name)
{
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::Users usr;
    orm_prot::UsersApps usr_apps;
    orm_prot::Apps app_;

    db(remove_from(usr_apps).using_(usr, app_, usr_apps).where(usr_apps.iduser == usr.id and usr.username == username and usr_apps.idapp == app_.id and app_.name == app_name));
    return true;
}

inline map<uint64_t,string> read_instalations(const string &username, optional<uint64_t> app_id = nullopt)
{
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::Users usr;
    orm_prot::Apps app_;
    orm_prot::UsersApps usr_apps;
    map<uint64_t,string> appsresult;

    if(app_id.has_value()){
        for (const auto &row : db(select(all_of(app_)).from(usr.join(usr_apps).on(usr.id == usr_apps.iduser).join(app_).on(app_.id == usr_apps.idapp))
        .where(usr.username == username and app_.id == app_id.value() ) ))
        {
            appsresult[row.id] = row.name;
        }
        return appsresult;
    }

    for (const auto &row : db(select(all_of(app_)).from(usr.join(usr_apps).on(usr.id == usr_apps.iduser).join(app_).on(app_.id == usr_apps.idapp)
    ).where(usr.username == username )))
    {
        appsresult[row.id] = row.name;
    }
    return appsresult;
}

//Users to asociate a task and boolean true to be scheduled not only added to tasker
inline bool create_task(const set<pair<string, bool>> &usernames_bindings_optional_scheduler, task &task_)
{
    auto &db = mysql_db::get_db_lazy().db;

    orm_prot::Tasks tks;

    //["external_id"].get<string>()
    auto jtask = task_.get_json();
    string external_id;
    string prot_id;
    if (jtask.find("external_id") != jtask.end()){
        external_id = jtask["external_id"].get<string>();
    }
    if (jtask.find("prot_id") != jtask.end()){
        prot_id = jtask["prot_id"].get<string>();
    }
    const auto &tsk_res = db(insert_into(tks).set(
        tks.name = task_.get_name(),
        tks.json = json(task_).dump(),
        tks.group = task_.get_task_group(),
        tks.start = sqlpp::tvin(system_clock::from_time_t(task_.get_interval().start)),
        tks.end = sqlpp::tvin(system_clock::from_time_t(task_.get_interval().end)),
        tks.externalId = sqlpp::tvin(external_id),
        tks.fromUserAppsId = sqlpp::tvin(task_.get_user_apps_id()),
        tks.protId = sqlpp::tvin(prot_id),
        tks.sessionId = sqlpp::tvin(task_.get_session_id().value_or(0))
    ));
    if (tsk_res < 1)
    {
        // Not insertion
        mysql_db::current_db.reset();
        return false;
    }
    task_.set_id(tsk_res);
    for_each(usernames_bindings_optional_scheduler.begin(), usernames_bindings_optional_scheduler.end(), [&](const pair<string, bool> &binding) {
        orm_prot::Taskers tasker_;
        orm_prot::Schedulers sche;
        orm_prot::Users usr;

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
        orm_prot::TasksTaskers tksTkrs;
        //Inserted so we need the binding
        //const auto &res_task_tasker =
         db(insert_into(tksTkrs).set(
            tksTkrs.idtask = tsk_res,
            tksTkrs.idtasker = tasker_id));
        if (binding.second)
        {
            orm_prot::TasksSchedulers tskSche;
            //const auto &res_task_tasker =
             db(insert_into(tskSche).set(
                tskSche.idtask = tsk_res,
                tskSche.idscheduler = sche_id));
        }
    });
    mysql_db::current_db.reset();
    return true;
}

inline unique_ptr<task> read_task(const string &username, const uint64_t task_id)
{
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::Taskers tasker_;
    orm_prot::TasksTaskers tskTkr;
    orm_prot::Tasks tsk;
    orm_prot::Users usr;
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


inline map<uint64_t,unique_ptr<task>> search_tasks_by_not_this_session_id(const uint64_t session_id, const uint64_t user_apps_id)
{
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::Tasks tsk;
    const auto & select = sqlpp::select(all_of(tsk))
                                .from(tsk)
                                .where(tsk.sessionId != session_id and tsk.fromUserAppsId == user_apps_id);
    map<uint64_t,unique_ptr<task>> vtasks;
    for (const auto & row : db(select))
    {
        json js  = json::parse(row.json.value());

        auto tk = make_unique<task>();
        from_json(js,*tk);
        tk->set_id(row.id);
        vtasks[row.id] = move(tk);
    }
    return vtasks;
}

inline map<uint64_t,unique_ptr<task>> read_tasks(const string &username)
{
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::Taskers tasker_;
    orm_prot::TasksTaskers tskTkr;
    orm_prot::Tasks tsk;
    orm_prot::Users usr;
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

inline void delete_task(const vector<uint64_t> task_ids)
{
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::Tasks tsk;
    auto remove_query = dynamic_remove(db).from(tsk).dynamic_where();
    for (auto &&i : task_ids)
    {
        remove_query.where.add(tsk.id == i);
    }

    db(remove_query);
}

inline void delete_task(const uint64_t task_id)
{
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::Tasks tsk;
    db(remove_from(tsk).where(tsk.id == task_id));
}

inline bool delete_task(const string &username, const uint64_t task_id)
{
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::Taskers tasker_;
    orm_prot::TasksTaskers tskTkr;
    orm_prot::Tasks tsk;
    orm_prot::Users usr;
    
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
    orm_prot::Tasks tsk;
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

inline uint64_t get_user_apps_id(const uint64_t user_id, const uint64_t app_id){
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::UsersApps uapps;
    const auto & select = sqlpp::select(all_of(uapps))
                                .from(uapps)
                                .where(uapps.iduser == user_id and uapps.idapp == app_id);
    const auto & resu = db(select);
    if(resu.empty()){
        // No installation
        return 0;
    }
    const auto & row = resu.front();
    const auto & user_apps_id = row.id ;
    return user_apps_id;
}

inline shared_ptr<app_state> create_session(const uint64_t user_id, const uint64_t app_id)
{
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::AppSessions sess;
    const uint64_t user_apps_id = get_user_apps_id( user_id, app_id);
    if(!user_apps_id){
        return nullptr; // No installation
    }
    shared_ptr<app_state> new_sess = make_shared<app_state>();
    const auto &sess_res = db(insert_into(sess).set(
        sess.json = json(*new_sess).dump(),
        sess.userApps =  user_apps_id,
        sess.unqName = "asdasd"
        ));
    if (sess_res < 1)
    {
        // Not insertion
        return nullptr;
    }
    new_sess->id = sess_res;
    return new_sess;
}

inline shared_ptr<app_state> read_session(const string &username, const uint64_t app_id)
{
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::AppSessions sess;
    orm_prot::Users usr;
    orm_prot::UsersApps uapps;
    const auto & select = sqlpp::select(all_of(sess))
                                .from(sess.join(uapps).on(uapps.id == sess.userApps).join(usr).on(uapps.iduser == usr.id))
                                .where(usr.username == username and uapps.idapp == app_id);
    const auto & resu = db(select);
    if(resu.empty()){
        return nullptr;
    }
    const auto & row = resu.front();
    json js  = json::parse(row.json.value());
    shared_ptr<app_state> fs = make_shared<app_state>();
    from_json(js,*fs);
    fs->id = row.id;
    return fs;
}

inline void update_session( const uint64_t fs_id, app_state &new_fs)
{
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::AppSessions sess;
    const auto & result = db(update(sess).set(
        sess.json = json(new_fs).dump()
    ).where(sess.id == fs_id));
    if(result <= 0){ return; }
    new_fs.id = result;
}

inline bool delete_session(const uint64_t fs_id)
{
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::AppSessions sess;
    if(db(remove_from(sess).using_(sess).where( 
    sess.id == fs_id
    ))){
        return true;
    }
    return false;
}

//https://github.com/rbock/sqlpp11/wiki/Select
inline void join()
{
    auto &db = mysql_db::get_db_lazy().db;
    orm_prot::Users usr;
    orm_prot::Apps app_;
    for (const auto &row : db(select(all_of(usr)).from(usr.join(app_).on(usr.id == app_.developer)).unconditionally()))
    {
        cout << row.id << " " << row.json << endl;
    }
}

#endif //DB_OPS_H