#include <iostream>

#include "server.h"

using namespace dbproxy;
using namespace boost;

using address = boost::asio::ip::address;

server::server(server::io_service &ios, const config cfg) : ios(ios),
                                                            conf(cfg),
                                                            local_acceptor(ios, server::endpoint(address::from_string(cfg.local_ip), cfg.local_port)),
                                                            upstream_endpoint(address::from_string(cfg.upstream_ip), cfg.upstream_port)
{
}

bool server::listen()
{
    try
    {
        auto local_logger = logger_registry::get("common")->create();
        auto pgsql_local_parser = parser_registry::get(conf.db_type);

        auto current_connection = std::make_shared<connection>(ios, pgsql_local_parser->create(local_logger));
        local_acceptor.async_accept(
            current_connection->get_local_socket(),
            [this, current_connection](const server::error_code &ec) {
                if (!ec)
                {
                    current_connection->start(upstream_endpoint);
                    if (!listen())
                    {
                        std::cerr << "Fail to listen local socket." << std::endl;
                    }
                }
                else
                {
                    std::cerr << "Error was occurred: " << ec.message() << std::endl;
                }
            });
    }
    catch (const std::exception &e)
    {
        std::cerr << "DbProxy exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}
