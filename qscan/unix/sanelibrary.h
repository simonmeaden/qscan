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
#ifndef SCANSANE_H
#define SCANSANE_H

#include <QObject>

#  include <QImage>
#  include <QMutexLocker>
#  include <QThread>

#  include <sane/sane.h>
#  include <sane/saneopts.h>

#  include <log4qt/consoleappender.h>
#  include <log4qt/logger.h>
#  include <log4qt/logmanager.h>
#  include <log4qt/ttcclayout.h>

#  include "scaninterface.h"
#  include "scanoptions.h"
#  include "version.h"

#ifndef PATH_MAX
  #define PATH_MAX 1024
#endif

class SaneLibrary final : public ScanLibrary
{
  Q_OBJECT
public:
  explicit SaneLibrary(QObject* parent = nullptr);
  ~SaneLibrary() override;

  bool init() override;

  // ScanInterface interface
  QStringList devices() override;
  ScanDevice* device(QString device_name) override;
  ScanOptions options(QString device_name) override;
  bool openDevice(QString device_name) override;
  bool startScan(QString device_name) override;
  void cancelScan(QString device_name) override;
  void exit();
  QRect geometry(QString device_name) override;
  const Version& version() const;

  bool topLeftX(ScanDevice* device, int& value) override;
  bool setTopLeftX(ScanDevice* device, int x) override;
  bool topLeftY(ScanDevice* device, int& value) override;
  bool setTopLeftY(ScanDevice* device, int x) override;
  bool bottomRightX(ScanDevice* device, int& value) override;
  bool setBottomRightX(ScanDevice* device, int value) override;
  bool bottomRightY(ScanDevice* device, int& value) override;
  bool setBottomRightY(ScanDevice* device, int x) override;
  bool contrast(ScanDevice* device, int& value) override;
  bool setContrast(ScanDevice* device, int value) override;
  bool brightness(ScanDevice* device, int& value) override;
  bool setBrightness(ScanDevice* device, int value) override;
  bool resolution(ScanDevice* device, int& value) override;
  bool setResolution(ScanDevice* device, int value) override;
  bool resolutionX(ScanDevice* device, int& value) override;
  bool setResolutionX(ScanDevice* device, int value) override;
  bool resolutionY(ScanDevice* device, int& value) override;
  bool setResolutionY(ScanDevice* device, int value) override;
  bool setPreview(ScanDevice* device) override;
  bool clearPreview(ScanDevice* device) override;

signals:
  void finished();
  void startScanning();
  void getAvailableOptions(ScanDevice*, ScanOptions);

protected:
  SANE_Status doScan(const char* fileName);
  Log4Qt::Logger* m_logger;
  DeviceMap m_scanners;
  OptionsMap m_options;
  QImage* m_image;
  Version m_version;

  //  int m_brightness_opt = -1;
  //  int m_resolution_opt = -1;
  //  int m_resolution_opt_x = -1;
  //  int m_resolution_opt_y = -1;
  //  int m_tl_x_opt = -1;
  //  int m_tl_y_opt = -1;
  //  int m_br_x_opt = -1;
  //  int m_br_y_opt = -1;
  //  int m_contrast_opt = -1;
  //  //  int m_resolution_x_opt = -1;
  //  //  int m_resolution_y_opt = -1;
  //  int m_source_opt = -1;
  //  int m_mode_opt = -1;

  static QMutex _mutex;

  void getAvailableScannerOptions(QString device_name) override;
  void receiveAvailableScannerOptions(QString device_name, ScanOptions options);

  static void callbackWrapper(SANE_String_Const resource, SANE_Char* name, SANE_Char* password);

  // ScanInterface interface
  bool setBoolValue(ScanDevice* device, QString name, bool value);
  bool setIntValue(ScanDevice* device, QString name, int value);
  bool getIntValue(ScanDevice* device, QString name, int& value);

public:
};

#endif // SCANSANE_H
