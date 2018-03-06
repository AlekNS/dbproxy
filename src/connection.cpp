#include <boost/bind.hpp>

#include "connection.h"
#include "handler_caller.h"

namespace dbproxy
{

connection::connection(connection::io_service &ios) : ios(ios),
                                                      local_sock(ios),
                                                      upstream_sock(ios)
{
}

connection::socket &connection::get_local_socket()
{
    return local_sock;
}

connection::socket &connection::get_upstream_socket()
{
    return upstream_sock;
}

void connection::start(const connection::endpoint &upstream_endpoint)
{
    upstream_sock.async_connect(
        upstream_endpoint,
        boost::bind(&connection::handle_connection, shared_from_this(), boost::asio::placeholders::error));
}

void connection::handle_connection(const connection::error_code &ec)
{
    if (!ec)
    {
        upstream_sock.async_read_some(
            boost::asio::buffer(upstream_buffer, def_buffer_size),
            make_custom_handler(
                upstream_socket_allocator,
                boost::bind(
                    &connection::handle_upstream_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred)));

        local_sock.async_read_some(
            boost::asio::buffer(local_buffer, def_buffer_size),
            make_custom_handler(
                local_socket_allocator,
                boost::bind(
                    &connection::handle_local_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred)));
    }
    else
    {
        close();
    }
}

void connection::handle_local_write(const connection::error_code &ec)
{
    if (!ec)
    {
        upstream_sock.async_read_some(
            boost::asio::buffer(upstream_buffer, def_buffer_size),
            make_custom_handler(
                upstream_socket_allocator,
                boost::bind(
                    &connection::handle_upstream_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred)));
    }
    else
    {
        close();
    }
}

void connection::handle_local_read(const connection::error_code &ec, const std::size_t bytes_transferred)
{
    if (!ec)
    {
        boost::asio::async_write(
            upstream_sock,
            boost::asio::buffer(local_buffer, bytes_transferred),
            make_custom_handler(
                upstream_socket_allocator,
                boost::bind(
                    &connection::handle_upstream_write,
                    shared_from_this(),
                    boost::asio::placeholders::error)));
    }
    else
    {
        close();
    }
}

void connection::handle_upstream_write(const connection::error_code &ec)
{
    if (!ec)
    {
        local_sock.async_read_some(
            boost::asio::buffer(local_buffer, def_buffer_size),
            make_custom_handler(
                local_socket_allocator,
                boost::bind(
                    &connection::handle_local_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred)));
    }
    else
    {
        close();
    }
}

void connection::handle_upstream_read(const connection::error_code &ec, const std::size_t bytes_transferred)
{
    if (!ec)
    {
        boost::asio::async_write(
            local_sock,
            boost::asio::buffer(upstream_buffer, bytes_transferred),
            make_custom_handler(
                local_socket_allocator,
                boost::bind(
                    &connection::handle_local_write,
                    shared_from_this(),
                    boost::asio::placeholders::error)));
    }
    else
    {
        close();
    }
}

void connection::close()
{
    std::lock_guard<std::mutex> lock(mtx);

    auto close_socket = [](connection::socket &socket) {
        if (socket.is_open())
        {
            connection::error_code ec;
            socket.shutdown(connection::socket::shutdown_both, ec);
            socket.close(ec);
        }
    };

    close_socket(upstream_sock);
    close_socket(local_sock);
}

}
