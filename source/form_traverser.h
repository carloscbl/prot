//
// Created by carlos on 2/03/19.
//

#ifndef FORM_TRAVERSER_H
#define FORM_TRAVERSER_H

#include <map>
#include <memory>

using namespace std;

struct question_node{
    int id = 0;
    string Question;
    string Answer;
    string task;
    map<string,int> branches;//"string:Yes -> id 50 is the next Question"
    //set<string> tags;
};

enum class UsualStates : int {
  exit = -1
};

class form_traverser {
private:
    using question_node_id = int;
    const map<question_node_id, unique_ptr<question_node> > qmap;



    UsualStates us = UsualStates::exit;
public:

    form_traverser(map<question_node_id, unique_ptr<question_node> > && questions);

    void run();
    void process_branches_outputs(unique_ptr<question_node> node) const noexcept;
};


#endif //FORM_TRAVERSER_H
