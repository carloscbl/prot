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
    std::ofstream output_stream_to_file(file_, std::ofstream::trunc);
    output_stream_to_file << std::setw(4) << content_file << std::endl;
}

void disk_storage::load (const string & index_name, json & content_file) const noexcept
{
    std::ifstream input_stream_from_file(folder.string() + "/" + index_name);
    input_stream_from_file >> content_file;
}
void mysql_db::load (const string & index_name, json & content_file) const noexcept{

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