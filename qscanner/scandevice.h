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
#ifndef SCANDEVICE_H
#define SCANDEVICE_H

#include <QObject>
#include <QMap>

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  #include <sane/sane.h>
#endif

namespace QScanner {

class ScanOptions;

class ScanDevice : public QObject
{
  Q_OBJECT

public: ScanDevice(QObject* parent = nullptr);
  ScanDevice(const ScanDevice& other);

  QString displayName() const;
  void setDisplayName(const QString& displayName);
  QString descriptor();
  void setDescriptor(QString descriptor);
  QString vendor() const;
  void setVendor(const QString& vendor);
  QString model() const;
  void setModel(const QString& model);
  QString type() const;
  void setType(const QString& type);
  ScanOptions* options() const;
  void setOptions(ScanOptions* options);

  //  QString op_name;
#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  //  SANE_Handle handle();
  //  void setHandle(const SANE_Handle& handle);
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  // TODO
#endif

  bool areAvailableOptionsSet() const;
  void setAvailableOptionsSet(bool areAvailableOptionsSet);

protected:
  QString m_display_name;
  QString m_descriptor;
  QString m_vendor;
  QString m_model;
  QString m_type;
  ScanOptions* m_options;
  bool m_available_options_set = false;

  //  QString op_name;
#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  //  SANE_Handle m_handle = nullptr;
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  // TODO
#endif

};
// typedef QSharedPointer<ScanDevice> Device;
//typedef QMap<QString, ScanDevice*> DeviceMap;
using DeviceMap = QMap<QString, ScanDevice*>;

} // end of namespace QScanner

Q_DECLARE_METATYPE(QScanner::ScanDevice)

#endif // SCANDEVICE_H
