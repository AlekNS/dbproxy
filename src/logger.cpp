#include "logger.h"

using namespace dbproxy;

template<> std::map<const std::string, std::shared_ptr<logger_instantiator>> logger_registry::registry::instantiators{};
