#include "parser.h"

using namespace dbproxy;

template<> std::map<const std::string, std::shared_ptr<parser_instantiator>> parser_registry::registry::instantiators{};

std::shared_ptr<parser> parser_instantiator_pgsql::create()
{
    return std::make_shared<parser_pgsql>();
}


// parser_pgsql::parser_pgsql()
// {

// }
