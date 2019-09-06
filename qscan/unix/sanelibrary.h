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
#ifndef SCANSANE_H
#define SCANSANE_H

#include <QObject>
#include <QImage>
#include <QtDebug>


#include "iscanlibrary.h"
#include "saneworker.h"
#include "scanoptions.h"
#include "scandevice.h"
#include "version.h"

#ifndef PATH_MAX
  #define PATH_MAX 1024
#endif

namespace QScanner {

class SaneLibrary final : public ScanLibrary
{
  Q_OBJECT
public:
  explicit SaneLibrary(QObject* parent = nullptr);
  ~SaneLibrary() override;

  bool init() override;
  void exit() override;

  // ScanInterface interface
  void detectDevices() override;
  QStringList devices() override;
  ScanDevice* device(QString device_name) override;
  //  ScanOptions options(QString device_name) override;
  bool detectAvailableOptions(QString device_name) override;

  ScanDevice* getCurrentDevice() override;
  void setCurrentDevice(ScanDevice* current_device) override;

  bool startScan(QString device_name) override;
  bool isScanning() const override;
  void cancelScan() override;
  QRect geometry(QString device_name) override;
  const Version& version() const;

  void setOption(ScanOptions::AvailableOptions option, QVariant value);
  //  void topLeftX(ScanDevice* device, int& value) override;
  void setTopLeftX(ScanDevice* device, int x) override;
  //  void topLeftY(ScanDevice* device, int& value) override;
  void setTopLeftY(ScanDevice* device, int x) override;
  //  void bottomRightX(ScanDevice* device, int& value) override;
  void setBottomRightX(ScanDevice* device, int value) override;
  //  void bottomRightY(ScanDevice* device, int& value) override;
  void setBottomRightY(ScanDevice* device, int x) override;
  //  void contrast(ScanDevice* device, int& value) override;
  void setContrast(ScanDevice* device, int value) override;
  //  void brightness(ScanDevice* device, int& value) override;
  void setBrightness(ScanDevice* device, int value) override;
  //  void resolution(ScanDevice* device, int& value) override;
  void setResolution(ScanDevice* device, int value) override;
  //  void resolutionX(ScanDevice* device, int& value) override;
  void setResolutionX(ScanDevice* device, int value) override;
  //  void resolutionY(ScanDevice* device, int& value) override;
  void setResolutionY(ScanDevice* device, int value) override;
  void setPreview(ScanDevice* device) override;
  void clearPreview(ScanDevice* device) override;
  void setMode(ScanDevice* device, const QString& value) override;
  void setSource(ScanDevice* device, const QString& value) override;

  /*!
     \brief Gets the available options for the device named by device_name.
  */
  void getAvailableScannerOptions(ScanDevice* device) override;

signals:
  void finished();
  void startScanning(ScanDevice*);
  void getAvailableOptions(ScanDevice*);
  void setBoolValue(ScanDevice*, int, const QString&, bool);
  void setIntValue(ScanDevice*, int, const QString&, int);
  void setStringValue(ScanDevice*, const QString&, const QString&);
  void getIntValue(ScanDevice*, int, int);
  void cancelScanning();
  //  void optionChanged(ScanOptions::AvailableOptions option, QVariant value);


protected:
  DeviceMap m_scanners;
  ScanDevice* m_current_device;
  QImage* m_image{};
  Version m_sane_version;
  QStringList m_device_list;
  bool m_scanning;
  SaneWorker* m_worker;

  //  QMutex m_mutex;

  //  void receiveIntValue(ScanDevice* device, int value);
  void scanIsCompleted();
  void receiveOptionChange(const QString descriptor, const QVariant& value);

public:
};

} // end of namespace QScanner

#endif // SCANSANE_H
