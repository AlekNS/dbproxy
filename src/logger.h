#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <map>
#include <memory>

#include "instantiator.h"

namespace dbproxy
{

class logger;

/**
 * Logger creator interface.
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
    virtual ~logger() = default;
};

}

#endif
