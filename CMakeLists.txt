cmake_minimum_required(VERSION 2.8.12)
cmake_policy(SET CMP0037 NEW)
project(chronoscopist)
set(Boost_ADDITIONAL_VERSIONS "1.XX" "1.XX.0")

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -fsanitize=leak -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
file(GLOB SOURCES  ${PROJECT_SOURCE_DIR}/src/*.cpp )
message(STATUS Mainsources: ${SOURCES})
include_directories( ${Boost_INCLUDE_DIR}  third-party )
#find_package(MySQL)
find_package( Boost 1.40 REQUIRED COMPONENTS program_options unit_test_framework log)
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
include_directories( ${Boost_INCLUDE_DIR} )
link_libraries( ${MYSQL_LIBRARY} ${Boost_LIBRARIES} pthread stdc++fs )
add_executable(chronoscopist-server ${PROJECT_SOURCE_DIR}/src/chronoscopist-server.cxx ${SOURCES})
add_executable(chronoscopist-client ${PROJECT_SOURCE_DIR}/src/chronoscopist-client.cxx ${SOURCES})

