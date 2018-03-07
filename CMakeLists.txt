cmake_minimum_required(VERSION 3.5)
project(DbProxy)

find_package(Boost 1.62 COMPONENTS system thread program_options REQUIRED)

add_definitions( -DBOOST_ALL_DYN_LINK )
set(CMAKE_CXX_FLAGS_DEBUG "-DNDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(DbProxy_SOURCES
  src/main.cpp
  src/config.cpp
  src/server.cpp
  src/connection.cpp
  src/parser.cpp
  src/logger.cpp
)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(DbProxy ${DbProxy_SOURCES})

target_link_libraries(DbProxy ${Boost_LIBRARIES})

enable_testing()
add_subdirectory(tests)
