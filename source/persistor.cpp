#include "persistor.h"

persistor & persistor::get_persistor_instance(){
    if(!persistor_instance) throw std::runtime_error("not initialized yet?");
    return *persistor_instance;
}
persistor::persistor(/* args */)
{
}

persistor::~persistor()
{
}
