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
#ifndef SANESCANETHREAD_H
#define SANESCANETHREAD_H

#include <QImage>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>

#include <log4qt/logger.h>

#include "scaninterface.h"

class SaneWorker : public QObject
{
  Q_OBJECT
public:
  explicit SaneWorker(QObject* parent = nullptr);

  void scan(ScanDevice* device);
  void loadAvailableScannerOptions(ScanDevice* device);
  void setBoolValue(ScanDevice* device,
                    int option_id,
                    const QString&,
                    bool value);
  void setIntValue(ScanDevice* device,
                   int option_id,
                   const QString& name,
                   int value);
  void setStringValue(ScanDevice* device,
                      int option_id,
                      const QString& name,
                      const QString& value);
  void cancelScan();

signals:
  void scanCompleted(const QImage&);
  void scanFailed();
  void scanProgress(double);
  void finished();
  //  void availableScannerOptions(ScanDevice*);
  //  void sendIntValue(ScanDevice*, int);
  void optionsSet();

protected:
  Log4Qt::Logger* m_logger;
  QMutex m_mutex;
  SANE_Handle m_handle;

  void getIntValue(ScanDevice* device, int option_id, const QString& name);
  void getStringValue(ScanDevice* device,
                      int option_id,
                      const QString& name,
                      const SANE_Option_Descriptor* opt);
  void setResolution(ScanDevice* device,
                     const SANE_Option_Descriptor* current_option,
                     SANE_Int option_id);
};

enum message_level
{
  MSG,   /* info message */
  INF,   /* non-urgent warning */
  WRN,   /* warning */
  ERR,   /* error, test can continue */
  FATAL, /* error, test can't/mustn't continue */
  BUG    /* bug in tstbackend */
};

static int
check(enum message_level level, int condition, const char* format, ...);
static void*
guards_malloc(size_t size);
static void
guards_free(void* ptr);
static const SANE_Option_Descriptor*
get_optdesc_by_name(SANE_Handle device, const char* name, int* option_num);
static char*
get_option_value(SANE_Handle device, const char* option_name);

#endif // SANESCANETHREAD_H
