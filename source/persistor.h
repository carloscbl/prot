#ifndef PERSISTOR_H
#define PERSISTOR_H

#include <memory>
/*design
    this class is intended to be an abstraction of serialization to json,
    regardless the configuration to persist on disk or network db like mongodb
*/
using std::shared_ptr;
using std::unique_ptr;

class persistor
{
private:
    /* data */
    static unique_ptr<persistor> persistor_instance;
public:
    persistor(/* args */);
    ~persistor();
    static persistor & get_persistor_instance();
};


#endif //PERSISTOR_H
