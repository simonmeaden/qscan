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
#include "sanelibrary.h"

#  include "log4qt/consoleappender.h"
#  include "log4qt/log4qt.h"
#  include "log4qt/logger.h"
#  include "log4qt/logmanager.h"
#  include "log4qt/ttcclayout.h"

#  include "saneworker.h"

using namespace Log4Qt;

QMutex SaneLibrary::_mutex;

SaneLibrary::SaneLibrary(QObject* parent)
  : ScanLibrary(parent)
{
  QMutexLocker locker(&_mutex);
  m_logger = Log4Qt::Logger::logger(QStringLiteral("ScanSane"));
}

SaneLibrary::~SaneLibrary()
{
  emit finished();
}

bool SaneLibrary::init()
{
  QMutexLocker locker(&_mutex);
  SANE_Int version_code = 0;
  SANE_Status status;
  status = sane_init(&version_code, callbackWrapper);

  if (status == SANE_STATUS_GOOD) {
    m_version = Version(version_code);
    m_logger->info(QString("SANE version code: %1").arg(version_code));
    return true;
  }

  return false;
}

QStringList SaneLibrary::devices()
{
  QMutexLocker locker(&_mutex);
  QStringList list;
  SANE_Status sane_status = SANE_STATUS_GOOD;
  const SANE_Device** device_list = nullptr;
  const SANE_Device* current_device;
  size_t current_device_index = 0;
  sane_status = sane_get_devices(&device_list, SANE_FALSE);

  if (device_list) {
    m_logger->debug(QString("sane_get_devices status: %1").arg(sane_strstatus(sane_status)));

    while ((current_device = device_list[current_device_index]) != nullptr) {
      if (!current_device) {
        m_logger->debug(QString("No devices connected"));
        break;

      } else {
        ScanDevice* scanner = new ScanDevice(this);
        scanner->name = current_device->name;
        scanner->vendor = current_device->vendor;
        scanner->model = current_device->model;
        scanner->type = current_device->type;
        m_scanners.insert(scanner->name, scanner);
        list.append(scanner->name);
      }

      ++current_device_index;
    }
  }

  return list;
}

ScanDevice* SaneLibrary::device(QString device_name)
{
  QMutexLocker locker(&_mutex);
  return m_scanners.value(device_name);
}

ScanOptions SaneLibrary::options(QString device_name)
{
  return m_options.value(device_name);
}

bool SaneLibrary::openDevice(QString device_name)
{
  QMutexLocker locker(&_mutex);
  SANE_Status sane_status = SANE_STATUS_GOOD;
  SANE_Handle sane_handle = nullptr;
  sane_status = sane_open(device_name.toStdString().c_str(), &sane_handle);

  if (sane_status == SANE_STATUS_GOOD) {
    ScanDevice* device = m_scanners.value(device_name);
    device->sane_handle = sane_handle;
    m_logger->debug(QString("sane_open status: %1").arg(sane_strstatus(sane_status)));
    QThread* thread = new QThread;
    SaneWorker* scan_worker = new SaneWorker(device);
    // cleanup
    connect(this, &SaneLibrary::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, scan_worker, &SaneWorker::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    // worker
    connect(this, &SaneLibrary::startScanning, scan_worker, &SaneWorker::scan);
    connect(this, &SaneLibrary::getAvailableOptions, scan_worker, &SaneWorker::getAvailableScannerOptions);
    connect(scan_worker, &SaneWorker::availableScannerOptions, this, &SaneLibrary::receiveAvailableScannerOptions);
    connect(scan_worker, &SaneWorker::scanCompleted, this, &ScanLibrary::scanCompleted);
    connect(scan_worker, &SaneWorker::scanFailed, this, &ScanLibrary::scanFailed);
    connect(scan_worker, &SaneWorker::scanProgress, this, &ScanLibrary::scanProgress);
    //
    scan_worker->moveToThread(thread);
    thread->start();
    return true;
  }

  return false;
}

bool SaneLibrary::startScan(QString device_name)
{
  QMutexLocker locker(&_mutex);
  ScanDevice* device = m_scanners.value(device_name);

  if (device->sane_handle) {
    emit startScanning();
  }

  return false;
}

void SaneLibrary::cancelScan(QString device_name)
{
  QMutexLocker locker(&_mutex);
  ScanDevice* device = m_scanners.value(device_name);
  sane_cancel(device->sane_handle);
}

void SaneLibrary::callbackWrapper(SANE_String_Const resource, SANE_Char* name, SANE_Char* password)
{
  // TODO some form of authorisation ???
  //  std::string name_destination;
  //  std::string password_destination;
  //  _callback(std::string(resource), name_destination,
  //  password_destination);
  //  assert(name_destination.size() < SANE_MAX_USERNAME_LEN);
  //  assert(password_destination.size() < SANE_MAX_PASSWORD_LEN);
  //  strncpy(username, name_destination.c_str(), name_destination.size());
  //  strncpy(password, password_destination.c_str(),
  //  password_destination.size());
}

void SaneLibrary::exit()
{
  sane_exit();
}

void SaneLibrary::getAvailableScannerOptions(QString device_name)
{
  ScanDevice* device = m_scanners.value(device_name);
  ScanOptions options(this);
  emit getAvailableOptions(device, options);
}

void SaneLibrary::receiveAvailableScannerOptions(QString device_name, ScanOptions options)
{
  m_options.insert(device_name, options);
}

QRect SaneLibrary::geometry(QString device_name)
{
  ScanOptions options = m_options.value(device_name);
  return options.geometry();
}

const Version& SaneLibrary::version() const
{
  return m_version;
}

bool SaneLibrary::setIntValue(ScanDevice* device, QString name, int value)
{
  int option_id = m_options.value(device->name).optionId(name);

  if (option_id > -1) {
    sane_control_option(device->sane_handle, option_id, SANE_ACTION_SET_VALUE, &value, nullptr);
    return true;
  }

  return false;
}

bool SaneLibrary::setBoolValue(ScanDevice* device, QString name, bool value)
{
  int option_id = m_options.value(device->name).optionId(name);

  if (option_id > -1) {
    sane_control_option(device->sane_handle, option_id, SANE_ACTION_SET_VALUE, &value, nullptr);
    return true;
  }

  return false;
}

bool SaneLibrary::getIntValue(ScanDevice* device, QString name, int& value)
{
  SANE_Status current_status;
  SANE_Int v;
  int option_id = m_options.value(device->name).optionId(name);

  if (option_id > -1) {
    current_status =
      sane_control_option(device->sane_handle, option_id, SANE_ACTION_GET_VALUE, &v, nullptr);

    if (current_status == SANE_STATUS_GOOD) {
      value = v;
      return true;
    }
  }

  return false;
}

bool SaneLibrary::topLeftX(ScanDevice* device, int& value)
{
  return getIntValue(device, SANE_NAME_SCAN_TL_X, value);
}

bool SaneLibrary::setTopLeftX(ScanDevice* device, int value)
{
  return setIntValue(device, SANE_NAME_SCAN_TL_X, value);
}

bool SaneLibrary::topLeftY(ScanDevice* device, int& value)
{
  return getIntValue(device, SANE_NAME_SCAN_TL_Y, value);
}

bool SaneLibrary::setTopLeftY(ScanDevice* device, int value)
{
  return setIntValue(device, SANE_NAME_SCAN_TL_Y, value);
}

bool SaneLibrary::bottomRightX(ScanDevice* device, int& value)
{
  return getIntValue(device, SANE_NAME_SCAN_BR_X, value);
}

bool SaneLibrary::setBottomRightX(ScanDevice* device, int value)
{
  return setIntValue(device, SANE_NAME_SCAN_BR_X, value);
}

bool SaneLibrary::bottomRightY(ScanDevice* device, int& value)
{
  return getIntValue(device, SANE_NAME_SCAN_BR_Y, value);
}

bool SaneLibrary::setBottomRightY(ScanDevice* device, int value)
{
  return setIntValue(device, SANE_NAME_SCAN_BR_Y, value);
}

bool SaneLibrary::contrast(ScanDevice* device, int& value)
{
  return getIntValue(device, SANE_NAME_CONTRAST, value);
}

bool SaneLibrary::setContrast(ScanDevice* device, int value)
{
  return setIntValue(device, SANE_NAME_CONTRAST, value);
}

bool SaneLibrary::brightness(ScanDevice* device, int& value)
{
  return getIntValue(device, SANE_NAME_BRIGHTNESS, value);
}

bool SaneLibrary::setBrightness(ScanDevice* device, int value)
{
  return setIntValue(device, SANE_NAME_BRIGHTNESS, value);
}

bool SaneLibrary::resolution(ScanDevice* device, int& value)
{
  return getIntValue(device, SANE_NAME_SCAN_RESOLUTION, value);
}

bool SaneLibrary::setResolution(ScanDevice* device, int value)
{
  return setIntValue(device, SANE_NAME_SCAN_RESOLUTION, value);
}

bool SaneLibrary::resolutionX(ScanDevice* device, int& value)
{
  return getIntValue(device, SANE_NAME_SCAN_X_RESOLUTION, value);
}

bool SaneLibrary::setResolutionX(ScanDevice* device, int value)
{
  return setIntValue(device, SANE_NAME_SCAN_X_RESOLUTION, value);
}

bool SaneLibrary::resolutionY(ScanDevice* device, int& value)
{
  return getIntValue(device, SANE_NAME_SCAN_Y_RESOLUTION, value);
}

bool SaneLibrary::setResolutionY(ScanDevice* device, int value)
{
  return setIntValue(device, SANE_NAME_SCAN_Y_RESOLUTION, value);
}

bool SaneLibrary::setPreview(ScanDevice* device)
{
  return setBoolValue(device, SANE_NAME_PREVIEW, SANE_TRUE);
}

bool SaneLibrary::clearPreview(ScanDevice* device)
{
  return setBoolValue(device, SANE_NAME_PREVIEW, SANE_FALSE);
}


static void auth_callback(SANE_String_Const resource, SANE_Char* username, SANE_Char* password)
{
  // TODO some form of authorisation ???
  //  std::string name_destination;
  //  std::string password_destination;
  //  _callback(std::string(resource), name_destination,
  //  password_destination);
  //  assert(name_destination.size() < SANE_MAX_USERNAME_LEN);
  //  assert(password_destination.size() < SANE_MAX_PASSWORD_LEN);
  //  strncpy(username, name_destination.c_str(), name_destination.size());
  //  strncpy(password, password_destination.c_str(),
  //  password_destination.size());
}
