#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE PgsqlParserTests

#include <boost/test/unit_test.hpp>
#include <parser_pgsql.h>

namespace bt = boost::unit_test;

BOOST_AUTO_TEST_CASE( ParseSimpleQueryAndReturnTrue )
{
    dbproxy::parser_pgsql parser;

    // BOOST_CHECK_EQUAL(5, 5);
    BOOST_TEST( true );
}

BOOST_AUTO_TEST_CASE( TryParserInvalidDataAndReturnFalse )
{
    dbproxy::parser_pgsql parser;

    // BOOST_CHECK_EQUAL(5, 5);
    BOOST_TEST( true );
}
