#include "persistor.h"


// template<typename T>
// persistor<T> & persistor<T>::get_persistor_instance()
// {
    
// }



// template<typename Tsubtype>
// void mysql_db::load (const string & index_name, json & content_file) const noexcept{
//     //db()
// }



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