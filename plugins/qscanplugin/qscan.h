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
#include <QtDebug>

#include "qscan_global.h"
#include "scaninterface.h"

class SCANSHARED_EXPORT QScan : public QObject
{
  Q_OBJECT
public:
  explicit QScan(QObject* parent = nullptr);
  //  ~QScan() = default;

  bool init();
  QStringList devices();
  ScanDevice* device(const QString& device_name);
  bool openDevice(const QString& device_name);
  bool startScanning(const QString& device_name);
  void cancelScan();
  bool isScanning();
  //  ScanOptions options(const QString& device_name);

  int topLeftX(ScanDevice* device);
  void setTopLeftX(ScanDevice* device, int value);
  int topLeftY(ScanDevice* device);
  void setTopLeftY(ScanDevice* device, int value);
  int bottomRightX(ScanDevice* device);
  void setBottomRightX(ScanDevice* device, int value);
  int bottomRightY(ScanDevice* device);
  void setBottomRightY(ScanDevice* device, int value);
  int contrast(ScanDevice* device);
  void setContrast(ScanDevice* device, int value);
  int brightness(ScanDevice* device);
  void setBrightness(ScanDevice* device, int value);
  int resolution(ScanDevice* device);
  void setResolution(ScanDevice* device, int value);
  int resolutionX(ScanDevice* device);
  void setResolutionX(ScanDevice* device, int value);
  int resolutionY(ScanDevice* device);
  void setResolutionY(ScanDevice* device, int value);
  void setPreview(ScanDevice* device);
  void clearPreview(ScanDevice* device);
  void setScanMode(ScanDevice* device, const QString& mode);
  void setSource(ScanDevice* device, const QString& source);

signals:
  void scanCompleted(const QImage&, const int resolution);
  void scanFailed();
  void scanOpenFailed();
  void scanProgress(const int&);
  void optionsSet(ScanDevice*);
  void sourceChanged(ScanDevice*);
  void modeChanged(ScanDevice*);

protected:
#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  ScanInterface* m_scan_lib;

#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
#include "win/scantwain.h"
  // TODO handle special twain shit
#endif
};

#endif // SCAN_H
