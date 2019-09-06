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
#include <QThread>
#include <QQueue>

#include "iscanlibrary.h"
#include "scanoptions.h"

namespace QScanner {

struct Task;

class SaneWorker : public QThread
{
  Q_OBJECT
public:
  enum SaneTask
  {
    SET_BOOL,
    SET_STRING,
    DEVICE_INT,
    DEVICE_STRING,
    SCAN,
    CANCEL_SCAN,
    LOAD_OPTIONS,
  };
  explicit SaneWorker(QObject* parent = nullptr);

  void scan(ScanDevice* device);
  void loadAvailableScannerOptions(ScanDevice* device);
  void setDeviceBoolValue(ScanDevice* device, int option_id, const QString& descriptor, bool value);
  void setDeviceIntValue(ScanDevice* device, int option_id, const QString& descriptor, int value);
  void setDeviceStringValue(ScanDevice* device, const QString& name, const QString& value);
  void cancelScan();

signals:
  void scanCompleted(const QImage&, const int resolution);
  void scanFailed();
  void scanOpenFailed(const QString&);
  void scanProgress(double);
  void finished();
  //  void availableScannerOptions(ScanDevice*);
  //  void sendIntValue(ScanDevice*, int);
  void optionsSet();
  //  void sourceChanged(ScanDevice*);
  //  void modeChanged(ScanDevice*);
  void optionChanged(const QString descriptor, const QVariant& value);

private:
  //  Log4Qt::Logger* m_logger;
  QMutex m_mutex;
  bool m_running;
  SANE_Handle m_sane_handle{};
  QQueue<Task*> m_tasks;

  void run();
  void do_scan(Task* task);
  void do_loadAvailableScannerOptions(Task* task);
  void do_setDeviceBoolValue(Task* task);
  void do_setDeviceIntValue(Task* task);
  void do_setDeviceStringValue(Task* task);

  void storeOptionData(SANE_Handle sane_handle,
                       ScanDevice* device,
                       const int option_id,
                       const SANE_Option_Descriptor* descriptor);
  QVariant handleDescriptorType(SANE_Handle sane_handle, ScanDevice* device,
                                const int option_id,
                                const SANE_Option_Descriptor* descriptor);
  QVariant handleConstraintType(const SANE_Option_Descriptor* descriptor);

  //  void storeIntOption(SANE_Handle sane_handle, ScanDevice* device, int option_id, const QString& name,
  //                      const SANE_Value_Type type);
  //  void storeStringOption(SANE_Handle sane_handle, ScanDevice* device, const QString& descriptor,
  //                         const SANE_Option_Descriptor* option, SANE_Int);
  //  void getListValue(ScanDevice* device, int option_id, const QString& name, const SANE_Option_Descriptor* opt);
  //  void storeResolution(SANE_Handle sane_handle, ScanDevice* device, const SANE_Option_Descriptor* descriptor,
  //                       SANE_Int option_id);
  //  void setStoredSource(SANE_Handle sane_handle, ScanDevice* device, const SANE_Option_Descriptor* option, SANE_Int);
  //  void setStoredMode(SANE_Handle sane_handle, ScanDevice* device, const SANE_Option_Descriptor* option, SANE_Int);
  QVariant getDeviceValue(SANE_Handle sane_handle, ScanDevice* device, const QString& option_name);

  static const int GUARDS_SIZE = 4; /* 4 bytes */
  void* guardedMalloc(size_t size);
  void guardedFree(void* ptr);
};

struct Task
{
  SaneWorker::SaneTask task;
  ScanDevice* device;
  int option_id;
  QString descriptor;
  QVariant value;
};


} // end of namespace QScanner

#endif  // SANESCANETHREAD_H
