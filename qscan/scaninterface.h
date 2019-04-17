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
#ifndef SCANLIB_H
#define SCANLIB_H

#include <QImage>
#include <QSharedPointer>
#include <QStringList>

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  #include <sane/sane.h>
#endif

#include "scanoptions.h"

typedef QSharedPointer<QImage> Image;

class ScanDevice : public QObject
{
public:
  ScanDevice(QObject* parent = nullptr) : QObject(parent) {}
  ~ScanDevice() {}

  QString name;
  QString vendor;
  QString model;
  QString type;
#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  SANE_Handle sane_handle = nullptr;
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  // TODO
#endif
};
//typedef QSharedPointer<ScanDevice> Device;
typedef QMap<QString, ScanDevice*> DeviceMap;

class ScanOptions;

class ScanInterface
{
public:
  typedef enum
  {
    SCAN_STATUS_GOOD = 0,     /* everything A-OK */
    SCAN_STATUS_UNSUPPORTED,  /* operation is not supported */
    SCAN_STATUS_CANCELLED,    /* operation was cancelled */
    SCAN_STATUS_DEVICE_BUSY,  /* device is busy; try again later */
    SCAN_STATUS_INVAL,        /* data is invalid (includes no dev at open) */
    SCAN_STATUS_EOF,          /* no more data available (end-of-file) */
    SCAN_STATUS_JAMMED,       /* document feeder jammed */
    SCAN_STATUS_NO_DOCS,      /* document feeder out of documents */
    SCAN_STATUS_COVER_OPEN,   /* scanner cover is open */
    SCAN_STATUS_IO_ERROR,     /* error during device I/O */
    SCAN_STATUS_NO_MEM,       /* out of memory */
    SCAN_STATUS_ACCESS_DENIED /* access to resource has been denied */
  } SCAN_Status;

  virtual ~ScanInterface() {}

  virtual bool init() = 0;
  virtual QStringList devices() = 0;
  virtual ScanDevice* device(QString device_name) = 0;
  virtual bool openDevice(QString device_name) = 0;
  virtual bool startScan(QString device_name) = 0;
  virtual ScanOptions options(QString device_name) = 0;
  virtual void cancelScan(QString device_name) = 0;
  virtual void getAvailableScannerOptions(QString device_name) = 0;
  virtual QRect geometry(QString device_name) = 0;

  virtual bool topLeftX(ScanDevice* device, int& value) = 0;
  virtual bool setTopLeftX(ScanDevice* device, int x) = 0;
  virtual bool topLeftY(ScanDevice* device, int& value) = 0;
  virtual bool setTopLeftY(ScanDevice* device, int x) = 0;
  virtual bool bottomRightX(ScanDevice* device, int& value) = 0;
  virtual bool setBottomRightX(ScanDevice* device, int value) = 0;
  virtual bool bottomRightY(ScanDevice* device, int& value) = 0;
  virtual bool setBottomRightY(ScanDevice* device, int x) = 0;
  virtual bool contrast(ScanDevice* device, int& value) = 0;
  virtual bool setContrast(ScanDevice* device, int value) = 0;
  virtual bool brightness(ScanDevice* device, int& value) = 0;
  virtual bool setBrightness(ScanDevice* device, int value) = 0;
  virtual bool resolution(ScanDevice* device, int& value) = 0;
  virtual bool setResolution(ScanDevice* device, int value) = 0;
  virtual bool resolutionX(ScanDevice* device, int& value) = 0;
  virtual bool setResolutionX(ScanDevice* device, int value) = 0;
  virtual bool resolutionY(ScanDevice* device, int& value) = 0;
  virtual bool setResolutionY(ScanDevice* device, int value) = 0;
  virtual bool setPreview(ScanDevice* device) = 0;
  virtual bool clearPreview(ScanDevice* device) = 0;


protected:
};

class ScanLibrary
  : public QObject
  , public ScanInterface
{
  Q_OBJECT
public:
  ScanLibrary(QObject* parent = nullptr);
  ~ScanLibrary();

signals:
  void scanCompleted(const QImage& image);
  void scanFailed();
  void scanProgress(const int&);
};

#endif // SCANLIB_H
