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
class form_collector
{
private:
    vector<path> candidate_jsons_paths;

public:
    form_collector() : candidate_jsons_paths()
    {
        fs::path full_path(fs::current_path());
        path forms = path(path("..") / "forms");
        if (is_directory(forms))
        {
            for (auto &entry : boost::make_iterator_range(directory_iterator(forms), {}))
                if (is_regular(entry))
                {
                    path file_json(entry);
                    if (file_json.extension() == ".json")
                    {
                        candidate_jsons_paths.push_back(canonical(file_json));
                    }
                }
        }
    }
    virtual ~form_collector() {}

    vector<string> get_forms_paths()
    {
        vector<string> ret;
        //for_each(candidate_jsons_paths.cbegin(), candidate_jsons_paths.cend() ,[]( string n) { std::cout << n << endl; });
        for (auto p : candidate_jsons_paths)
        {
            ret.push_back(p.string());
        }
        return ret;
    }
};

#endif //FORM_COLLECTOR_H