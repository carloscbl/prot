#include "persistor.h"

mysql_db & mysql_db::get_db_lazy(){
    if(!mysql_db::current_db){
        mysql_db::current_db = std::make_unique<mysql_db>();
    }
    return *mysql_db::current_db;
}

shared_ptr<mysql::connection_config> mysql_db::get_db_config()
{
    auto config = std::make_shared<mysql::connection_config>();
    config->user = "root";//from env
    config->database = "test_prot";//from env
    config->debug = true; //from env
    config->password = "example"; //from env
    config->host = "127.0.0.1";//Get from env
    config->port = 3306;//from env
    return config;
}