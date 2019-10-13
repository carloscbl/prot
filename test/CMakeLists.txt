project(test_prot)


SET(CMAKE_CXX_FLAGS "-g -Wall -Werror -Wno-unused-parameter -Wno-unused-but-set-variable -std=c++17")

file(GLOB TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/*.h
    ${PROJECT_SOURCE_DIR}/*.cpp
    # ${CMAKE_SOURCE_DIR}/source/*.h
    # ${CMAKE_SOURCE_DIR}/source/*cpp
)
message(${CMAKE_SOURCE_DIR})
message(${PROJECT_SOURCE_DIR}/..)
message(${TEST_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/source)

enable_testing()


add_executable(test_prot ${TEST_SOURCES})
target_include_directories(test_prot PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(test_prot PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty)
TARGET_LINK_LIBRARIES( test_prot lprot)
#target_include_directories(test_prot PUBLIC ../externals/catch2/)
 
add_test(
  NAME test_prot
  COMMAND test_prot #-o report.xml -r junit
)