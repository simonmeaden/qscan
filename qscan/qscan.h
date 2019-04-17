/*
    Copyright © Simon Meaden 2019.
    This file was developed as part of the QScan cpp library but could
    easily be used elsewhere.

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
#ifndef SCAN_H
#define SCAN_H

#include <QObject>

#include "log4qt/logger.h"

#include "qscan_global.h"
#include "scaninterface.h"

class SCANSHARED_EXPORT QScan : public QObject
{
  Q_OBJECT
public:
  explicit QScan(QObject* parent = nullptr);
  ~QScan() {}

  bool init();
  QStringList devices();
  ScanDevice* device(QString device_name);
  bool openDevice(QString device_name);
  bool startScanning(QString device_name);
  void cancelScan(QString device_name);
  ScanOptions options(QString device_name);

  bool topLeftX(ScanDevice* device, int& value);
  bool setTopLeftX(ScanDevice* device, int value);
  bool topLeftY(ScanDevice* device, int& value);
  bool setTopLeftY(ScanDevice* device, int value);
  bool bottomRightX(ScanDevice* device, int& value);
  bool setBottomRightX(ScanDevice* device, int value);
  bool bottomRightY(ScanDevice* device, int& value);
  bool setBottomRightY(ScanDevice* device, int value);
  bool contrast(ScanDevice* device, int& value);
  bool setContrast(ScanDevice* device, int value);
  bool brightness(ScanDevice* device, int& value);
  bool setBrightness(ScanDevice* device, int value);
  bool resolution(ScanDevice* device, int& value);
  bool setResolution(ScanDevice* device, int value);
  bool resolutionX(ScanDevice* device, int& value);
  bool setResolutionX(ScanDevice* device, int value);
  bool resolutionY(ScanDevice* device, int& value);
  bool setResolutionY(ScanDevice* device, int value);
  bool setPreview(ScanDevice* device);
  bool clearPreview(ScanDevice* device);

signals:
  void scanCompleted(const QImage&);
  void scanFailed();
  void scanProgress(const int&);

protected:
  Log4Qt::Logger* m_logger;
#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  ScanInterface* m_scan_lib;

#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
#  include "win/scantwain.h"
  // TODO handle special twain shit
#endif

};

#endif // SCAN_H
