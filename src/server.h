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

    virtual ~server() = default;

    /**
     * Init server configuration.
     */
    server(io_service &ios, const config cfg);

    /**
     * Accept connections
     */
    bool listen();

  protected:

    config conf;
    io_service &ios;
    endpoint upstream_endpoint;
    acceptor local_acceptor;
};

}

#endif
