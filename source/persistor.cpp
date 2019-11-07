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