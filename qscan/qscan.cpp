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
  #include "win/twainlibrary.h"
#endif

QScan::QScan(QObject* parent)
  : QObject(parent)
{
#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  m_scan_lib = new SaneLibrary(this);
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  m_lib = new ScanTwain(this);
#endif
  auto* scan_lib = dynamic_cast<ScanLibrary*>(m_scan_lib);

  if (scan_lib) {
    connect(scan_lib, &ScanLibrary::scanCompleted, this, &QScan::scanCompleted);
    connect(scan_lib, &ScanLibrary::scanFailed, this, &QScan::scanFailed);
    connect(scan_lib, &ScanLibrary::scanProgress, this, &QScan::scanProgress);
    connect(scan_lib, &ScanLibrary::optionsSet, this, &QScan::optionsSet);
    connect(scan_lib, &ScanLibrary::sourceChanged, this, &QScan::sourceChanged);
    connect(scan_lib, &ScanLibrary::modeChanged, this, &QScan::modeChanged);
  }
}

bool QScan::init()
{
  return m_scan_lib->init();
}

QStringList QScan::devices()
{
  return m_scan_lib->devices();
}

ScanDevice* QScan::device(const QString& device_name)
{
  return m_scan_lib->device(device_name);
}

bool QScan::openDevice(const QString& device_name)
{
  bool open = m_scan_lib->detectAvailableOptions(device_name);

  if (open) {
    qInfo() << tr("Scanner %1 open.").arg(device_name);
    m_scan_lib->getAvailableScannerOptions(device_name);
  }

  return open;
}

bool QScan::startScanning(const QString& device_name)
{
  bool open = m_scan_lib->detectAvailableOptions(device_name);

  qInfo() << tr("Starting scan");
  return m_scan_lib->startScan(device_name);
}

void QScan::cancelScan(/*const QString& device_name*/)
{
  qInfo() << tr("Cancelled scan");
  m_scan_lib->cancelScan(/*device_name*/);
}

bool QScan::isScanning()
{
  return m_scan_lib->isScanning();
}

int QScan::topLeftX(ScanDevice* device)
{
  return device->options->topLeftX();
}

void QScan::setTopLeftX(ScanDevice* device, int value)
{
  m_scan_lib->setTopLeftX(device, value);
}

int QScan::topLeftY(ScanDevice* device)
{
  return device->options->topLeftY();
}

void QScan::setTopLeftY(ScanDevice* device, int value)
{
  m_scan_lib->setTopLeftY(device, value);
}

int QScan::bottomRightX(ScanDevice* device)
{
  return device->options->bottomRightX();
}

void QScan::setBottomRightX(ScanDevice* device, int value)
{
  m_scan_lib->setBottomRightX(device, value);
}

int QScan::bottomRightY(ScanDevice* device)
{
  return device->options->bottomRightY();
}

void QScan::setBottomRightY(ScanDevice* device, int value)
{
  m_scan_lib->setBottomRightY(device, value);
}

int QScan::contrast(ScanDevice* device)
{
  return device->options->contrast();
}

void QScan::setContrast(ScanDevice* device, int value)
{
  m_scan_lib->setContrast(device, value);
}

int QScan::brightness(ScanDevice* device)
{
  return device->options->brightness();
}

void QScan::setBrightness(ScanDevice* device, int value)
{
  m_scan_lib->setBrightness(device, value);
}

int QScan::resolution(ScanDevice* device)
{
  return device->options->resolution();
}

void QScan::setResolution(ScanDevice* device, int value)
{
  m_scan_lib->setResolution(device, value);
}

int QScan::resolutionX(ScanDevice* device)
{
  return device->options->resolutionX();
}

void QScan::setResolutionX(ScanDevice* device, int value)
{
  m_scan_lib->setResolutionX(device, value);
}

int QScan::resolutionY(ScanDevice* device)
{
  return device->options->resolutionY();
}

void QScan::setResolutionY(ScanDevice* device, int value)
{
  m_scan_lib->setResolutionY(device, value);
}

void QScan::setPreview(ScanDevice* device)
{
  m_scan_lib->setPreview(device);
}

void QScan::clearPreview(ScanDevice* device)
{
  m_scan_lib->clearPreview(device);
}

void QScan::setScanMode(ScanDevice* device, const QString& mode)
{
  m_scan_lib->setMode(device, mode);
}

void QScan::setSource(ScanDevice* device, const QString& source)
{
  m_scan_lib->setSource(device, source);
}
