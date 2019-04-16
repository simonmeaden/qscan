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
#include <QObject>
#include <QMutex>
#include <QMutexLocker>

#include <log4qt/logger.h>

#include "scaninterface.h"

class SaneWorker : public QObject
{
  Q_OBJECT
public: SaneWorker(ScanDevice* device, QObject* parent = nullptr);
  ~SaneWorker() {}

  void scan();

  void getAvailableScannerOptions(ScanDevice* device, ScanOptions options);

signals:
  void scanCompleted(const QImage&);
  void scanFailed();
  void scanProgress(double);
  void finished();
  void availableScannerOptions(QString name, ScanOptions options);

protected:
  QImage m_image;
  ScanDevice* m_device;
  Log4Qt::Logger* m_logger;
  QMutex m_mutex;
};

#endif // SANESCANETHREAD_H
