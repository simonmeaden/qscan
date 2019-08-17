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
#ifndef SCAN_H
#define SCAN_H

#include <QObject>
#include <QtDebug>

#include "qscan_global.h"
#include "iscanlibrary.h"

namespace QScanner {

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
  IScanLibrary* m_scan_lib;

#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
#include "win/scantwain.h"
  // TODO handle special twain shit
#endif
};

} // end of namespace QScanner

#endif // SCAN_H
