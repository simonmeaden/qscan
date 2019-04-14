/*
    Copyright © Simon Meaden 2019.
    This file is part of the QScan cpp library.

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
#include "qscan.h"

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
#  include "unix/sanelibrary.h"
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
#  include "unix/scantwain.h"
#endif

QScan::QScan(QObject* parent)
  : QObject(parent)
{
  m_logger = Log4Qt::Logger::logger(QStringLiteral("Scan"));

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  m_lib = new SaneLibrary(this);

#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  m_lib = new ScanTwain(this);

#endif
  connect(
    static_cast<ScanLibrary*>(m_lib), &ScanLibrary::scanCompleted, this, &QScan::scanCompleted);
  connect(static_cast<ScanLibrary*>(m_lib), &ScanLibrary::scanFailed, this, &QScan::scanFailed);
  connect(static_cast<ScanLibrary*>(m_lib), &ScanLibrary::scanProgress, this, &QScan::scanProgress);
}

bool
QScan::init()
{
  return m_lib->init();
}

QStringList
QScan::getDevices()
{
  QStringList names = m_lib->getDevices();
  return names;
}

Device
QScan::getDevice(QString device_name)
{
  return m_lib->getDevice(device_name);
}

bool
QScan::openDevice(QString device_name)
{
  Device device = m_lib->getDevice(device_name);
  m_logger->info(QString("Name: %1, vendor: %2, model: %3, type: %4")
                   .arg(device->name)
                   .arg(device->model)
                   .arg(device->vendor)
                   .arg(device->type));

  if (m_lib->openDevice(device_name)) {
    m_current_device = device;
    m_lib->getScannerOptions(device_name);
    return true;
  }

  return false;
}

bool
QScan::startScanning(QString device_name)
{
  if (m_lib->startScan(device_name)) {
  }
}

void
QScan::cancelScan(QString device_name)
{
  m_lib->cancelScan(device_name);
}

Options
QScan::options(QString device_name)
{
  return m_lib->options(device_name);
}
