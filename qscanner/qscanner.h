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
#include "scanoptions.h"

namespace QScanner {

#define MANUFACTURER "Simon Meaden"
#define PRODUCT "QScanner Library"
#define PRODUCT_NAME "QScanner"

class QSCANSHARED_EXPORT QScanner : public QObject
{
  Q_OBJECT
public:

  explicit QScanner(QObject* parent = nullptr);
  explicit QScanner(const QString& manufacturer,
                    const QString& product,
                    const QString& product_name,
                    QObject* parent = nullptr);
  //  ~QScan() = default;

  bool init();
  void retestDevices();
  /*!
     \brief Get the list of avilable device name strings.
  */
  QStringList devices();

  bool isDeviceAvaliable();

  /*!
     \brief Opens the device named by device_name.

     Opens the device returning true and setting the current device,
     if the open was successful.
  */
  //  bool openDevice(const QString& device_name);
  bool startScanning(const QString& device_name);
  void cancelScan();
  bool isScanning();

  /*!
     \brief Check whether a particular AvailableOption is available on the current ScanDevice;
  */
  bool isAvailable(ScanOptions::AvailableOptions option);

  /*!
     \brief Get the value of a particular AvailableOption on the current device.

     Returns the current value as a QVariant. If the option is not available
     the an invalid QVariant is returned.
  */
  QVariant value(ScanOptions::AvailableOptions option);

  //  /*!
  //   * \brief
  //   */
  //  static QString optionName(AvailableOptions option);
  //  ScanOptions options(const QString& device_name);
  /*!
     \brief Get the scanner device vendor's name.
  */
  QString vendor();
  /*!
     \brief Get the scanner device model name.
  */
  QString model();
  /*!
     \brief Get the scanner device type.
  */
  QString type();

  QString name();
  QString displayName();
  ScanOptions* options();
  bool setDevice(const QString& name);

  int topLeftX();
  void setTopLeftX(int value);
  int topLeftY();
  void setTopLeftY(int value);
  int bottomRightX();
  void setBottomRightX(int value);
  int bottomRightY();
  void setBottomRightY(int value);
  int contrast();
  void setContrast(int value);
  int brightness();
  void setBrightness(int value);
  int resolution();
  void setResolution(int value);
  int resolutionX();
  void setResolutionX(int value);
  int resolutionY();
  void setResolutionY(int value);
  void setPreview();
  void clearPreview();
  void setScanMode(const QString& mode);
  void setSource(const QString& source);

  QString manufacturer() const;
  void setManufacturer(const QString& manufacturer);
  QString product() const;
  void setProduct(const QString& product);
  QString product_name() const;
  void setProduct_name(const QString& product_name);

signals:
  void scanCompleted(const QImage&, const int resolution);
  void scanFailed();
  void scanOpenFailed();
  void scanProgress(const int&);
  void optionsSet();
  //  void sourceChanged();
  //  void modeChanged();
  void optionChanged(ScanOptions::AvailableOptions option, QVariant value);


private:
  IScanLibrary* m_scan_lib;

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)

#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  //#include "win/scantwain.h"
  // TODO handle special twain shit
#endif

  void initLib();

  QString m_manufacturer;
  QString m_product;
  QString m_product_name;

  static const QString m_version;
  static const int m_major_version;
  static const int m_minor_version;
  static const int m_build_version;

};

} // end of namespace QScanner

#endif // SCAN_H
