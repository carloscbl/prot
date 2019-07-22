#ifndef FORM_COLLECTOR_H
#define FORM_COLLECTOR_H
#include <string>
#include <vector>
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS
#include <iostream>
using namespace std;

namespace fs = boost::filesystem;

//This class is intended for monitor the folder of forms to charge them dynamically
class form_collector{
private:
    //vector<string> forms;
public:
    form_collector(){
        fs::path full_path(fs::current_path());
        std::cout << "Current path is : " << full_path << std::endl;
        //std::cout << "Current path is " << fs::current_path() << std::endl;
        //PENDING ON KNOW HOW TO HANDLE RELATIVE PATHS
        // for (const auto & entry : fs::directory_iterator("",fs::directory_options::skip_permission_denied)){
        //     std::cout << entry.path() << std::endl;
        // }
    }
    virtual ~form_collector(){}
};

#endif //FORM_COLLECTOR_H