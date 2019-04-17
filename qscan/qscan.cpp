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
#include "qscan.h"

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  #include "unix/sanelibrary.h"
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  #include "unix/scantwain.h"
#endif

QScan::QScan(QObject* parent)
  : QObject(parent)
{
  m_logger = Log4Qt::Logger::logger(QStringLiteral("Scan"));
#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  m_scan_lib = new SaneLibrary(this);
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  m_lib = new ScanTwain(this);
#endif
  connect(
    static_cast<ScanLibrary*>(m_scan_lib), &ScanLibrary::scanCompleted, this, &QScan::scanCompleted);
  connect(static_cast<ScanLibrary*>(m_scan_lib), &ScanLibrary::scanFailed, this, &QScan::scanFailed);
  connect(static_cast<ScanLibrary*>(m_scan_lib), &ScanLibrary::scanProgress, this, &QScan::scanProgress);
}

bool QScan::init()
{
  return m_scan_lib->init();
}

QStringList QScan::devices()
{
  return m_scan_lib->devices();
}

ScanDevice* QScan::device(QString device_name)
{
  return m_scan_lib->device(device_name);
}

bool QScan::openDevice(QString device_name)
{
  bool open =  m_scan_lib->openDevice(device_name) ;

  if (open) {
    m_scan_lib->getAvailableScannerOptions(device_name);
  }

  return open;
}

bool QScan::startScanning(QString device_name)
{
  return m_scan_lib->startScan(device_name);
}

void QScan::cancelScan(QString device_name)
{
  m_scan_lib->cancelScan(device_name);
}

ScanOptions QScan::options(QString device_name)
{
  return m_scan_lib->options(device_name);
}

bool QScan::topLeftX(ScanDevice* device, int& value)
{
  return m_scan_lib->topLeftX(device, value);
}

bool QScan::setTopLeftX(ScanDevice* device, int value)
{
  return m_scan_lib->setTopLeftX(device, value);
}

bool QScan::topLeftY(ScanDevice* device, int& value)
{
  return m_scan_lib->topLeftY(device, value);
}

bool QScan::setTopLeftY(ScanDevice* device, int value)
{
  return m_scan_lib->setTopLeftY(device, value);
}

bool QScan::bottomRightX(ScanDevice* device, int& value)
{
  return m_scan_lib->bottomRightX(device, value);
}

bool QScan::setBottomRightX(ScanDevice* device, int value)
{
  return m_scan_lib->setBottomRightX(device, value);
}

bool QScan::bottomRightY(ScanDevice* device, int& value)
{
  return m_scan_lib->bottomRightY(device, value);
}

bool QScan::setBottomRightY(ScanDevice* device, int value)
{
  return m_scan_lib->setBottomRightY(device, value);
}

bool QScan::contrast(ScanDevice* device, int& value)
{
  return m_scan_lib->contrast(device, value);
}

bool QScan::setContrast(ScanDevice* device, int value)
{
  return m_scan_lib->setContrast(device, value);
}

bool QScan::brightness(ScanDevice* device, int& value)
{
  return m_scan_lib->brightness(device, value);
}

bool QScan::setBrightness(ScanDevice* device, int value)
{
  return m_scan_lib->setBrightness(device, value);
}

bool QScan::resolution(ScanDevice* device, int& value)
{
  return m_scan_lib->resolution(device, value);
}

bool QScan::setResolution(ScanDevice* device, int value)
{
  return m_scan_lib->setResolution(device, value);
}

bool QScan::resolutionX(ScanDevice* device, int& value)
{
  return m_scan_lib->resolutionX(device, value);
}

bool QScan::setResolutionX(ScanDevice* device, int value)
{
  return m_scan_lib->setResolutionX(device, value);
}

bool QScan::resolutionY(ScanDevice* device, int& value)
{
  return m_scan_lib->resolutionY(device, value);
}

bool QScan::setResolutionY(ScanDevice* device, int value)
{
  return m_scan_lib->setResolutionY(device, value);
}

bool QScan::setPreview(ScanDevice* device)
{
  return m_scan_lib->setPreview(device);
}

bool QScan::clearPreview(ScanDevice* device)
{
  return m_scan_lib->clearPreview(device);
}
