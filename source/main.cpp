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
#include "form_reader.h"
#include "form_parser.h"
#include "form_collector.h"
#include "request.h"

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
    So -MhuDndmy 16 00 alberto "Not in home but..." "Call Assitant to walk with the dog" 22 12 2019
  */
  form_collector fc;
  command_processor cp;
  task task_;

  std::vector<std::string> forms_paths;
  forms_paths =  fc.get_forms_paths();

  form form_;
  request request_;
  cp.register_actionable("task", &task_);
  cp.register_actionable("form", &form_);
  cp.register_actionable("req", &request_);
  
//////////////////////////////////////////////////////
//// COMMANDS ARE NOW PERFORMABLE
//////////////////////////////////////////////////////

  for_each(forms_paths.begin(), forms_paths.end(), [&cp](const string & s){
    cp.perform_command("form add -P " + s);
  });

//////////////////////////////////////////////////////
  vector<string> arg_i(argv + 1, argv + argc);

  p("Starting program \n\n");
  cp.perform_command(arg_i);

  p("write \"exit\" to finish");
  while (true){

    string st;
    getline(cin,st);
    if(st == "exit"){
      break;
    }
    cp.perform_command(st);
  }

  return 0;
}
