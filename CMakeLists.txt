cmake_minimum_required(VERSION 2.8.12)
cmake_policy(SET CMP0037 NEW)
project(chronoscopist)
set(Boost_ADDITIONAL_VERSIONS "1.XX" "1.XX.0")

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -fsanitize=leak -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

execute_process(COMMAND mysql_config --cflags
    OUTPUT_VARIABLE MYSQL_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND mysql_config --libs
    OUTPUT_VARIABLE MYSQL_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)


#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
file(GLOB SOURCES  ${PROJECT_SOURCE_DIR}/src/*.cpp )
message(STATUS Mainsources: ${SOURCES})
include_directories( ${Boost_INCLUDE_DIR}  third-party )
include_directories(${MYSQL_INCLUDE_DIR})
find_package( Boost 1.40 REQUIRED COMPONENTS program_options unit_test_framework log)
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
include_directories( ${Boost_INCLUDE_DIR} )
link_libraries( ${MYSQL_LIBRARY} ${Boost_LIBRARIES} pthread stdc++fs ${MYSQL_LIBS} )
add_executable(chronoscopist-server ${PROJECT_SOURCE_DIR}/src/chronoscopist-server.cxx ${SOURCES})
add_executable(chronoscopist-client ${PROJECT_SOURCE_DIR}/src/chronoscopist-client.cxx ${SOURCES})

