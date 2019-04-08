/*
    Copyright © Simon Meaden 2019.
    This file is part of the QScan cpp library.

    QScan is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QScan is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QScan.  If not, see <http://www.gnu.org/licenses/>.
    It is also available on request from Simon Meaden simonmeaden@sky.com.
*/
#include "mainwindow.h"
#include <QApplication>
#include <QImage>
#include <QLoggingCategory>

#include <log4qt/consoleappender.h>
#include <log4qt/logger.h>
#include <log4qt/logmanager.h>
#include <log4qt/ttcclayout.h>

typedef QSharedPointer<QImage> Image;
Q_DECLARE_METATYPE(Image)

using namespace Log4Qt;
int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  qRegisterMetaType<Image>();

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
