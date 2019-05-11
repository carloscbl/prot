cmake_minimum_required(VERSION 3.10.0)

project(prot)

SET(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++17")
#SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++17")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
FIND_PACKAGE( Boost 1.69 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
include_directories(${PROJECT_SOURCE_DIR}/source)
find_package(Threads REQUIRED)

include_directories("thirdparty/nlohmann_json")

SET(protobuf_ false)
if(protobuf_)
    add_compile_definitions(PROTOBUF_RUNNING)
    message("My src = ${PROJECT_SOURCE_DIR}/source")
    message("My src = ${PROTOBUF_INCLUDE_DIR}")

    find_package(Protobuf REQUIRED)
    if(PROTOBUF_FOUND)
        message ("protobuf found")
    else()
        message (FATAL_ERROR "Cannot find Protobuf")
    endif()

    PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${PROJECT_SOURCE_DIR}/source/task.proto)

    # Print path to generated files
    message ("PROTO_SRCS = ${PROTO_SRCS}")
    message ("PROTO_HDRS = ${PROTO_HDRS}")

    add_executable (prot ${PROJECT_SOURCE_DIR}/source/main.cpp ${PROTO_SRCS} ${PROTO_HDRS})
    TARGET_LINK_LIBRARIES(prot  ${PROTOBUF_LIBRARY})
else()
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/source/*.h
    ${PROJECT_SOURCE_DIR}/source/*.cpp
    ${PROJECT_SOURCE_DIR}/thirdparty/nlohmann_json/json.hpp
)
message(${SOURCES})

    add_executable (prot ${PROJECT_SOURCE_DIR}/source/main.cpp ${SOURCES} )
    TARGET_LINK_LIBRARIES( prot LINK_PUBLIC ${Boost_LIBRARIES})

endif()


