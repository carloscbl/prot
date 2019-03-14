//
// Created by carlos on 2/03/19.
//

#ifndef FORM_TRAVERSER_H
#define FORM_TRAVERSER_H

#include <map>
#include <memory>

#include <vector>

using namespace std;

struct question_node{
    using posible_answer_given = string;
    struct branch
    {
        unique_ptr<vector<string> > local_taskstory;
        int next_node_id;
        //map<string,string> params;
    };
    int id = 0;
    string Question;
    string Answer;

    map<posible_answer_given,branch> branches;//"yes" : branch {"task add -n washer", UsualStates::exit};
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
