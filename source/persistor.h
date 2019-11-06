#ifndef PERSISTOR_H
#define PERSISTOR_H

#include <memory>
#include "json.hpp"

using json = nlohmann::json;
using std::string;

/*design
    this class is intended to be an abstraction of serialization to json,
    regardless the configuration to persist on disk or network db like mongodb
*/
using std::shared_ptr;
using std::unique_ptr;

class persistor
{
private:
    /* data */
    static unique_ptr<persistor> persistor_instance;
protected:
    string path;
public:
    persistor(string route_table_or_folder);
    ~persistor();
    static persistor & get_persistor_instance();
    virtual void save (string index_name, json file) noexcept = 0;
    persistor & set_path(string route_table_or_folder);
};


#endif //PERSISTOR_H
