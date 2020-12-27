
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/algorithm/string.hpp>

namespace prot::specifics{

inline std::string get_uuid(){
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    std::string prot_id = boost::uuids::to_string(uuid);
    boost::erase_all(prot_id, "-");
    return prot_id;
}

}; //prot::specifics