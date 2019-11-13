cmake_minimum_required(VERSION 3.10.0)

project(prot)

SET(CMAKE_CXX_FLAGS "-g -Wall -Werror -Wno-unused-parameter -Wno-unused-but-set-variable -std=c++17")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
FIND_PACKAGE( Boost 1.69 COMPONENTS program_options filesystem system REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
include_directories(${PROJECT_SOURCE_DIR}/source)
find_package(Threads REQUIRED)
#include_directories("thirdparty/mongocxx/mongocxx/include")
include_directories("thirdparty/nlohmann_json")
# NOTE: For this to work, the CMAKE_PREFIX_PATH variable must be set to point to the directory that
# was used as the argument to CMAKE_INSTALL_PREFIX when building libmongocxx.

FIND_PACKAGE(libmongocxx REQUIRED)

FIND_PACKAGE(libbsoncxx REQUIRED)

# INCLUDE_DIRECTORIES(${LIBMONGOCXX_INCLUDE_DIR})

# INCLUDE_DIRECTORIES(${LIBBSONCXX_INCLUDE_DIR})


INCLUDE_DIRECTORIES("thirdparty/mongocxx/mongocxx/include/mongocxx/v_noabi")

INCLUDE_DIRECTORIES("thirdparty/mongocxx/mongocxx/include/bsoncxx/v_noabi")

# INCLUDE_DIRECTORIES("/usr/local/include/libmongoc-1.0")

# INCLUDE_DIRECTORIES("/usr/local/include/libbson-1.0")


add_subdirectory(test)



file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/source/*.h
    ${PROJECT_SOURCE_DIR}/source/*.cpp
    ${PROJECT_SOURCE_DIR}/thirdparty/nlohmann_json/json.hpp
)
message(${SOURCES})



add_library (lprot ${PROJECT_SOURCE_DIR}/source/main.cpp ${SOURCES} )
add_executable (prot ${PROJECT_SOURCE_DIR}/source/main.cpp ${SOURCES} )
TARGET_LINK_LIBRARIES( prot LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries( prot PRIVATE mongocxx)
target_link_libraries( prot PRIVATE bsoncxx)


