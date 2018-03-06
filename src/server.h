#ifndef __SERVER_H__
#define __SERVER_H__

#include <boost/asio.hpp>
#include "config.h"
#include "connection.h"

namespace dbproxy
{

/**
 * Proxy server
 */
class server : private boost::noncopyable
{
  public:
    using error_code = boost::system::error_code;
    using endpoint = boost::asio::ip::tcp::endpoint;
    using acceptor = boost::asio::ip::tcp::acceptor;
    using io_service = boost::asio::io_service;

    server(io_service &ios, const config cfg);

    virtual ~server() = default;

    /**
     * Start accept connections
     */
    bool listen();

  protected:
    // handle connection
    void handle_accept(std::shared_ptr<connection> conn, const error_code &error);

    config conf;
    io_service &ios;
    endpoint upstream_endpoint;
    acceptor local_acceptor;
};

}

#endif
