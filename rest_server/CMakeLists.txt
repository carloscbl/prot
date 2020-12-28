cmake_minimum_required (VERSION 3.2)

project(api-server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pg -ggdb -g3 -rdynamic" )

include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external)
SET(PISTACHE_TEST "OFF")
SET(BUILD_TESTING "OFF")
SET(JSON_BuildTests "OFF")
SET(FMT_TEST "OFF")

ExternalProject_Add(PISTACHE
   GIT_REPOSITORY https://github.com/oktal/pistache.git
   GIT_TAG 4e48b6d3da2bc5143e906c5bb80e1a4f6b6e2376
   CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

# ExternalProject_Add(NLOHMANN
#     GIT_REPOSITORY https://github.com/nlohmann/json.git
#     GIT_TAG master
#     CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
# )

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/api/generated)
include_directories(model)
include_directories(api)
include_directories(impl)
# find_package(fmt REQUIRED)

file(GLOB SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/api/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/impl/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/model/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_executable(${PROJECT_NAME} ${SRCS} )
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty)
add_dependencies(${PROJECT_NAME} PISTACHE )
target_link_libraries( ${PROJECT_NAME} pistache pthread lprot fmt::fmt sqlpp11 sqlpp-mysql mariadb ${Boost_LIBRARIES} )
