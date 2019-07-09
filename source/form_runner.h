#ifndef FORM_RUNNER_H
#define FORM_RUNNER_H
#include <chrono>
#include "iuser.h"
#include "form.h"

using namespace std::chrono_literals;

//So this class will handle the call form_run from form_parser
//On construction it will check for the current form being evaluated prevoiously and unfinnised
//So will find for a existing session and restore it and handle in a threaded way the life time of this session
//Probably a thread pool is a good idea an a queue, but for now, we will start new threads for simplicity
//This will requiere a iuser and a form name to find if the user have currently existing one
//If not we will create a session and store it
class form_runner
{
private:
    /* data */
    const chrono::minutes life_time = 5min;
    iuser & user;
    form & form_;
public:
    form_runner(iuser user, form form_);
    ~form_runner();
};

form_runner::form_runner(iuser user, form form_):
user(user),
form_(form_)
{
    //Create thread
    //Check serialiced session or create a new one
    //Perform or traverse the questionary

}

form_runner::~form_runner()
{
}


#endif //FORM_RUNNER_H