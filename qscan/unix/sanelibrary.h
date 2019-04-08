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
#ifndef SCANSANE_H
#define SCANSANE_H

#include <QObject>

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)

#include <QImage>
#include <QMutexLocker>
#include <QThread>

#include <sane/sane.h>
#include <sane/saneopts.h>

#include <log4qt/consoleappender.h>
#include <log4qt/logger.h>
#include <log4qt/logmanager.h>
#include <log4qt/ttcclayout.h>

#include "scaninterface.h"
#include "scanoptions.h"
#include "version.h"

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

class SaneLibrary final : public ScanLibrary
{
  Q_OBJECT
public:
  //  using callback_type = void(const std::string& resource,
  //                             std::string& username,
  //                             std::string& password);

  explicit SaneLibrary(QObject* parent = nullptr);
  ~SaneLibrary() override;

  bool init() override;

  QStringList getDevices() override;
  Device getDevice(QString name) override;
  bool openDevice(QString device) override;
  bool startScan(QString name) override;
  void cancelScan(QString name) override;
  void exit();
  ScanOptions* options(QString name) override;
  void setOptions(QString name, ScanOptions* options) override;

  const Version& version() const;

protected:
  SANE_Status doScan(const char* fileName);
  Log4Qt::Logger* m_logger;
  DeviceMap m_scanners;
  OptionsMap m_options;
  QImage* m_image;
  Version m_version;

  static QMutex _mutex;

  void scan(Device device);

  static void callbackWrapper(SANE_String_Const resource,
                              SANE_Char* name,
                              SANE_Char* password);
};

#endif

#endif // SCANSANE_H
