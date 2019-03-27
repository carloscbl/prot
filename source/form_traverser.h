//
// Created by carlos on 2/03/19.
//

#ifndef FORM_TRAVERSER_H
#define FORM_TRAVERSER_H

#include <map>
#include <memory>

#include <iostream>
#include <vector>
#include <string>

using namespace std;
struct branch
{

  vector<string> local_taskstory;
  int next_node_id;
  //map<string,string> params;
};
struct question_node{
    using posible_answer_given = string;

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
    map<question_node_id, question_node > * qmap = nullptr;



    UsualStates us = UsualStates::exit;
public:

    form_traverser(map<question_node_id, question_node> * questions):qmap(questions){}
    form_traverser() = default;
    ~form_traverser(){
      delete qmap;
    }
    void run();
    void process_branches_outputs(question_node node) const noexcept;

    void print_out(){
      if(!qmap) return;
      for (auto &e : *qmap)
      {
        //
        cout << e.second.id << ":" << e.second.Question << ":" << e.second.Answer << ":" << ":" << *e.second.branches.begin()->second.local_taskstory.begin()
         << ":" << e.second.branches.begin()->second.next_node_id << endl;
      }
    }
};


#endif //FORM_TRAVERSER_H
