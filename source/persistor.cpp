#include "persistor.h"

persistor & persistor::get_persistor_instance(){
    if(!persistor_instance) throw std::runtime_error("not initialized yet?");
    return *persistor_instance;
}

persistor::persistor(string route_table_or_folder)
{
}

persistor::~persistor()
{
}

persistor & persistor::set_path(string route_table_or_folder){
    this->path = route_table_or_folder;
    return *this;
}