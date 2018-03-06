cmake_minimum_required(VERSION 3.5)
project(DbProxy)

find_package(Boost 1.62 COMPONENTS system thread unit_test_framework REQUIRED)
set(Boost_USE_STATIC_LIBS ON)
add_definitions (-DBOOST_TEST_DYN_LINK)
add_definitions (-DBOOST_ALL_DYN_LINK)

include_directories (../src ${Boost_INCLUDE_DIRS})

add_executable(utest test_pgsql_parser.cpp)

target_link_libraries(utest ${Boost_LIBRARIES}) # ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

enable_testing()
add_test(NAME MyTest COMMAND utest)
