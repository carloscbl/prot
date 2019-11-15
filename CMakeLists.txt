cmake_minimum_required(VERSION 3.10.0)

project(prot)

SET(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -Wno-missing-field-initializers -Wno-unused-parameter -Wno-unused-but-set-variable -std=c++17")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
FIND_PACKAGE( Boost 1.69 COMPONENTS program_options filesystem system REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
include_directories(${PROJECT_SOURCE_DIR}/source)
find_package(Threads REQUIRED)

include_directories("thirdparty/nlohmann_json")

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
