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

#include "log4qt/consoleappender.h"
#include "log4qt/log4qt.h"
#include "log4qt/logger.h"
#include "log4qt/logmanager.h"
#include "log4qt/ttcclayout.h"

#include "saneworker.h"

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
    m_logger->debug(
      QString("sane_get_devices status: %1").arg(sane_strstatus(sane_status)));

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
SaneLibrary::getDevice(QString name)
{
  QMutexLocker locker(&_mutex);

  return m_scanners.value(name);
}

bool
SaneLibrary::openDevice(QString name)
{
  QMutexLocker locker(&_mutex);

  SANE_Status sane_status = SANE_STATUS_GOOD;
  SANE_Handle sane_handle = nullptr;

  sane_status = sane_open(name.toStdString().c_str(), &sane_handle);

  if (sane_status == SANE_STATUS_GOOD) {
    m_scanners.value(name)->sane_handle = sane_handle;
    m_logger->debug(
      QString("sane_open status: %1").arg(sane_strstatus(sane_status)));
    return true;
  }

  return false;
}

bool
SaneLibrary::startScan(QString name)
{
  QMutexLocker locker(&_mutex);

  Device device = m_scanners.value(name);

  if (device->sane_handle) {
    scan(device);
  }

  return false;
}

void
SaneLibrary::cancelScan(QString name)
{
  QMutexLocker locker(&_mutex);

  Device device = m_scanners.value(name);
  sane_cancel(device->sane_handle);
}

void
SaneLibrary::scan(Device device)
{
  QThread* thread = new QThread;
  SaneWorker* scan_worker = new SaneWorker(device);
  connect(scan_worker, &SaneWorker::finished, thread, &QThread::quit);
  connect(
    scan_worker, &SaneWorker::finished, scan_worker, &SaneWorker::deleteLater);
  connect(thread, &QThread::finished, thread, &QThread::deleteLater);
  connect(thread, &QThread::started, scan_worker, &SaneWorker::scan);
  connect(
    scan_worker, &SaneWorker::scanCompleted, this, &ScanLibrary::scanCompleted);
  connect(scan_worker, &SaneWorker::scanFailed, this, &ScanLibrary::scanFailed);
  connect(
    scan_worker, &SaneWorker::scanProgress, this, &ScanLibrary::scanProgress);
  scan_worker->moveToThread(thread);
  thread->start();
}

void
SaneLibrary::callbackWrapper(SANE_String_Const resource,
                             SANE_Char* name,
                             SANE_Char* password)
{}

void
SaneLibrary::exit()
{
  sane_exit();
}

ScanOptions*
SaneLibrary::options(QString name)
{
  QMutexLocker locker(&_mutex);

  Device device = m_scanners.value(name);
  SANE_Status current_status;
  SANE_Int number_of_options = 0;
  SANE_Int option_id = 1;

  current_status = sane_control_option(
    device->sane_handle, 0, SANE_ACTION_GET_VALUE, &number_of_options, nullptr);
  if (current_status == SANE_STATUS_GOOD) {
    Options options = Options(new ScanOptions());
    for (SANE_Int i = 1; i < number_of_options; ++option_id) {
      if (const auto current_option =
            sane_get_option_descriptor(device->sane_handle, option_id)) {
        int id = option_id;
        //        if (current_option->name/* &&
        //            SANE_OPTION_IS_ACTIVE(current_option->cap)*/) {
        QString name(current_option->name);
        QString title(current_option->title);
        QString desc(current_option->desc);
        int size = current_option->size;
        int type = current_option->type;

        bool bool_value;
        int int_value;
        SANE_Word word_value;
        std::string string_value;

        switch (type) {
          case SANE_TYPE_BOOL:
            if (current_option->size == sizeof(SANE_Bool)) {
              current_status = sane_control_option(device->sane_handle,
                                                   id,
                                                   SANE_ACTION_GET_VALUE,
                                                   &bool_value,
                                                   nullptr);
            }
            break;
          case SANE_TYPE_INT:
            if (current_option->size == sizeof(SANE_Int)) {
              if (current_option->size == sizeof(SANE_Bool)) {
                current_status = sane_control_option(device->sane_handle,
                                                     id,
                                                     SANE_ACTION_GET_VALUE,
                                                     &int_value,
                                                     nullptr);
              }
              break;
              case SANE_TYPE_FIXED:
                if (current_option->size == sizeof(SANE_Fixed)) {
                  if (current_option->size == sizeof(SANE_Bool)) {
                    current_status = sane_control_option(device->sane_handle,
                                                         id,
                                                         SANE_ACTION_GET_VALUE,
                                                         &word_value,
                                                         nullptr);
                  }
                  break;
                  case SANE_TYPE_STRING:
                    current_status = sane_control_option(device->sane_handle,
                                                         id,
                                                         SANE_ACTION_GET_VALUE,
                                                         &string_value,
                                                         nullptr);
                    break;
                    //                    case SANE_TYPE_BUTTON:
                    //                      m_options.emplace_back(m_device_handle,
                    //                      Button{}, info); break;
                    //                    case SANE_TYPE_GROUP:
                    //                      m_options.emplace_back(m_device_handle,
                    //                      Group{}, info); break;
                  default:
                    break;
                }
            }
        }

        ++i;
      }
    }
  }
}

void
SaneLibrary::setOptions(QString name, ScanOptions* options)
{
  QMutexLocker locker(&_mutex);

  Device device = m_scanners.value(name);
  SANE_Status current_status;
}

const Version&
SaneLibrary::version() const
{
  return m_version;
}

static void
auth_callback(SANE_String_Const resource,
              SANE_Char* username,
              SANE_Char* password)
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

// static void
// writePnmHeader(SANE_Frame format, int width, int height, int depth, FILE*
// ofp)
//{
//  switch (format) {
//    case SANE_FRAME_RED:
//    case SANE_FRAME_GREEN:
//    case SANE_FRAME_BLUE:
//    case SANE_FRAME_RGB:
//      fprintf(ofp,
//              "P6\n# SANE data follows\n%d %d\n%d\n",
//              width,
//              height,
//              (depth <= 8) ? 255 : 65535);
//      break;

//    default:
//      if (depth == 1) {
//        fprintf(ofp, "P4\n# SANE data follows\n%d %d\n", width, height);

//      } else {
//        fprintf(ofp,
//                "P5\n# SANE data follows\n%d %d\n%d\n",
//                width,
//                height,
//                (depth <= 8) ? 255 : 65535);
//      }

//      break;
//  }
//}

// static SANE_Status
// scanIt(FILE* ofp)
//{
//  int i, len, first_frame = 1, offset = 0, must_buffer = 0, hundred_percent;
//  SANE_Byte min = 0xff, max = 0;
//  SANE_Parameters parm;
//  SANE_Status status;
//  Image image = { 0, 0, 0, 0, 0 };
//  static const char* format_name[] = { "gray", "RGB", "red", "green", "blue"
//  }; SANE_Word total_bytes = 0, expected_bytes; SANE_Int hang_over = -1;

//  do {
//    if (!first_frame) {
//      status = sane_start(device);

//      if (status != SANE_STATUS_GOOD) {
//        goto cleanup;
//      }
//    }

//    status = sane_get_parameters(device, &parm);

//    if (status != SANE_STATUS_GOOD) {
//      goto cleanup;
//    }

//    if (first_frame) {
//      switch (parm.format) {
//        case SANE_FRAME_RED:
//        case SANE_FRAME_GREEN:
//        case SANE_FRAME_BLUE:
//          assert(parm.depth == 8);
//          must_buffer = 1;
//          offset = parm.format - SANE_FRAME_RED;
//          break;

//        case SANE_FRAME_RGB:
//          assert((parm.depth == 8) || (parm.depth == 16));

//        case SANE_FRAME_GRAY:
//          assert((parm.depth == 1) || (parm.depth == 8) || (parm.depth ==
//          16));

//          if (parm.lines < 0) {
//            must_buffer = 1;
//            offset = 0;

//          } else {
//            writePnmHeader(
//              parm.format, parm.pixels_per_line, parm.lines, parm.depth,
//              ofp);
//          }

//          break;

//        default:
//          break;
//      }

//      if (must_buffer) {
//        image.width = parm.bytes_per_line;

//        if (parm.lines >= 0) {
//          image.height = parm.lines - STRIP_HEIGHT + 1;

//        } else {
//          image.height = 0;
//        }

//        image.x = image.width - 1;
//        image.y = -1;

//        if (!advance(&image)) {
//          status = SANE_STATUS_NO_MEM;
//          goto cleanup;
//        }
//      }

//    } else {
//      assert(parm.format >= SANE_FRAME_RED && parm.format <=
//      SANE_FRAME_BLUE); offset = parm.format - SANE_FRAME_RED; image.x =
//      image.y = 0;
//    }

//    hundred_percent =
//      parm.bytes_per_line * parm.lines *
//      ((parm.format == SANE_FRAME_RGB || parm.format == SANE_FRAME_GRAY) ? 1
//                                                                         :
//                                                                         3);

//    while (1) {
//      double progr;
//      status = sane_read(device, buffer, buffer_size, &len);
//      total_bytes += (SANE_Word)len;
//      progr = ((total_bytes * 100.) / (double)hundred_percent);

//      if (progr > 100.) {
//        progr = 100.;
//      }

//      if (status != SANE_STATUS_GOOD) {
//        if (status != SANE_STATUS_EOF) {
//          return status;
//        }

//        break;
//      }

//      if (must_buffer) {
//        switch (parm.format) {
//          case SANE_FRAME_RED:
//          case SANE_FRAME_GREEN:
//          case SANE_FRAME_BLUE:
//            for (i = 0; i < len; ++i) {
//              image.data[offset + 3 * i] = buffer[i];

//              if (!advance(&image)) {
//                status = SANE_STATUS_NO_MEM;
//                goto cleanup;
//              }
//            }

//            offset += 3 * len;
//            break;

//          case SANE_FRAME_RGB:
//            for (i = 0; i < len; ++i) {
//              image.data[offset + i] = buffer[i];

//              if (!advance(&image)) {
//                status = SANE_STATUS_NO_MEM;
//                goto cleanup;
//              }
//            }

//            offset += len;
//            break;

//          case SANE_FRAME_GRAY:
//            for (i = 0; i < len; ++i) {
//              image.data[offset + i] = buffer[i];

//              if (!advance(&image)) {
//                status = SANE_STATUS_NO_MEM;
//                goto cleanup;
//              }
//            }

//            offset += len;
//            break;

//          default:
//            break;
//        }

//      } else { /* ! must_buffer */
//        if ((parm.depth != 16)) {
//          fwrite(buffer, 1, len, ofp);

//        } else {
//#if !defined(WORDS_BIGENDIAN)
//          int i, start = 0;

//          /* check if we have saved one byte from the last sane_read */
//          if (hang_over > -1) {
//            if (len > 0) {
//              fwrite(buffer, 1, 1, ofp);
//              buffer[0] = (SANE_Byte)hang_over;
//              hang_over = -1;
//              start = 1;
//            }
//          }

//          /* now do the byte-swapping */
//          for (i = start; i < (len - 1); i += 2) {
//            unsigned char LSB;
//            LSB = buffer[i];
//            buffer[i] = buffer[i + 1];
//            buffer[i + 1] = LSB;
//          }

//          /* check if we have an odd number of bytes */
//          if (((len - start) % 2) != 0) {
//            hang_over = buffer[len - 1];
//            len--;
//          }

//#endif
//          fwrite(buffer, 1, len, ofp);
//        }
//      }

//      if (verbose && parm.depth == 8) {
//        for (i = 0; i < len; ++i)
//          if (buffer[i] >= max) {
//            max = buffer[i];

//          } else if (buffer[i] < min) {
//            min = buffer[i];
//          }
//      }
//    }

//    first_frame = 0;
//  } while (!parm.last_frame);

//  if (must_buffer) {
//    image.height = image.y;
//    writePnmHeader(
//      parm.format, parm.pixels_per_line, image.height, parm.depth, ofp);

//#if !defined(WORDS_BIGENDIAN)

//    if (parm.depth == 16) {
//      int i;

//      for (i = 0; i < image.height * image.width; i += 2) {
//        unsigned char LSB;
//        LSB = image.data[i];
//        image.data[i] = image.data[i + 1];
//        image.data[i + 1] = LSB;
//      }
//    }

//#endif
//    fwrite(image.data, 1, image.height * image.width, ofp);
//  }

//  fflush(ofp);

// cleanup:

//  if (image.data) {
//    free(image.data);
//  }

//  return status;
//}

// SANE_Status
// ScanSane::startScan(SANE_Handle sane_handle, SANE_String_Const fileName)
//{
//  SANE_Status sane_status = SANE_STATUS_GOOD;
//  device = sane_handle;
//  // if (sane_status = sane_start(sane_handle))
//  // {
//  //     printf("sane_start status: %s\n", sane_strstatus(sane_status));
//  // }
//  // return sane_status;
//  return doScan(fileName);
//}

// void
// ScanSane::closeDevice(SANE_Handle sane_handle)
//{
//  sane_close(sane_handle);
//}

// SANE_Status
// doScan(const char* fileName)
//{
//  SANE_Status status;
//  //  FILE* ofp = NULL;
//  //  char path[PATH_MAX];
//  //  char part_path[PATH_MAX];
//  //  buffer_size = (32 * 1024);
//  //  buffer = malloc(buffer_size);

//  //  do {
//  //    int dwProcessID = getpid();
//  //    sprintf(path, "%s%d.pnm", fileName, dwProcessID);
//  //    strcpy(part_path, path);
//  //    strcat(part_path, ".part");

//  //    status = sane_start(device);

//  //    if (status != SANE_STATUS_GOOD) {
//  //      break;
//  //    }

//  //    if (NULL == (ofp = fopen(part_path, "w"))) {
//  //      status = SANE_STATUS_ACCESS_DENIED;
//  //      break;
//  //    }

//  //    status = scanIt(ofp);

//  //    switch (status) {
//  //      case SANE_STATUS_GOOD:
//  //      case SANE_STATUS_EOF: {
//  //        status = SANE_STATUS_GOOD;

//  //        if (!ofp || 0 != fclose(ofp)) {
//  //          status = SANE_STATUS_ACCESS_DENIED;
//  //          break;

//  //        } else {
//  //          ofp = NULL;

//  //          if (rename(part_path, path)) {
//  //            status = SANE_STATUS_ACCESS_DENIED;
//  //            break;
//  //          }
//  //        }
//  //      } break;

//  //      default:
//  //        break;
//  //    }
//  //  } while (0);

//  //  if (SANE_STATUS_GOOD != status) {
//  //    sane_cancel(device);
//  //  }

//  //  if (ofp) {
//  //    fclose(ofp);
//  //    ofp = NULL;
//  //  }

//  //  if (buffer) {
//  //    free(buffer);
//  //    buffer = NULL;
//  //  }

//  return status;
//}

#endif
