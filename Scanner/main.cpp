#include "mainwindow.h"
#include <QApplication>
#include <QLoggingCategory>

#include <log4qt/consoleappender.h>
#include <log4qt/logger.h>
#include <log4qt/logmanager.h>
#include <log4qt/ttcclayout.h>

using namespace Log4Qt;
int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  LogManager::rootLogger();
  TTCCLayout* p_layout = new TTCCLayout();
  p_layout->setName(QStringLiteral("Logger"));
  p_layout->activateOptions();
  // Create an appender
  ConsoleAppender* p_appender =
    new ConsoleAppender(p_layout, ConsoleAppender::STDOUT_TARGET);
  p_appender->setName(QStringLiteral("Console"));
  p_appender->activateOptions();
  // Set appender on root logger
  Logger::rootLogger()->addAppender(p_appender);
  Logger::rootLogger()->setLevel(Level::DEBUG_INT);

  //  auto* object = new LoggerObject(&a);
  QLoggingCategory::setFilterRules("*.debug=false\n"
                                   "virus.debug=true");
  MainWindow w;
  w.show();

  return a.exec();
}
