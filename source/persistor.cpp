#include "persistor.h"
#include "spdlog/spdlog.h"


optional<string> get_env(const string & env_var) {
    const char* env_p = std::getenv(env_var.c_str());
    if(!env_p){
        return std::nullopt;
    }

    return env_p;
}

mysql_db & mysql_db::get_db_lazy(){
    if(!mysql_db::current_db){
        mysql_db::current_db = std::make_unique<mysql_db>();
    }
    return *mysql_db::current_db;
}

shared_ptr<mysql::connection_config> mysql_db::get_db_config()
{
    auto config = std::make_shared<mysql::connection_config>();
    config->user = get_env("PROT_DB_USER").value_or("root");
    config->database = get_env("PROT_DB_SCHEMA").value_or("prot");
    config->debug = true; 
    config->password = get_env("PROT_DB_PASSW").value_or("example");
    config->host = get_env("PROT_DB_HOST").value_or("127.0.0.1");
    config->port = stol(get_env("PROT_DB_PORT").value_or("3306"));
    SPDLOG_DEBUG("paswword {}", config->password);
    SPDLOG_DEBUG("user {}", config->user);
    SPDLOG_DEBUG("host {}", config->host);
    SPDLOG_DEBUG("port {}", config->port);
    return config;
}
