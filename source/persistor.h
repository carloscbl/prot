#ifndef PERSISTOR_H
#define PERSISTOR_H

#include <memory>
#include "json.hpp"
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
using json = nlohmann::json;
using std::string;
using std::endl;
using std::cout;

/*design
    this class is intended to be an abstraction of serialization to json,
    regardless the configuration to persist on disk or network db like mongodb
*/
using std::shared_ptr;
using std::unique_ptr;

namespace fs = boost::filesystem;
using namespace fs;

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
    static persistor & get_persistor_instance();
    virtual void save (const string & index_name, const json & content_file) const noexcept = 0;
    virtual void load (const string & index_name, json & content_file) const noexcept = 0;
    persistor & set_path(string route_table_or_folder);
    static void set_persistor(unique_ptr<persistor> && storage){
        persistor::persistor_instance = move(storage);
    }
};

class disk_storage : public persistor{
private:
    const path folder = path(path("..") / "persistence");
public:
    disk_storage():persistor(folder.string()){};
    virtual void save ( const string & index_name, const json & content_file) const noexcept override ;
    virtual void load (const string & index_name, json & content_file) const noexcept;
};

// class mongo_db : public persistor{
//     mongo_db():persistor(""){};
//     virtual void save ( const string & index_name, const json & content_file) const noexcept override {
//         throw std::runtime_error("not implemented");
//     }
// };

#endif //PERSISTOR_H
