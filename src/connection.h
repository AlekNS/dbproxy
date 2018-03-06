#ifndef __SESSION_H__
#define __SESSION_H__

#include <mutex>
#include <boost/asio.hpp>

#include "parser.h"
#include "handler_alloc.h"

namespace dbproxy
{

/**
 * Connection
 */
class connection : public std::enable_shared_from_this<connection>
{
  public:
    enum
    {
        def_buffer_size = 1024 * 6
    };
    using error_code = boost::system::error_code;
    using io_service = boost::asio::io_service;
    using endpoint = boost::asio::ip::tcp::endpoint;
    using socket = boost::asio::ip::tcp::socket;
    using buffer = std::array<uint8_t, def_buffer_size>;

    connection(io_service &ios);
    virtual ~connection() = default;

    void start(const endpoint &down_endpoint);

    void handle_connection(const error_code &ec);

    socket &get_upstream_socket();
    socket &get_local_socket();

  private:
    void close();

    void handle_local_write(const error_code &ec);
    void handle_local_read(const error_code &ec,
                           const size_t bytes_transferred);

    void handle_upstream_write(const error_code &ec);
    void handle_upstream_read(const error_code &ec,
                              const size_t bytes_transferred);

    io_service &ios;
    std::mutex mtx;

    buffer upstream_buffer;
    buffer local_buffer;
    socket upstream_sock;
    socket local_sock;
    handler_allocator<def_buffer_size> upstream_socket_allocator;
    handler_allocator<def_buffer_size> local_socket_allocator;
};

}

#endif
