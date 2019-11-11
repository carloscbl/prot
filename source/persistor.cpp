#include "persistor.h"



persistor & persistor::get_persistor_instance()
{
    if(!persistor::persistor_instance) {
        throw std::runtime_error("not initialized yet?");
    }
    return *persistor::persistor_instance;
}


persistor & persistor::set_path(string route_table_or_folder){
    this->m_path = route_table_or_folder;
    return *this;
}

void disk_storage::save ( const string & index_name, const json & content_file) const noexcept {
    if (!is_directory(folder))
    {
        boost::filesystem::create_directory(folder);
    }
    //cout << content_file.dump(4) << endl;
    string file_ = folder.string() + "/" + index_name;
    std::ofstream o(file_, std::ofstream::trunc);
    o << std::setw(4) << content_file << std::endl;
}

void disk_storage::load (const string & index_name, json & content_file) const noexcept
{
    std::ifstream i(folder.string() + "/" + index_name);
    i >> content_file;
}