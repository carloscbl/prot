#ifndef TEST_H
#define TEST_H

#include <fstream>
#include "command_processor.h"
#include "CRUD_actionable.h"

class test final : public CRUD_actionable<test>
{
private:
    command_processor & cp;
    CRUD_plus_actions_map test_map{
        {"sequence", [this](map<char, string> s) {
            perform_sequence_of(s['p']);
        }},
    };
    //Modulator params like -h -n -P
    map_local_functions setters{};

public:
    test(command_processor & cp);
    ~test() = default;
    void perform_sequence_of(const string & file_path);
};

void test::perform_sequence_of(const string & file_path){
    std::ifstream i(file_path);
    std::string line;
    while (std::getline(i, line))
    {
        if(line[0] == '#') continue;
        this->cp.perform_command(line);
    }
}

test::test(command_processor & cp):CRUD_actionable(test_map, setters), cp(cp)
{
}


#endif //TEST_H