//
// Created by carlos on 2/03/19.
//

#ifndef IACTIONABLE_H
#define IACTIONABLE_H

#include <map>
#include <string>

class iactionable {
  public:
  virtual void send_action(std::string action ,std::map<char,std::string> params) = 0;
};


#endif //IACTIONABLE_H
