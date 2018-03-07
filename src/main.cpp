#include <boost/thread.hpp>

#include "server.h"
#include "logger.h"
#include "parser.h"

void register_parsers()
{
    dbproxy::parser_registry::register_it("pgsql", std::make_shared<dbproxy::parser_instantiator_pgsql_simple_query>());
}

void register_loggers(const dbproxy::config &cfg)
{
    dbproxy::logger_registry::register_it("common", std::make_shared<dbproxy::logger_instantiator_simple_out_file>(cfg.log_file));
}

int main(const int argc, const char *const *argv)
{
    try
    {
        register_parsers();

        auto cfg = dbproxy::config::from_args(argc, argv);

        register_loggers(cfg);

        boost::asio::io_service io_service(cfg.workers_count);

        std::cout << "DbProxy: "
                  << cfg.local_ip << ":" << cfg.local_port
                  << " -> "
                  << cfg.upstream_ip << ":" << cfg.upstream_port
                  << " log to "
                  << cfg.log_file
                  << " workers "
                  << cfg.workers_count
                  << std::endl;

        dbproxy::server server(io_service, cfg);
        if (!server.listen())
        {
            throw std::runtime_error("Can't start listening");
        }

        boost::thread_group workers_pool;
        for (size_t i = 0; i < cfg.workers_count; ++i)
        {
            workers_pool.create_thread([&io_service]() {
                io_service.run();
            });
        }

        std::cout << "Listen connections..." << std::endl;
        workers_pool.join_all();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
