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
#include "qscan.h"
#include "scandevice.h"
#include "logging.h"

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  #include "unix/sanelibrary.h"
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  #include "win/twainlibrary.h"
#endif

namespace QScanner {


QScan::QScan(QObject* parent)
  : QObject(parent)
  , m_current_device(nullptr)
{
  qRegisterMetaType<ScanOptions>();
  qRegisterMetaType<ScanDevice>();
  qRegisterMetaType<RangeData>();

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  m_scan_lib = new SaneLibrary(this);
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  m_scan_lib = new ScanTwain(this);
#endif
  auto* scan_lib = dynamic_cast<ScanLibrary*>(m_scan_lib);

  if (scan_lib) {
    connect(scan_lib, &ScanLibrary::scanCompleted, this, &QScan::scanCompleted);
    connect(scan_lib, &ScanLibrary::scanFailed, this, &QScan::scanFailed);
    connect(scan_lib, &ScanLibrary::scanOpenFailed, this, &QScan::scanOpenFailed);
    connect(scan_lib, &ScanLibrary::scanProgress, this, &QScan::scanProgress);
    connect(scan_lib, &ScanLibrary::optionsSet, this, &QScan::optionsSet);
    //    connect(scan_lib, &ScanLibrary::sourceChanged, this, &QScan::sourceChanged);
    //    connect(scan_lib, &ScanLibrary::modeChanged, this, &QScan::modeChanged);
    connect(scan_lib, &ScanLibrary::optionChanged, this, &QScan::optionChanged);
  }
}

bool QScan::init()
{
  bool success = m_scan_lib->init();

  if (success) {
    m_scan_lib->detectDevices();
  }

  return success;
}

void QScan::retestDevices()
{
  m_scan_lib->detectDevices();
}

QStringList QScan::devices()
{
  return m_scan_lib->devices();
}

bool QScan::isDeviceAvaliable()
{
  if (m_current_device) {
    return true;
  }

  return false;
}

//ScanDevice* QScan::device(const QString& device_name)
//{
//  return m_scan_lib->device(device_name);
//}

//bool QScan::openDevice(const QString& device_name)
//{
//  bool open = m_scan_lib->detectAvailableOptions(device_name);

//  if (open) {
//    qCInfo(LogQScan) << tr("Scanner %1 open.").arg(device_name);
//    ScanDevice* device = m_scan_lib->getAvailableScannerOptions(device_name);
//    m_current_device = device;
//  }

//  return open;
//}

bool QScan::startScanning(const QString& device_name)
{
  bool open = m_scan_lib->detectAvailableOptions(device_name);

  if (open) {
    qCInfo(LogQScan) << tr("Starting scan");
    return m_scan_lib->startScan(device_name);
  }

  return false;
}

void QScan::cancelScan(/*const QString& device_name*/)
{
  qCInfo(LogQScan) << tr("Cancelled scan");
  m_scan_lib->cancelScan(/*device_name*/);
}

bool QScan::isScanning()
{
  return m_scan_lib->isScanning();
}

bool QScan::isAvailable(ScanOptions::AvailableOptions option)
{
  if (m_current_device) {
    return m_current_device->options()->isAvailable(option);
  }

  return false;
}

QVariant QScan::value(ScanOptions::AvailableOptions option)
{
  if (isAvailable(option)) {
    return m_current_device->options()->optionValue(option);
  }

  return QVariant();
}

QString QScan::vendor()
{
  if (m_current_device) {
    return m_current_device->vendor();
  }

  return QString();
}

QString QScan::model()
{
  if (m_current_device) {
    return m_current_device->model();
  }

  return QString();
}

QString QScan::type()
{
  if (m_current_device) {
    return m_current_device->type();
  }

  return QString();
}

QString QScan::name()
{
  if (m_current_device) {
    return m_current_device->descriptor();
  }

  return QString();
}

QString QScan::displayName()
{
  if (m_current_device) {
    return m_current_device->displayName();
  }

  return nullptr;
}

ScanOptions* QScan::options()
{
  if (m_current_device) {
    return m_current_device->options();
  }

  return nullptr;
}

bool QScan::setDevice(const QString& name)
{
  ScanDevice* device = m_scan_lib->device(name);

  if (device && device != m_current_device) {
    m_current_device = device;
    m_scan_lib->setCurrentDevice(device);
    return true;
  }

  return false;
}

int QScan::topLeftX()
{
  return m_current_device->options()->right();
}

void QScan::setTopLeftX(int value)
{
  m_scan_lib->setTopLeftX(m_current_device, value);
}

int QScan::topLeftY()
{
  return m_current_device->options()->top();
}

void QScan::setTopLeftY(int value)
{
  m_scan_lib->setTopLeftY(m_current_device, value);
}

int QScan::bottomRightX()
{
  return m_current_device->options()->left();
}

void QScan::setBottomRightX(int value)
{
  m_scan_lib->setBottomRightX(m_current_device, value);
}

int QScan::bottomRightY()
{
  return m_current_device->options()->right();
}

void QScan::setBottomRightY(int value)
{
  m_scan_lib->setBottomRightY(m_current_device, value);
}

int QScan::contrast()
{
  return m_current_device->options()->contrast();
}

void QScan::setContrast(int value)
{
  m_scan_lib->setContrast(m_current_device, value);
}

int QScan::brightness()
{
  return m_current_device->options()->brightness();
}

void QScan::setBrightness(int value)
{
  m_scan_lib->setBrightness(m_current_device, value);
}

int QScan::resolution()
{
  return m_current_device->options()->resolution();
}

void QScan::setResolution(int value)
{
  m_scan_lib->setResolution(m_current_device, value);
}

int QScan::resolutionX()
{
  return m_current_device->options()->resolutionX();
}

void QScan::setResolutionX(int value)
{
  m_scan_lib->setResolutionX(m_current_device, value);
}

int QScan::resolutionY()
{
  return m_current_device->options()->resolutionY();
}

void QScan::setResolutionY(int value)
{
  m_scan_lib->setResolutionY(m_current_device, value);
}

void QScan::setPreview()
{
  m_scan_lib->setPreview(m_current_device);
}

void QScan::clearPreview()
{
  m_scan_lib->clearPreview(m_current_device);
}

void QScan::setScanMode(const QString& mode)
{
  m_scan_lib->setMode(m_current_device, mode);
}

void QScan::setSource(const QString& source)
{
  m_scan_lib->setSource(m_current_device, source);
}

} // end of namespace QScanner
