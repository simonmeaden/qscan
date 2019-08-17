/*
  Copyright © Simon Meaden 2019.
  This file was developed as part of the Biblios application but could
  easily be used elsewhere.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#ifndef SCANLIB_H
#define SCANLIB_H

#include <QObject>
#include <QImage>
#include <QSharedPointer>
#include <QStringList>

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  #include <sane/sane.h>
#endif

//#include "iplugininterface.h"
namespace QScanner {

using Image = QSharedPointer<QImage>;

class ScanOptions;
class ScanDevice;

// Interface for the scan libraries
class IScanLibrary
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

  virtual ~IScanLibrary() = default;

  virtual bool init() = 0;
  virtual void exit() = 0;
  virtual QStringList devices() = 0;
  virtual ScanDevice* device(QString device_name) = 0;
  virtual bool detectAvailableOptions(QString device_name) = 0;
  virtual bool startScan(QString device_name) = 0;

  virtual void cancelScan(/*QString device_name*/) = 0;
  virtual void getAvailableScannerOptions(QString device_name) = 0;
  virtual QRect geometry(QString device_name) = 0;

  //  virtual void topLeftX(ScanDevice* device, int& value) = 0;
  virtual void setTopLeftX(ScanDevice* device, int x) = 0;
  //  virtual void topLeftY(ScanDevice* device, int& value) = 0;
  virtual void setTopLeftY(ScanDevice* device, int x) = 0;
  //  virtual void bottomRightX(ScanDevice* device, int& value) = 0;
  virtual void setBottomRightX(ScanDevice* device, int value) = 0;
  //  virtual void bottomRightY(ScanDevice* device, int& value) = 0;
  virtual void setBottomRightY(ScanDevice* device, int x) = 0;
  //  virtual void contrast(ScanDevice* device, int& value) = 0;
  virtual void setContrast(ScanDevice* device, int value) = 0;
  //  virtual void brightness(ScanDevice* device, int& value) = 0;
  virtual void setBrightness(ScanDevice* device, int value) = 0;
  //  virtual void resolution(ScanDevice* device, int& value) = 0;
  virtual void setResolution(ScanDevice* device, int value) = 0;
  //  virtual void resolutionX(ScanDevice* device, int& value) = 0;
  virtual void setResolutionX(ScanDevice* device, int value) = 0;
  //  virtual void resolutionY(ScanDevice* device, int& value) = 0;
  virtual void setResolutionY(ScanDevice* device, int value) = 0;
  virtual void setPreview(ScanDevice* device) = 0;
  virtual void clearPreview(ScanDevice* device) = 0;
  virtual void setMode(ScanDevice* device, const QString& mode) = 0;
  virtual void setSource(ScanDevice* device, const QString& source) = 0;
  virtual bool isScanning() const = 0;

protected:
};

/*!
  \class ScanLibrary

  A base class defining some required signals.
*/
class ScanLibrary
  : public QObject
  , public IScanLibrary
{
  Q_OBJECT
public: ScanLibrary(QObject* parent = nullptr);  //  ~ScanLibrary();

signals:
  void scanCompleted(const QImage& image, const int resolution);
  void scanFailed();
  void scanOpenFailed(const QString&);
  void scanProgress(const int&);
  void optionsSet(ScanDevice*);
  void sourceChanged(ScanDevice*);
  void modeChanged(ScanDevice*);
};

} // end of namespace QScanner


#endif // SCANLIB_H
