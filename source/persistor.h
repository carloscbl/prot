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
#include <functional>
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/mysql/mysql.h>
#include "test_prot.h"
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
    string m_path;
public:
    persistor(){};
    persistor(string route_table_or_folder){};
    ~persistor(){};
   

    template<typename Tsubtype>
    void save (const string & index_name, const json & content_file) const noexcept{
        cout << "persistor save" << endl;
        ((T*)this)->template save<Tsubtype>(index_name, content_file); 
    }
    template<typename Tsubtype>
    void load (const string & index_name, json & content_file) const noexcept{
        cout << "persistor load" << endl;
        ((T*)this)->template load<Tsubtype>(index_name, content_file); 
    }

    persistor & set_path(string route_table_or_folder);
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

template<typename T>
persistor<T> & persistor<T>::set_path(string route_table_or_folder){
    this->m_path = route_table_or_folder;
    return *this;
}

template<typename Tsubtype>
void disk_storage::save ( const string & index_name, const json & content_file) const noexcept {
    if (!is_directory(folder))
    {
        boost::filesystem::create_directory(folder);
    }
    //cout << content_file.dump(4) << endl;
    string file_ = folder.string() + "/" + index_name;
    std::ofstream output_stream_to_file(file_, std::ofstream::trunc);
    output_stream_to_file << std::setw(4) << content_file << std::endl;
}

template<typename Tsubtype>
void disk_storage::load (const string & index_name, json & content_file) const noexcept
{
    std::ifstream input_stream_from_file(folder.string() + "/" + index_name);
    input_stream_from_file >> content_file;
}

namespace mysql = sqlpp::mysql;
class mysql_db : public persistor<mysql_db>{
public:
    static shared_ptr<mysql::connection_config> get_db_config();
    mysql_db():persistor(""),db(get_db_config()){
    };

    mysql::connection db;
    template<typename Tsubtype>
    void save ( const string & index_name, const json & content_file) noexcept {
        cout << "save" << endl;
    }
    template<typename Tsubtype>
    void load (const string & index_name, json & content_file) noexcept{
        Tsubtype table;
        test_prot::Users usr;
        usr.
        for(const auto& row : db.run(sqlpp::select(all_of(table)).from(table).unconditionally()))
        {
            row
        	std::cerr << "row.name: " << row.name <<  std::endl;
        };
        cout << "load" << endl;
    }
};

class persistor_instance{
    using pers = variant<unique_ptr<mysql_db>,unique_ptr<disk_storage>>;
    inline static pers m_persistor_instance;
public:
    inline static void set_instance(pers && per){
        persistor_instance::m_persistor_instance = move(per);
    }
    template<typename T>
    inline static void save ( const string & index_name, const json & content_file) noexcept {
        
        switch( m_persistor_instance.index())
        {
        case 0:
            std::get<unique_ptr<mysql_db>>(m_persistor_instance)->template save<T>(index_name, content_file);
            break;
        case 1:
            std::get<unique_ptr<disk_storage>>(m_persistor_instance)->template save<T>(index_name, content_file);
            break;
        default:
            throw std::runtime_error("bad persistor instance");
            break;
        }
    }
    template<typename T>
    inline static void load (const string & index_name, json & content_file) noexcept{
        switch( m_persistor_instance.index())
        {
        case 0:
            std::get<unique_ptr<mysql_db>>(m_persistor_instance)->template load<T>(index_name, content_file);
            break;
        case 1:
            std::get<unique_ptr<disk_storage>>(m_persistor_instance)->template load<T>(index_name, content_file);
            break;
        default:
            throw std::runtime_error("bad persistor instance");
            break;
        }
    }
};


#endif //PERSISTOR_H
