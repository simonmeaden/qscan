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
#ifndef SCANTWAIN_H
#define SCANTWAIN_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QVector>

#include "twpp.hpp"

#include "iscanlibrary.h"

namespace QScanner {

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)

class TwainLibrary : public ScanLibrary
{
public:
  TwainLibrary(QObject* parent = nullptr);
  TwainLibrary(QString manufacturer,
               QString product,
               QString productName,
               QObject* parent = nullptr);

  // IScanLibrary interface
  bool init() override;
  void exit() override;

  void detectDevices() override;
  ScanDevice* device(QString name) override;
  QStringList devices() override;

  bool detectAvailableOptions(QString device_name) override;

  ScanDevice* currentDevice() override;
  void setCurrentDevice(ScanDevice* current_device) override;
  bool startScan(QString device_name) override;
  void cancelScan() override;
  //  void getAvailableScannerOptions(ScanDevice* device);
  QRect geometry(QString device_name) override;
  void setTopLeftX(ScanDevice* device, int x) override;
  void setTopLeftY(ScanDevice* device, int x) override;
  void setBottomRightX(ScanDevice* device, int value) override;
  void setBottomRightY(ScanDevice* device, int x) override;
  void setContrast(ScanDevice* device, int value) override;
  void setBrightness(ScanDevice* device, int value) override;
  void setResolution(ScanDevice* device, int value) override;
  void setResolutionX(ScanDevice* device, int value) override;
  void setResolutionY(ScanDevice* device, int value) override;
  void setPreview(ScanDevice* device) override;
  void clearPreview(ScanDevice* device) override;
  void setMode(ScanDevice* device, const QString& mode) override;
  void setSource(ScanDevice* device, const QString& source) override;
  bool isScanning() const override;

  void setManufacturer(const QString& manufacturer) override;
  void setProduct(const QString& product) override;
  void setProductName(const QString& productName) override;

private:
  Twpp::Manager m_twain_manager;
  QList<Twpp::Source> m_sources;
  char m_version_char[32];
  char m_manufacturer_char[32];
  char m_product_char[32];
  char m_product_name_char[32];

  void initLib();
};

#endif // MS Check

} // end of namespace QScanner

#endif // SCANTWAIN_H
