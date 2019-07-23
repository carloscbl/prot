#ifndef FORM_COLLECTOR_H
#define FORM_COLLECTOR_H
#include <string>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
using namespace std;

namespace fs = boost::filesystem;
using namespace fs;

//This class is intended for monitor the folder of forms to charge them dynamically
class form_collector{
private:
    vector<path> candidate_jsons_paths;
public:
    form_collector():
    candidate_jsons_paths()
    {
        fs::path full_path(fs::current_path());
        //std::cout << "Current path is : " << full_path << std::endl;
        path forms = path(path("..") / "forms");
        //cout << forms << endl;
        if(is_directory(forms)) {
            //std::cout << forms << " is a directory containing:\n";

            for(auto& entry : boost::make_iterator_range(directory_iterator(forms), {}))
            if(is_regular(entry)){
                path file_json(entry);
                if(file_json.extension() == ".json"){
                    //std::cout << file_json << "\n";
                    //std::cout << canonical(file_json) << "\n";
                    candidate_jsons_paths.push_back(canonical(file_json));
                }
            }
        }
        get_forms_paths();
    }
    virtual ~form_collector(){}

    vector<string> get_forms_paths () const noexcept{
        vector<string> ret;
        for_each(candidate_jsons_paths.begin(),candidate_jsons_paths.end(),[&ret](const path & p){
            ret.push_back(p.string());
        });
        for_each(ret.begin(), ret.end() ,[](const string& n) { std::cout << n << endl; });
        return ret;
    }
};

#endif //FORM_COLLECTOR_H