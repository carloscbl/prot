#ifndef BRANCH_H
#define BRANCH_H

#include <map>
#include <memory>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class branch
{



public:
  //This struct designate the source of actions that will be executed because of this branch election
  vector<string> * local_taskstory = nullptr;
  int next_node_id = 0;

  
  branch(vector<string> *local_taskstory, int next_node_id);
  virtual ~branch(){if(local_taskstory){delete local_taskstory;}}
};

#endif //BRANCH_H