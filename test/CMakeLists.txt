project(prot)

add_executable(test_prot main_test.cpp)
target_include_directories(test_prot PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty)