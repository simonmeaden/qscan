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

  QString name;
  QString vendor;
  QString model;
  QString type;
  QString op_name;
  ScanOptions* options;
#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  SANE_Handle sane_handle = nullptr;
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
