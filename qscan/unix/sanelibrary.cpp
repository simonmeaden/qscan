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
#include "sanelibrary.h"

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)

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

SaneLibrary::~SaneLibrary() {}

bool
SaneLibrary::init()
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

QStringList
SaneLibrary::getDevices()
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
        Device scanner = Device(new ScanDevice());
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

Device
SaneLibrary::getDevice(QString device_name)
{
  QMutexLocker locker(&_mutex);
  return m_scanners.value(device_name);
}

Options
SaneLibrary::options(QString device_name)
{
  return m_options.value(device_name);
}

bool
SaneLibrary::openDevice(QString device_name)
{
  QMutexLocker locker(&_mutex);
  SANE_Status sane_status = SANE_STATUS_GOOD;
  SANE_Handle sane_handle = nullptr;
  sane_status = sane_open(device_name.toStdString().c_str(), &sane_handle);

  if (sane_status == SANE_STATUS_GOOD) {
    m_scanners.value(device_name)->sane_handle = sane_handle;
    m_logger->debug(QString("sane_open status: %1").arg(sane_strstatus(sane_status)));
    return true;
  }

  return false;
}

bool
SaneLibrary::startScan(QString device_name)
{
  QMutexLocker locker(&_mutex);
  Device device = m_scanners.value(device_name);

  if (device->sane_handle) {
    scan(device);
  }

  return false;
}

void
SaneLibrary::cancelScan(QString device_name)
{
  QMutexLocker locker(&_mutex);
  Device device = m_scanners.value(device_name);
  sane_cancel(device->sane_handle);
}

void
SaneLibrary::scan(Device device)
{
  QThread* thread = new QThread;
  SaneWorker* scan_worker = new SaneWorker(device);
  connect(scan_worker, &SaneWorker::finished, thread, &QThread::quit);
  connect(scan_worker, &SaneWorker::finished, scan_worker, &SaneWorker::deleteLater);
  connect(thread, &QThread::finished, thread, &QThread::deleteLater);
  connect(thread, &QThread::started, scan_worker, &SaneWorker::scan);
  connect(scan_worker, &SaneWorker::scanCompleted, this, &ScanLibrary::scanCompleted);
  connect(scan_worker, &SaneWorker::scanFailed, this, &ScanLibrary::scanFailed);
  connect(scan_worker, &SaneWorker::scanProgress, this, &ScanLibrary::scanProgress);
  scan_worker->moveToThread(thread);
  thread->start();
}

void
SaneLibrary::callbackWrapper(SANE_String_Const resource, SANE_Char* name, SANE_Char* password)
{}

void
SaneLibrary::exit()
{
  sane_exit();
}

void
SaneLibrary::getScannerOptions(QString device_name)
{
  QMutexLocker locker(&_mutex);
  Device device = m_scanners.value(device_name);
  SANE_Status current_status;
  SANE_Int number_of_options = 0;
  SANE_Int option_id = 1;
  current_status =
    sane_control_option(device->sane_handle, 0, SANE_ACTION_GET_VALUE, &number_of_options, nullptr);

  if (current_status == SANE_STATUS_GOOD) {
    Options options = Options(new ScanOptions());
    m_options.insert(device_name, options);

    for (SANE_Int i = 1; i < number_of_options; ++option_id) {
      if (const auto current_option = sane_get_option_descriptor(device->sane_handle, option_id)) {
        int id = option_id;
        QString name(current_option->name);
        //        if (current_option->name && SANE_OPTION_IS_ACTIVE(current_option->cap)) {
        //          options->setName(QString());
        //        }
        //        if (current_option->title) {
        //          options->setTitle(QString(current_option->title));
        //        }
        //        if (current_option->desc) {
        //          options->setDescription(QString(current_option->desc));
        //        }

        if (name == SANE_NAME_SCAN_TL_X) {
          m_tl_x_opt = option_id;
        }

        if (name == SANE_NAME_SCAN_TL_Y) {
          m_tl_y_opt = option_id;
        }

        if (name == SANE_NAME_SCAN_BR_X) {
          m_br_x_opt = option_id;
        }

        if (name == SANE_NAME_SCAN_BR_Y) {
          m_br_y_opt = option_id;
        }

        if (name == SANE_NAME_SCAN_RESOLUTION) {
          m_resolution_opt = option_id;
        }

        if (name == SANE_NAME_SCAN_X_RESOLUTION) {
          m_resolution_opt_x = option_id;
        }

        if (name == SANE_NAME_SCAN_Y_RESOLUTION) {
          m_resolution_opt_y = option_id;
        }

        if (name == SANE_NAME_CONTRAST) {
          m_contrast_opt = option_id;
        }

        if (name == SANE_NAME_BRIGHTNESS) {
          m_brightness_opt = option_id;
        }

        if (name == SANE_NAME_SCAN_SOURCE) {
          m_source_opt = option_id;
        }

        if (name == SANE_NAME_SCAN_MODE) {
          m_mode_opt = option_id;
        }

        ++i;
      }
    }
    // get the initial geometry
    options->setTopLeftX(options->getScannerValue(device, m_tl_x_opt));
    options->setTopLeftY(options->getScannerValue(device, m_tl_y_opt));
    options->setBottomRightX(options->getScannerValue(device, m_br_x_opt));
    options->setBottomRightY(options->getScannerValue(device, m_br_y_opt));
    options->setContrast(options->getScannerValue(device, m_contrast_opt));
    options->setBrightness(options->getScannerValue(device, m_brightness_opt));
    options->setScanResolution(options->getScannerValue(device, m_resolution_opt));
    if (options->scanResolution() >= 0) {
      // generally you either get resolution OR resolution_x and resolution_y.
      options->setScanResolutionX(options->getScannerValue(device, m_resolution_opt_x));
      options->setScanResolutionY(options->getScannerValue(device, m_resolution_opt_y));
    }
    options->setScanMode(options->getScannerValue(device, m_resolution_opt_y));
  }
}

QRect
SaneLibrary::geometry(QString device_name)
{
  Options options = m_options.value(device_name);
  if (!options.isNull())
    return options->geometry();
  return QRect();
}

int
SaneLibrary::contrast(QString device_name)
{
  Options options = m_options.value(device_name);
  if (!options.isNull())
    return options->contrast();
  return -1;
}

int
SaneLibrary::brightness(QString device_name)
{
  Options options = m_options.value(device_name);
  if (!options.isNull())
    return options->brightness();
  return -1;
}

const Version&
SaneLibrary::version() const
{
  return m_version;
}

static void
auth_callback(SANE_String_Const resource, SANE_Char* username, SANE_Char* password)
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

#endif
