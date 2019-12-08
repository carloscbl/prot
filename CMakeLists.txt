cmake_minimum_required(VERSION 3.10.0)

project(prot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -Wno-missing-field-initializers -Wno-unused-parameter -Wno-unused-but-set-variable ")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
FIND_PACKAGE( Boost 1.69 COMPONENTS program_options filesystem system REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
include_directories(${PROJECT_SOURCE_DIR}/source)
find_package(Threads REQUIRED)
find_package(Sqlpp11 REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/api/generated)
include_directories("thirdparty/nlohmann_json")

add_subdirectory(test)
add_subdirectory(rest_server)



file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/source/*.h
    ${PROJECT_SOURCE_DIR}/source/*.cpp
    ${PROJECT_SOURCE_DIR}/thirdparty/nlohmann_json/json.hpp
)
message(${SOURCES})



add_library (lprot ${PROJECT_SOURCE_DIR}/source/main.cpp ${SOURCES} )
add_executable (prot ${PROJECT_SOURCE_DIR}/source/main.cpp ${SOURCES} )
TARGET_LINK_LIBRARIES( prot LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(prot PRIVATE sqlpp11 sqlpp-mysql mariadb)