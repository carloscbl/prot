#include "task.h"

using namespace std;

task::task():dateUTC(time(nullptr)){
  static int acummulator = 0;
  id = "task" + to_string(acummulator++);
}

task::~task(){
  cout << "~destroying " << this->id <<endl;
}