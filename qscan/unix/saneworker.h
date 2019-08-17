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
#ifndef SANESCANETHREAD_H
#define SANESCANETHREAD_H

#include <QImage>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QtDebug>

#include "iscanlibrary.h"
#include "scanoptions.h"

namespace QScanner {

class SaneWorker : public QObject
{
  Q_OBJECT
public:
  explicit SaneWorker(QObject* parent = nullptr);

  void scan(ScanDevice* device);
  void loadAvailableScannerOptions(ScanDevice* device);
  void setBoolValue(ScanDevice* device, int option_id, const QString&, bool value);
  void setIntValue(ScanDevice* device, int option_id, const QString& name, int value);
  void setStringValue(ScanDevice* device, const QString& name, const QString& value);
  void cancelScan();

signals:
  void scanCompleted(const QImage&, const int resolution);
  void scanFailed();
  void scanOpenFailed(const QString&);
  void scanProgress(double);
  void finished();
  //  void availableScannerOptions(ScanDevice*);
  //  void sendIntValue(ScanDevice*, int);
  void optionsSet(ScanDevice*);
  void sourceChanged(ScanDevice*);
  void modeChanged(ScanDevice*);

protected:
  //  Log4Qt::Logger* m_logger;
  QMutex m_mutex;
  SANE_Handle m_sane_handle{};

  void getIntValue(ScanDevice* device, int option_id, const QString& name);
  void getListValue(ScanDevice* device, int option_id, const QString& name, const SANE_Option_Descriptor* opt);
  void setResolution(ScanDevice* device, const SANE_Option_Descriptor* current_option, SANE_Int option_id);
  void setSource(ScanDevice* device, const SANE_Option_Descriptor* option, SANE_Int);
  void setMode(ScanDevice* device, const SANE_Option_Descriptor* option, SANE_Int);

  static const int GUARDS_SIZE = 4; /* 4 bytes */
  void* guardedMalloc(size_t size);
  void guardedFree(void* ptr);
  QVariant getOptionValue(ScanDevice* device, const QString& option_name);
};

} // end of namespace QScanner

#endif  // SANESCANETHREAD_H
