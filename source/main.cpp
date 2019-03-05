#include <iostream>
#include <ctime>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>
#include <cstring>
#include <functional>
#include <vector>
#include <memory>
#include "command_processor.h"
#include "task.h"
#include "form.h"
#include "iactionable.h"




#ifdef PROTOBUF_RUNNING
  #include "task.pb.h"
#endif
#define p(X) std::cout <<  X  <<std::endl;

using namespace std;




int main(int argc, char const *argv[])
{
  /*Utilization
  prot task <Name> <Description> <CreationDate>
  */
  /*prototipe of utilization commands and inmediately the input so...  
  task -> "Setting a task"  
    n name
    d day
    m month
    y year
    D Description
    u username
    h hour
    M minute
    So -MhuDndmy 16 00 albertRiv "Not in home but..." "Call Assitant to walk with the dog" 22 12 2019
  */

  command_processor cp;
  task task_ ;
  form form_;
  cp.register_actionable("task", &task_);
  cp.register_actionable("form", &form_);





  vector<string> arg_i(argv + 1, argv + argc);

  /*for(auto e : arg_i){
    p(e);
  }*/
  p("Starting program \n\n");
  cp.perform_command(arg_i);
//  for(auto && e : env::tasker){
//    cout << e.first << "-:-" << e.second->id <<endl;
//    e.second->print_();
//  }
  p("write \"exit\" to finish");
    while (true){

      string st;
      getline(cin,st);
      if(st == "exit"){
        break;
      }
      cp.perform_command(st);
    }
  
    // GOOGLE_PROTOBUF_VERIFY_VERSION;
    // /* code */
    // prot::task tk;
    // tk.set_id(25);
    // tk.set_stamp_creation(static_cast<int64_t>(std::time(0)));
    // tk.set_stamp_end(static_cast<int64_t>(std::time(0)));
    // prot::general_priority priority;

    // priority = prot::general_priority::not_urgent;

    // cout << priority << endl;
    // fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    // if (!tk.SerializeToOstream(&output)) {
    //   cerr << "Failed to write address book." << endl;
    //   return -1;
    // }
    // p("prot bye")
    return 0;
}
