//
// Created by carlos on 2/03/19.
//

#ifndef ICOMMANDABLE_H
#define ICOMMANDABLE_H
#include <string>
#include <vector>

using namespace std;
class icommandable
{
private:
    /* data */
protected:
    //
public:
    virtual void perform_command(string s) = 0;
    virtual void perform_command(vector<string> s) = 0;
};

#endif //ICOMMANDABLE_H