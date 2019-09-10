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
#include "qscanner.h"
#include "scandevice.h"
#include "logging.h"

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  #include "unix/sanelibrary.h"
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  #include "win/twainlibrary.h"
#endif

namespace QScanner {

const int QScanner::m_major_version = QSCAN_VERSION_MAJOR;
const int QScanner::m_minor_version = QSCAN_VERSION_MINOR;
const int QScanner::m_build_version = QSCAN_VERSION_BUILD;
const QString QScanner::m_version = QString("%1.%2.%3")
                                    .arg(QScanner::m_major_version)
                                    .arg(QScanner::m_minor_version)
                                    .arg(QScanner::m_build_version);

QScanner::QScanner(QObject* parent)
  : QObject(parent)
  , m_manufacturer(MANUFACTURER)
  , m_product(PRODUCT)
  , m_product_name(PRODUCT_NAME)
{
  initLib();
}

QScanner::QScanner(const QString& manufacturer,
                   const QString& product,
                   const QString& product_name,
                   QObject* parent)
  : QObject(parent)
  , m_manufacturer(manufacturer)
  , m_product(product)
  , m_product_name(product_name)
{
  initLib();
}

void QScanner::initLib()
{
  qRegisterMetaType<ScanOptions>();
  qRegisterMetaType<ScanDevice>();
  qRegisterMetaType<RangeData>();

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  m_scan_lib = new SaneLibrary(m_manufacturer, m_product, m_product_name, this);
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  m_scan_lib = new TwainLibrary(m_manufacturer, m_product, m_product_name, this);
#endif
  auto* scan_lib = dynamic_cast<ScanLibrary*>(m_scan_lib);

  if (scan_lib) {
    connect(scan_lib, &ScanLibrary::scanCompleted, this, &QScanner::scanCompleted);
    connect(scan_lib, &ScanLibrary::scanFailed, this, &QScanner::scanFailed);
    connect(scan_lib, &ScanLibrary::scanOpenFailed, this, &QScanner::scanOpenFailed);
    connect(scan_lib, &ScanLibrary::scanProgress, this, &QScanner::scanProgress);
    connect(scan_lib, &ScanLibrary::optionsSet, this, &QScanner::optionsSet);
    //    connect(scan_lib, &ScanLibrary::sourceChanged, this, &QScan::sourceChanged);
    //    connect(scan_lib, &ScanLibrary::modeChanged, this, &QScan::modeChanged);
    connect(scan_lib, &ScanLibrary::optionChanged, this, &QScanner::optionChanged);
  }
}

QString QScanner::product_name() const
{
  return m_product_name;
}

void QScanner::setProduct_name(const QString& product_name)
{
  m_product_name = product_name;
  m_scan_lib->setProductName(product_name);
}

QString QScanner::product() const
{
  return m_product;
}

void QScanner::setProduct(const QString& product)
{
  m_product = product;
  m_scan_lib->setProduct(product);
}

QString QScanner::manufacturer() const
{
  return m_manufacturer;
}

void QScanner::setManufacturer(const QString& manufacturer)
{
  m_manufacturer = manufacturer;
  m_scan_lib->setManufacturer(manufacturer);
}

bool QScanner::init()
{
  bool success = m_scan_lib->init();

  if (success) {
    m_scan_lib->detectDevices();
  }

  return success;
}

void QScanner::retestDevices()
{
  m_scan_lib->detectDevices();
}

QStringList QScanner::devices()
{
  return m_scan_lib->devices();
}

bool QScanner::isDeviceAvaliable()
{
  if (m_current_device) {
    return true;
  }

  return false;
}

bool QScanner::startScanning(const QString& device_name)
{
  bool open = m_scan_lib->detectAvailableOptions(device_name);

  if (open) {
    qCInfo(LogQScan) << tr("Starting scan");
    return m_scan_lib->startScan(device_name);
  }

  return false;
}

void QScanner::cancelScan(/*const QString& device_name*/)
{
  qCInfo(LogQScan) << tr("Cancelled scan");
  m_scan_lib->cancelScan(/*device_name*/);
}

bool QScanner::isScanning()
{
  return m_scan_lib->isScanning();
}

bool QScanner::isAvailable(ScanOptions::AvailableOptions option)
{
  if (m_current_device) {
    return m_current_device->options()->isAvailable(option);
  }

  return false;
}

QVariant QScanner::value(ScanOptions::AvailableOptions option)
{
  if (isAvailable(option)) {
    return m_current_device->options()->optionValue(option);
  }

  return QVariant();
}

QString QScanner::vendor()
{
  if (m_current_device) {
    return m_current_device->vendor();
  }

  return QString();
}

QString QScanner::model()
{
  if (m_current_device) {
    return m_current_device->model();
  }

  return QString();
}

QString QScanner::type()
{
  if (m_current_device) {
    return m_current_device->type();
  }

  return QString();
}

QString QScanner::name()
{
  if (m_current_device) {
    return m_current_device->descriptor();
  }

  return QString();
}

QString QScanner::displayName()
{
  if (m_current_device) {
    return m_current_device->displayName();
  }

  return nullptr;
}

ScanOptions* QScanner::options()
{
  if (m_current_device) {
    return m_current_device->options();
  }

  return nullptr;
}

bool QScanner::setDevice(const QString& name)
{
  ScanDevice* device = m_scan_lib->device(name);

  if (device && device != m_current_device) {
    m_current_device = device;
    m_scan_lib->setCurrentDevice(device);
    return true;
  }

  return false;
}

int QScanner::topLeftX()
{
  return m_scan_lib->currentDevice()->options()->right();
}

void QScanner::setTopLeftX(int value)
{
  m_scan_lib->setTopLeftX(m_current_device, value);
}

int QScanner::topLeftY()
{
  return m_current_device->options()->top();
}

void QScanner::setTopLeftY(int value)
{
  m_scan_lib->setTopLeftY(m_current_device, value);
}

int QScanner::bottomRightX()
{
  return m_current_device->options()->left();
}

void QScanner::setBottomRightX(int value)
{
  m_scan_lib->setBottomRightX(m_current_device, value);
}

int QScanner::bottomRightY()
{
  return m_current_device->options()->right();
}

void QScanner::setBottomRightY(int value)
{
  m_scan_lib->setBottomRightY(m_current_device, value);
}

int QScanner::contrast()
{
  return m_current_device->options()->contrast();
}

void QScanner::setContrast(int value)
{
  m_scan_lib->setContrast(m_current_device, value);
}

int QScanner::brightness()
{
  return m_current_device->options()->brightness();
}

void QScanner::setBrightness(int value)
{
  m_scan_lib->setBrightness(m_current_device, value);
}

int QScanner::resolution()
{
  return m_current_device->options()->resolution();
}

void QScanner::setResolution(int value)
{
  m_scan_lib->setResolution(m_current_device, value);
}

int QScanner::resolutionX()
{
  return m_current_device->options()->resolutionX();
}

void QScanner::setResolutionX(int value)
{
  m_scan_lib->setResolutionX(m_current_device, value);
}

int QScanner::resolutionY()
{
  return m_current_device->options()->resolutionY();
}

void QScanner::setResolutionY(int value)
{
  m_scan_lib->setResolutionY(m_current_device, value);
}

void QScanner::setPreview()
{
  m_scan_lib->setPreview(m_current_device);
}

void QScanner::clearPreview()
{
  m_scan_lib->clearPreview(m_current_device);
}

void QScanner::setScanMode(const QString& mode)
{
  m_scan_lib->setMode(m_current_device, mode);
}

void QScanner::setSource(const QString& source)
{
  m_scan_lib->setSource(m_current_device, source);
}

} // end of namespace QScanner
