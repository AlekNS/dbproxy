cmake_minimum_required(VERSION 3.5)
project(DbProxy)

find_package(Boost 1.62 COMPONENTS system thread unit_test_framework REQUIRED)
set(Boost_USE_STATIC_LIBS ON)
add_definitions (-DBOOST_TEST_DYN_LINK)
add_definitions (-DBOOST_ALL_DYN_LINK)

include_directories (../src ${Boost_INCLUDE_DIRS})

add_executable(test_pgsql_parser
    test_pgsql_parser.cpp
    ../src/parser.cpp
)

target_link_libraries(test_pgsql_parser ${Boost_LIBRARIES}) # ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(NAME PgSqlParser COMMAND test_pgsql_parser)
