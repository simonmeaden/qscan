#ifndef LOGGER_H
#define LOGGER_H

#include "logger_global.h"

// used by threaded classes to send messages to main class logger.
enum LogLevel
{
  TRACE,
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL,
  OFF,
};

#if defined(LOGGER_ENABLE)
#include "log4qt/consoleappender.h"
#include "log4qt/log4qt.h"
#include "log4qt/logger.h"
#include "log4qt/logmanager.h"
#include "log4qt/ttcclayout.h"

#else

namespace Log4Qt {

class LOGGERSHARED_EXPORT Logger
{

public:
  Logger() {}

  static Logger logger(const QString&) { return new Logger(); }

  void info(const QString&) {}
  void warn(const QString&) {}
  void fatal(const QString&) {}
  void error(const QString&) {}
  void debug(const QString&) {}
  void trace(const QString&) {}
  void warn(const QString&) {}
};

} // end of namespace
#endif // blank logger
#endif // LOGGER_H
