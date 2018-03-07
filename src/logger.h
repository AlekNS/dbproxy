#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <map>
#include <thread>
#include <mutex>
#include <fstream>
#include <list>

#include "instantiator.h"

namespace dbproxy
{

class logger;

/**
 * logger creator interface.
 */
class logger_instantiator : public instantiator<logger> { };

/**
 * Registry for loggers.
 * Not thread safe.
 */
class logger_registry : public registry<logger_instantiator> { };

/**
 * Logger interface.
 */
class logger
{
  public:
    virtual void out(const std::string &str) = 0;

    virtual ~logger() = default;
};

class logger_file;

/**
 * Concrete logger_file instantiator.
 */
class logger_instantiator_simple_out_file : public logger_instantiator
{
public:
    logger_instantiator_simple_out_file(const std::string &logfile);

    virtual std::shared_ptr<logger> create() override;

private:
    std::string outfile;
    static std::shared_ptr<logger> instance;
    static std::mutex mtx;
};

/**
 * Async queue based file logger.
 */
class logger_file: public logger
{
public:
    logger_file(const std::string &file);

    virtual void out(const std::string &str) override;

    void start();
    void stop();

private:
    void write_worker();

    enum { max_buffer_size = 16384 };
    bool done;
    std::string outfile;
    std::thread worker_thread;
    std::fstream outstream;
    std::mutex mtx;
    std::list<std::string> buffer;
};

}

#endif
