project(test_prot)


SET(CMAKE_CXX_FLAGS "-pg -ggdb -g3  -Wall -Wno-unused-parameter -std=c++17")
SET(FMT_TEST "OFF")
SET(PISTACHE_TEST "OFF")
SET(BUILD_TESTING "OFF")
SET(JSON_BuildTests "OFF")
SET(FMT_TEST "OFF")
file(GLOB TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/*.h
    ${PROJECT_SOURCE_DIR}/*.cpp
    # ${CMAKE_SOURCE_DIR}/source/*.h
    # ${CMAKE_SOURCE_DIR}/source/*cpp
)
message(${CMAKE_SOURCE_DIR})
message(${PROJECT_SOURCE_DIR}/..)
message(${TEST_SOURCES})
# find_package(fmt)
# SET(FMT_TEST "false")

include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/api/generated)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
enable_testing()

add_executable(test_prot ${TEST_SOURCES})
target_include_directories(test_prot PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(test_prot PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty)
TARGET_LINK_LIBRARIES( test_prot LINK_PUBLIC lprot  ${Boost_LIBRARIES} fmt::fmt )
target_link_libraries( test_prot PRIVATE sqlpp11 sqlpp-mysql mariadb)
 
add_test(
  NAME test_prot
  COMMAND test_prot #-o report.xml -r junit
)