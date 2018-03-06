#ifndef __PARSER_H__
#define __PARSER_H__

#include <map>
#include <memory>

#include "instantiator.h"

namespace dbproxy
{

class parser;

/**
 * Parser creator interface.
 */
class parser_instantiator : public instantiator<parser> { };

/**
 * Registry for parsers.
 * Not thread safe.
 */
class parser_registry : public registry<parser_instantiator> { };

/**
 * Parser interface.
 */
class parser
{
  public:
    virtual ~parser() = default;

    // virtual void process_buffer(buffer buf);
    // virtual bool is_ready() = 0;
    // virtual unique<stringstream> get_stream();
};

/**
 * Concrete pgsql instantiator.
 */
class parser_instantiator_pgsql : public parser_instantiator
{
public:
    virtual std::shared_ptr<parser> create() override;
};

/**
 * Postgresql package parser.
 */
class parser_pgsql: public parser
{
public:

private:

};

}

#endif
