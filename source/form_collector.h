#ifndef FORM_COLLECTOR_H
#define FORM_COLLECTOR_H
#include <string>
#include <vector>
//#include <filesystem>
#include <iostream>
using namespace std;

//namespace fs = std::filesystem;

//This class is intended for monitor the folder of forms to charge them dynamically
class form_collector{
private:
    vector<string> forms;
public:
    form_collector(){
        //std::cout << "Current path is " << fs::current_path() << '\n';
        //PENDING ON KNOW HOW TO HANDLE RELATIVE PATHS
        // for (const auto & entry : fs::directory_iterator("",fs::directory_options::skip_permission_denied)){
        //     std::cout << entry.path() << std::endl;
        // }
    }
};

#endif //FORM_COLLECTOR_H