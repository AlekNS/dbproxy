#include <iostream>

#include "server.h"

using namespace dbproxy;
using namespace boost;

server::server(server::io_service &ios, const config cfg) : ios(ios),
                                                            conf(cfg),
                                                            local_acceptor(ios, server::endpoint(boost::asio::ip::address::from_string(cfg.local_ip), cfg.local_port)),
                                                            upstream_endpoint(boost::asio::ip::address::from_string(cfg.upstream_ip), cfg.upstream_port)
{
}

bool server::listen()
{
    try
    {
        auto current_connection = std::make_shared<connection>(ios);
        local_acceptor.async_accept(
            current_connection->get_local_socket(),
            [this, current_connection](const server::error_code &ec) {
                handle_accept(current_connection, ec);
            });
    }
    catch (const std::exception &e)
    {
        std::cerr << "DbProxy exception: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void server::handle_accept(std::shared_ptr<connection> current_connection, const server::error_code &ec)
{
    std::cout << "Accept" << std::endl;
    if (!ec)
    {
        current_connection->start(upstream_endpoint);
        if (!listen())
        {
            std::cerr << "Fail to listen." << std::endl;
        }
    }
    else
    {
        std::cerr << "Error: " << ec.message() << std::endl;
    }
}
