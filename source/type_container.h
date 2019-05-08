#ifndef TYPE_CONTAINER_H
#define TYPE_CONTAINER_H
#include <string>

using namespace std;

template<typename T> //The value from the json can come converted, or we would directly want to filter it
class type_container{
public:
    type_container(string type, T value );
    T value;
    string type;
    bool valid = false;
};

#endif //TYPE_CONTAINER_H