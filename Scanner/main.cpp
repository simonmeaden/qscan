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
#include <QPlainTextEdit>
#include <QTextStream>

#if defined(LOGGER_ENABLE)
  //  #include "texteditiodevice.h"
  #include <log4qt/consoleappender.h>
  #include <log4qt/logger.h>
  #include <log4qt/logmanager.h>
  #include <log4qt/ttcclayout.h>
  //  #include <log4qt/writerappender.h>
#endif

#include "scanoptions.h"
//#include "scanpage.h"
#include "documentdata.h"
#include "logger.h"

using Image = QSharedPointer<QImage>;
Q_DECLARE_METATYPE(Image)

using namespace Log4Qt;
int main(int argc, char* argv[])
{
  //
  QApplication a(argc, argv);
  qRegisterMetaType<Image>();
  qRegisterMetaType<ScanOptions>();
  qRegisterMetaType<DocData>();
  qRegisterMetaType<DocumentData>();
  qRegisterMetaType<LogLevel>();

#if defined(LOGGER_ENABLE)
  LogManager::rootLogger();
  auto* p_layout = new TTCCLayout();
  p_layout->setName(QStringLiteral("Logger"));
  p_layout->activateOptions();
  // Create an appender
  ConsoleAppender* p_appender =
    new ConsoleAppender(p_layout, ConsoleAppender::STDOUT_TARGET);
  p_appender->setName(QStringLiteral("Console"));
  p_appender->activateOptions();
  //
  //  auto* text_edit = new QPlainTextEdit();
  //  auto* log_io_device = new TextEditIoDevice(text_edit);
  //  auto* stream = new QTextStream(log_io_device);
  //  WriterAppender* p_writer = new WriterAppender(p_layout, stream);
  //  p_writer->setName("StreamWriter");
  //  p_writer->activateOptions();
  // Set appender on root logger
  Logger::rootLogger()->addAppender(p_appender);
  //  Logger::rootLogger()->addAppender(p_writer);
  Logger::rootLogger()->setLevel(Level::DEBUG_INT);
  //  auto* object = new LoggerObject(&a);
  QLoggingCategory::setFilterRules("*.debug=false\n"
                                   "virus.debug=true");

#endif

  MainWindow w;

  //#if defined(LOGGER_ENABLE)
  //  w.setLogTextEdit(text_edit);
  //#endif

  w.show();
  int res = QApplication::exec();

  //#if defined(LOGGER_ENABLE)
  //  Logger::rootLogger()->removeAppender(p_writer);
  //  delete log_io_device;
  //  delete stream;
  //  delete text_edit;
  //#endif
  return res;
}
