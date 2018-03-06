#include <boost/program_options.hpp>

#include "config.h"
#include "parser.h"

using namespace dbproxy;

config config::from_args(int argc, const char *const *argv)
{
    using namespace boost::program_options;
    using namespace std;

    config cfg;

    options_description description("options");
    description.add_options()
        ("db-type", value<string>(&cfg.db_type)->required(), "datbase type")
        ("log-file", value<string>(&cfg.log_file)->default_value("dbproxy.log"), "log file")
        ("workers", value<boost::uint16_t>(&cfg.workers_count)->default_value(1), "workers count")
        ("upstream-ip", value<string>(&cfg.upstream_ip)->required(), "upstream IP")
        ("upstream-port", value<boost::uint16_t>(&cfg.upstream_port)->required(), "upstream port")
        ("local-ip", value<string>(&cfg.local_ip)->default_value("127.0.0.1"), "local IP")
        ("local-port ", value<boost::uint16_t>(&cfg.local_port)->required(), "local port")
    ;

    variables_map options;

    store(parse_command_line(argc, argv, description), options);
    notify(options);

    cfg.validate();

    return cfg;
}

void config::validate() throw()
{
    if (!workers_count)
    {
        throw std::invalid_argument("Workers count should be greater than zero");
    }

    if (!dbproxy::parser_registry::is_exists(db_type))
    {
        throw std::invalid_argument("Not supported database type");
    }

    // @TODO: validate params
}
