#ifndef PERSISTOR_H
#define PERSISTOR_H

#include <memory>
#include "json.hpp"
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <variant>
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>
#include "../api/generated/test_prot.h"
using json = nlohmann::json;
using std::string;
using std::endl;
using std::cout;
using std::variant;

/*design
    this class is intended to be an abstraction of serialization to json,
    regardless the configuration to persist on disk or network db like mongodb
*/
using std::shared_ptr;
using std::unique_ptr;

namespace fs = boost::filesystem;
using namespace fs;


template<typename T>
class persistor
{
private:
    /* data */
protected:
    inline static unique_ptr<persistor> persistor_instance;
    string m_path;
public:
    persistor(){};
    persistor(string route_table_or_folder){};
    ~persistor(){};
    static persistor & get_persistor_instance(){
        if(!persistor_instance) {
            throw std::runtime_error("not initialized yet?");
        }
        return *persistor_instance;
    }

    template<typename Tsubtype>
    void save (const string & index_name, const json & content_file) const noexcept{
        ((T*)this)->template save<Tsubtype>(index_name, content_file); 
    }
    template<typename Tsubtype>
    void load (const string & index_name, json & content_file) const noexcept{
        ((T*)this)->template load<Tsubtype>(index_name, content_file); 
    }

    persistor & set_path(string route_table_or_folder);
    static void set_persistor(unique_ptr<persistor> && storage){
        persistor_instance = move(storage);
    }
};

class disk_storage : public persistor<disk_storage>{
private:
    //TODO : To be adquired by a env var PERSISTENCE_PATH
    inline static const path folder = "../persistence";
public:
    disk_storage():persistor(this->folder.string()){};
    template<typename Tsubtype>
    void save ( const string & index_name, const json & content_file) const noexcept ;
    template<typename Tsubtype>
    void load (const string & index_name, json & content_file) const noexcept;
};


namespace mysql = sqlpp::mysql;
class mysql_db : public persistor<mysql_db>{
public:
    static shared_ptr<mysql::connection_config> get_db_config();
    mysql_db():persistor(""),db(get_db_config()){
    };

    mysql::connection db;
    template<typename Tsubtype>
    void save ( const string & index_name, const json & content_file) const noexcept {
        cout << "aaa" << endl;
    }
    template<typename Tsubtype>
    void load (const string & index_name, json & content_file) const noexcept{
        cout << "aaa" << endl;
    }
};

class persistor_instance{
    inline static unique_ptr<variant<persistor<mysql_db>,persistor<disk_storage>>> m_persistor_instance;
};


#endif //PERSISTOR_H
