#ifndef PERSISTOR_H
#define PERSISTOR_H

#include <memory>
#include "json.hpp"
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <fstream>
#include <iomanip>
using json = nlohmann::json;
using std::string;

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
    persistor(string route_table_or_folder){};
    ~persistor(){};
    static persistor & get_persistor_instance();
    virtual void save (const string & index_name, const json & content_file) const noexcept = 0;
    persistor & set_path(string route_table_or_folder);
    static void set_persistor(unique_ptr<persistor> && storage){
        persistor::persistor_instance = move(storage);
    }
};

class disk_storage : public persistor{
    disk_storage():persistor("../persistence/"){};
    virtual void save ( const string & index_name, const json & content_file) const noexcept  override {
        fs::path full_path(fs::current_path());
        path folder = path(path("..") / "persistence");

        if (is_directory(folder))
        {
            string file_ = folder.string() + index_name;
            std::ofstream o(file_);
            o << std::setw(4) << content_file << std::endl;
        }
    }
};

#endif //PERSISTOR_H
