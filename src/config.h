#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <boost/cstdint.hpp>

namespace dbproxy
{

/**
 * DB Proxy configuration
 */
class config
{
  public:
    std::string db_type;
    std::string log_file;
    boost::uint16_t workers_count;
    std::string upstream_ip;
    boost::uint16_t upstream_port;
    std::string local_ip;
    boost::uint16_t local_port;

    /**
     * Parse parameters from argc, argv
     */
    static config from_args(int argc, const char *const *argv);

  private:
    // validate fields
    void validate() throw();
};

}

#endif
