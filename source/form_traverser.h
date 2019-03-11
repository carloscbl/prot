//
// Created by carlos on 2/03/19.
//

#ifndef FORM_TRAVERSER_H
#define FORM_TRAVERSER_H

#include <map>

using namespace std;

struct question_node{
    int id = 0;
    string Question;
    string Answer;
    string task;
    map<string,int> branches;//"string:Yes -> id 50 is the next Question"
    //set<string> tags;
};

class form_traverser {
private:
    using question_node_id = int;
    const map<question_node_id, unique_ptr<question_node> > qmap;

    enum class UsualStates : int {
        exit = -1
    };

    UsualStates us = UsualStates::exit;
public:
    form_traverser();

    void run();
};


#endif //FORM_TRAVERSER_H
