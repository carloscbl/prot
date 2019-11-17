project(test_prot)


SET(CMAKE_CXX_FLAGS "-g -Wall -Wno-unused-parameter -Wno-unused-but-set-variable -std=c++17")

file(GLOB TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/*.h
    ${PROJECT_SOURCE_DIR}/*.cpp
    # ${CMAKE_SOURCE_DIR}/source/*.h
    # ${CMAKE_SOURCE_DIR}/source/*cpp
)
message(${CMAKE_SOURCE_DIR})
message(${PROJECT_SOURCE_DIR}/..)
message(${TEST_SOURCES})

# set(LIB_DIR "${PROJECT_SOURCE_DIR}/libs")
# set(HinnantDate_ROOT_DIR "${LIB_DIR}/date/include")
# set(SQLite3_DIR "${LIB_DIR}/libsqlite3/cmake")
# set(Sqlpp11_DIR "${LIB_DIR}/sqlpp11")
# set(Sqlpp11Sqlite3_DIR "${LIB_DIR}/sqlpp11-connector-sqlite3")
# set(Hashidsxx_DIR "${LIB_DIR}/hashidsxx")

include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/api/generated)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
enable_testing()

find_package(Sqlpp11 REQUIRED)
#target_include_directories(sqlpp11 PRIVATE ${sqlpp11_SOURCE_DIR}/include)
#include_directories(${Sqlpp11_DIR})
add_executable(test_prot ${TEST_SOURCES})
target_include_directories(test_prot PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(test_prot PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty)
TARGET_LINK_LIBRARIES( test_prot LINK_PUBLIC lprot  ${Boost_LIBRARIES})
target_link_libraries(test_prot PRIVATE sqlpp11)
target_link_libraries(test_prot PRIVATE sqlpp-mysql)
target_link_libraries(test_prot PRIVATE mariadb)
#TARGET_LINK_LIBRARIES( test_prot )
#target_include_directories(test_prot PUBLIC ../externals/catch2/)
 
add_test(
  NAME test_prot
  COMMAND test_prot #-o report.xml -r junit
)