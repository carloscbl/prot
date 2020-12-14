cmake_minimum_required(VERSION 3.10.0)

project(prot)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "-pg -ggdb -g3 -Wall -Wextra -Werror -Wno-missing-field-initializers -Wno-unused-parameter -Wunused-variable -Wno-unused-private-field ")
add_definitions(-Wno-deprecated)
SET(FMT_TEST "OFF")
SET(PISTACHE_TEST "OFF")
SET(BUILD_TESTING "OFF")
SET(JSON_BuildTests "OFF")
SET(FMT_TEST "OFF")
SET(SPDLOG_FMT_EXTERNAL_HO "ON")
SET(SPDLOG_FMT_EXTERNAL "ON")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
FIND_PACKAGE( Boost 1.69 COMPONENTS program_options filesystem system REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
include_directories(${PROJECT_SOURCE_DIR}/source)
find_package(Threads REQUIRED)
find_package(Sqlpp11 REQUIRED)



include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/api/generated)
include_directories("thirdparty/nlohmann_json")
# include_directories("thirdparty/spdlog")
# thirdparty/HowardHinnantDate/date/include/date/tz.h
# include_directories("thirdparty/HowardHinnantDate")
# include_directories("thirdparty/HowardHinnantDate/date")
include_directories("thirdparty/HowardHinnantDate/date/include")
add_subdirectory(fmt) # Order matters for subdirectories
# add_subdirectory(spdlog) # Order matters for subdirectories

ExternalProject_Add(spdlog
    PREFIX ${CMAKE_BINARY_DIR}/thirdparty/spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    # GIT_TAG "v1.6.1"
    TIMEOUT 10
    CONFIGURE_COMMAND ""
    CMAKE_ARGS "-DSPDLOG_FMT_EXTERNAL=ON -DDSPDLOG_FMT_EXTERNAL:string=ON"
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)

add_subdirectory(rest_server)
add_subdirectory(test)
add_subdirectory(jobs_watcher)



file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/source/*.h
    ${PROJECT_SOURCE_DIR}/source/*.cpp
    ${PROJECT_SOURCE_DIR}/thirdparty/nlohmann_json/json.hpp
    ${PROJECT_SOURCE_DIR}/thirdparty/HowardHinnantDate/date/src/tz.cpp
)
message(${SOURCES})


add_library (lprot  ${SOURCES} ) #${PROJECT_SOURCE_DIR}/source/main.cpp
TARGET_LINK_LIBRARIES( lprot LINK_PUBLIC pthread ${Boost_LIBRARIES} fmt::fmt curl  )