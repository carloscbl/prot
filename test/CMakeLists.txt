project(prot_test)

file(GLOB TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/*.h
    ${PROJECT_SOURCE_DIR}/*.cpp
)
message(${PROJECT_SOURCE_DIR})
message(${TEST_SOURCES})

add_executable(test_prot main_test.cpp ${TEST_SOURCES})
target_include_directories(test_prot PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty)