#include <boost/endian/conversion.hpp>

#include "parser.h"

using namespace dbproxy;

template <>
std::map<const std::string, std::shared_ptr<parser_instantiator>> parser_registry::registry::instantiators{};

std::shared_ptr<parser> parser_instantiator_pgsql_simple_query::create(std::shared_ptr<logger> log)
{
    return std::make_shared<parser_pgsql_simple_query>(log);
}

parser_pgsql_simple_query::parser_pgsql_simple_query(std::shared_ptr<logger> logger) : log(logger),
                                                                                       last_query(),
                                                                                       buffer(),
                                                                                       state(wait_startup_message)
{
}

const std::string &parser_pgsql_simple_query::get_last_query()
{
    return last_query;
}

void parser_pgsql_simple_query::process_buffer(boost::uint8_t *buf, const size_t size)
{
    const static int startupMessageDelta = 8;

    if (!size)
    {
        return;
    }

    buffer.append(std::string(reinterpret_cast<const char *>(buf), size));

    while (buffer.size() > 5)
    {
        // it's first connection
        if (state == wait_startup_message && buffer.size() > 15)
        {
            auto package_size = boost::endian::endian_reverse(*reinterpret_cast<const boost::uint32_t *>(buffer.c_str() + startupMessageDelta));

            if (buffer.size() >= package_size)
            {
                buffer = buffer.substr(package_size + startupMessageDelta);
                state = wait_commands;
                continue;
            }
        }
        // process commands
        if (state == wait_commands)
        {
            auto package_size = boost::endian::endian_reverse(*reinterpret_cast<const boost::uint32_t *>(buffer.c_str() + 1));
            if (buffer.size() >= package_size)
            {
                if (buffer[0] == 'Q') // PgSql Simple Query
                {
                    last_query = buffer.substr(5, package_size - 5);
                    log->out(last_query);
                }
                buffer = buffer.substr(std::min(static_cast<size_t>(package_size) + 1, buffer.size()));
                continue;
            }
        }
        break;
    }
}
