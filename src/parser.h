#ifndef __PARSER_H__
#define __PARSER_H__

#include <boost/cstdint.hpp>
#include <sstream>

#include "logger.h"
#include "instantiator.h"

namespace dbproxy
{

class parser;

/**
 * Parser creator interface.
 */
class parser_instantiator : public instantiator<parser, std::shared_ptr<logger>>
{
};

/**
 * Registry for parsers.
 * Not thread safe.
 */
class parser_registry : public registry<parser_instantiator>
{
};

/**
 * Parser interface.
 */
class parser
{
  public:
    virtual ~parser() = default;

    virtual void process_buffer(boost::uint8_t *buf, const size_t size) = 0;
};

/**
 * Concrete pgsql_simple_query instantiator.
 */
class parser_instantiator_pgsql_simple_query : public parser_instantiator
{
  public:
    virtual std::shared_ptr<parser> create(std::shared_ptr<logger> log) override;
};

/**
 * Postgresql package parser.
 */
class parser_pgsql_simple_query : public parser
{
  public:
    parser_pgsql_simple_query(std::shared_ptr<logger> logger);

    virtual void process_buffer(boost::uint8_t *buf, const size_t size) override;

    const std::string &get_last_query();

  private:
    enum
    {
        wait_startup_message,
        wait_commands
    } state;
    std::shared_ptr<logger> log;
    std::string last_query;
    std::string buffer;
};

}

#endif
