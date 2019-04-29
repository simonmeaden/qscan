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
#include "saneworker.h"

#include <memory>
#include <stdlib.h>

SaneWorker::SaneWorker(QObject* parent)
  : QObject(parent)
  , m_handle(0)
{
  //  m_logger = Log4Qt::Logger::logger(tr("SaneWorker"));
}

void
SaneWorker::scan(ScanDevice* device)
{
  SANE_Handle sane_handle;
  SANE_Status status;
  SANE_Parameters parameters;
  status = sane_open(device->name.toStdString().c_str(), &sane_handle);
  QImage image;

  if (status != SANE_STATUS_GOOD) {
    emit scanFailed();
    return;
  }

  status = sane_start(sane_handle);

  if (status != SANE_STATUS_GOOD) {
    emit scanFailed();
    return;
  }

  status = sane_get_parameters(sane_handle, &parameters);

  if (status != SANE_STATUS_GOOD) {
    emit scanFailed();
    return;
  }

  int len, offset = 0, full_size = 0;
  bool first_frame = true, must_buffer = true;
  //  SANE_Byte min = 0xff, max = 0;
  SANE_Word total_bytes = 0;
  //  SANE_Word expected_bytes;
  //  SANE_Int hang_over = -1;
  SANE_Int buffer_size = (32 * 1024);
  std::unique_ptr<uchar[]> buffer(new uchar[ulong(buffer_size)]);
  //  SANE_Byte* buffer[buffer_size];
  QImage::Format format = QImage::Format_RGB32;
  bool greyscale = false;
  full_size = parameters.bytes_per_line * parameters.lines;

  if (first_frame) {
    switch (parameters.format) {
      case SANE_FRAME_RED:
      case SANE_FRAME_GREEN:
      case SANE_FRAME_BLUE:
        if (parameters.depth == 8) {
          must_buffer = true;
          offset = parameters.format - SANE_FRAME_RED;
          format = QImage::Format_RGB32;
          full_size *= 3;
          emit log(LogLevel::WARN, tr("Sane frame RED/GREEN/BLUE"));
          break;
        }

        emit log(LogLevel::WARN,
                 tr("Sane frame RED/GREEN/BLUE but depth not 8"));

        emit scanFailed();
        return;

      case SANE_FRAME_RGB:
        if (parameters.depth == 8) {
          format = QImage::Format_RGB32;
          emit log(LogLevel::WARN, tr("Sane frame RGB depth 8"));
          break;

        } else if (parameters.depth == 16) {
          format = QImage::QImage::Format_RGBX64;
          emit log(LogLevel::WARN, tr("Sane frame RGB depth 16"));
          break;
        }

        //        m_logger->warn(tr("Sane frame RGB but depth 1"));
        emit scanFailed();
        return;

      case SANE_FRAME_GRAY:
        if (parameters.depth == 1) {
          format = QImage::Format_Mono;

          if (parameters.lines < 0) {
            must_buffer = true;
            offset = 0;
          }

          emit log(LogLevel::WARN, tr("Sane frame Mono depth 1"));
          break;

        } else if (parameters.depth == 8) {
          format = QImage::Format_Grayscale8;
          greyscale = true;

          if (parameters.lines < 0) {
            must_buffer = true;
            offset = 0;
          }

          emit log(LogLevel::WARN, tr("Sane frame Grey depth 8"));
          break;

        } else if (parameters.depth == 16) {
          // QImage doesn't have a 16 bit greyscale
          // so use RGB and set all three channels
          // to the same value.
          format = QImage::Format_RGBX64;
          greyscale = true;

          if (parameters.lines < 0) {
            must_buffer = true;
            offset = 0;
          }

          emit log(LogLevel::WARN, tr("Sane frame Grey depth 16"));
          break;

        } else {
          emit scanFailed();
          return;
        }
    }

    image = QImage(parameters.pixels_per_line, parameters.lines, format);

  } else {
    // TODO not first frame ?
  }

  int progress;
  int x = 0, y = 0;
  quint32 value;

  while (true) {
    status = sane_read(sane_handle, buffer.get(), buffer_size, &len);

    if (status == SANE_STATUS_GOOD) {
      if (len > 0) {
        total_bytes += SANE_Word(len);
        progress = int((total_bytes * 100.0) / double(full_size));
        emit scanProgress(progress);

        if (parameters.format == SANE_FRAME_RGB) {
          if (format == QImage::Format_RGB32) {
            for (int i = 0; i < len; i += 3) {
              if (x > 0 && ((x + 1) % parameters.pixels_per_line) == 0) {
                x = 0;
                y++;

              } else {
                x++;
              }

              value = 0xff000000;
              quint32 c1 = buffer[ulong(i)];
              c1 = c1 << 16;
              quint32 c2 = buffer[ulong(i + 1)];
              c2 = c2 << 8;
              quint8 c3 = buffer[ulong(i + 2)];
              value += c1 + c2 + c3;
              image.setPixel(x, y, value);
            }

          } else if (format == QImage::Format_RGBX64) {
          }
        } else {
          // SANE_FRAME_RED,GREEN & BLUE
          // TODO - Multi frame scans.
        }
      }
    }

    if (total_bytes == full_size) {
      break;
    }
  }

  emit log(LogLevel::INFO, tr("Scan has completed"));

  sane_close(sane_handle);

  emit scanCompleted(image);
  //  emit finished();
  //  emit scanFailed();
  //  return;
}

void
SaneWorker::setResolution(ScanDevice* device,
                          const SANE_Option_Descriptor* option,
                          SANE_Int option_id)
{
  switch (option->unit) {
    case SANE_UNIT_MM:
      device->options->setUnits(ScanUnits::MM);
      break;
    case SANE_UNIT_DPI:
      device->options->setUnits(ScanUnits::DPI);
      break;
  }
  if (option->constraint_type == SANE_CONSTRAINT_RANGE) {
    switch (option->type) {
      case SANE_TYPE_INT:
        device->options->setMinResolution(option->constraint.range->min);
        device->options->setMaxResolution(option->constraint.range->max);
        break;
      case SANE_TYPE_FIXED:
        device->options->setMinResolution(
          SANE_UNFIX(option->constraint.range->min));
        device->options->setMaxResolution(
          SANE_UNFIX(option->constraint.range->max));
        break;
      default:
        break;
    }
  }
  int value = -1;
  SANE_Status status = sane_control_option(
    device->sane_handle, option_id, SANE_ACTION_GET_VALUE, &value, nullptr);

  if (status == SANE_STATUS_GOOD) {
    switch (option->type) {
      case SANE_TYPE_INT:
        device->options->setResolution(value);
        break;
      case SANE_TYPE_FIXED:
        device->options->setResolution(SANE_UNFIX(value));
        break;
      default:
        break;
    }
  }
}

void
SaneWorker::loadAvailableScannerOptions(ScanDevice* device)
{
  QMutexLocker locker(&m_mutex);
  SANE_Handle sane_handle;
  SANE_Status status;
  SANE_Int number_of_options = 0;
  SANE_Int option_id = 1;
  SANE_Int info;
  QStringList list;

  status = sane_open(device->name.toStdString().c_str(), &sane_handle);
  device->sane_handle = sane_handle;

  if (status != SANE_STATUS_GOOD) {
    emit scanFailed();
    return;
  }

  status = sane_control_option(
    sane_handle, 0, SANE_ACTION_GET_VALUE, &number_of_options, &info);

  if (status == SANE_STATUS_GOOD) {
    for (SANE_Int i = 1; i < number_of_options; ++option_id) {
      if (const auto option =
            sane_get_option_descriptor(sane_handle, option_id)) {
        QString name(option->name);
        QString title(option->title);
        if (!name.isEmpty()) {
          device->options->setOptionId(name, option_id);
          device->options->setOptionSize(name, option->size);
          device->options->setOptionType(name, option->type);
        }

        emit log(LogLevel::INFO, tr("Name : %1, Title : %2").arg(name, title));
        QStringList list;
        if (name == SANE_NAME_SCAN_TL_X || name == SANE_NAME_SCAN_TL_Y ||
            name == SANE_NAME_SCAN_BR_X || name == SANE_NAME_SCAN_BR_Y ||
            name == SANE_NAME_CONTRAST || name == SANE_NAME_BRIGHTNESS) {
          getIntValue(device, option_id, name);

        } else if (name == SANE_NAME_SCAN_RESOLUTION) {
          setResolution(device, option, option_id);

        } else if (name == SANE_NAME_SCAN_SOURCE) {
          switch (option->type) {
            case SANE_TYPE_STRING: {
              QString s;

              switch (option->constraint_type) {
                case SANE_CONSTRAINT_STRING_LIST:
                  for (int i = 0; option->constraint.string_list[i] != nullptr;
                       i++) {
                    s = QString(option->constraint.string_list[i]);
                    list.append(s);
                  }

                  break;

                case SANE_CONSTRAINT_WORD_LIST:
                  for (int i = 1; i <= option->constraint.word_list[0]; i++) {
                    s = QString(option->constraint.word_list[i]);
                    list.append(s);
                  }

                  break;

                default:
                  break;
              }
            }

            default:
              break;
          }
          if (!list.isEmpty()) {
            device->options->setSources(list);
          }
          // get the current source
          if (status == SANE_STATUS_GOOD) {
            QString str_value =
              getOptionValue(device, SANE_NAME_SCAN_SOURCE).toString();
            device->options->setSource(str_value);
          }
        } else if (name == SANE_NAME_SCAN_MODE) {

          switch (option->type) {
            case SANE_TYPE_STRING: {
              QString s;

              switch (option->constraint_type) {
                case SANE_CONSTRAINT_STRING_LIST:
                  for (int i = 0; option->constraint.string_list[i] != nullptr;
                       i++) {
                    s = QString(option->constraint.string_list[i]);
                    list.append(s);
                  }

                  break;

                case SANE_CONSTRAINT_WORD_LIST:
                  for (int i = 1; i <= option->constraint.word_list[0]; i++) {
                    s = QString(option->constraint.word_list[i]);
                    list.append(s);
                  }

                  break;

                default:
                  break;
              }
            }

            default:
              break;
          }
          if (!list.isEmpty()) {
            device->options->setModes(list);
          }
          // get the current mode
          if (status == SANE_STATUS_GOOD) {
            QString out_str =
              getOptionValue(device, SANE_NAME_SCAN_MODE).toString();
            device->options->setMode(out_str);
          }
        }

        ++i;
      }
    }
    emit optionsSet(device);
  }

  sane_close(sane_handle);
}

void
SaneWorker::setBoolValue(ScanDevice* device,
                         int option_id,
                         const QString& /*name*/,
                         bool value)
{
  SANE_Handle sane_handle;
  SANE_Status status;
  SANE_Int info;
  status = sane_open(device->name.toStdString().c_str(), &sane_handle);

  if (status == SANE_STATUS_GOOD) {
    if (option_id > 0) {
      status = sane_control_option(
        sane_handle, option_id, SANE_ACTION_SET_VALUE, &value, &info);
      if (status == SANE_STATUS_GOOD) {
        if ((info & ~(SANE_INFO_RELOAD_OPTIONS | SANE_INFO_RELOAD_PARAMS)) ==
            0) {
          sane_close(sane_handle);
          return;
        }
        // TODO set whichever bool values need setting.
      }
    }
  }

  sane_close(sane_handle);
}

void
SaneWorker::setIntValue(ScanDevice* device,
                        int option_id,
                        const QString& name,
                        int value)
{
  SANE_Handle sane_handle;
  SANE_Status status;
  SANE_Int info;
  status = sane_open(device->name.toStdString().c_str(), &sane_handle);

  if (status == SANE_STATUS_GOOD) {

    if (option_id > 0) {
      status = sane_control_option(
        sane_handle, option_id, SANE_ACTION_SET_VALUE, &value, &info);
      if (status == SANE_STATUS_GOOD) {
        if (name == SANE_NAME_SCAN_TL_X) {
          device->options->setTopLeftX(value);
        } else if (name == SANE_NAME_SCAN_TL_Y) {
          device->options->setTopLeftY(value);
        } else if (name == SANE_NAME_SCAN_BR_X) {
          device->options->setBottomRightX(value);
        } else if (name == SANE_NAME_SCAN_BR_Y) {
          device->options->setBottomRightY(value);
        } else if (name == SANE_NAME_SCAN_RESOLUTION) {
          device->options->setResolution(value);
        } else if (name == SANE_NAME_SCAN_X_RESOLUTION) {
          device->options->setResolutionX(value);
        } else if (name == SANE_NAME_SCAN_Y_RESOLUTION) {
          device->options->setResolutionY(value);
        } else if (name == SANE_NAME_CONTRAST) {
          device->options->setContrast(value);
        } else if (name == SANE_NAME_BRIGHTNESS) {
          device->options->setBrightness(value);
        }
        if ((info & ~(SANE_INFO_RELOAD_OPTIONS | SANE_INFO_RELOAD_PARAMS)) ==
            0) {
          sane_close(sane_handle);
          return;
        }
      }
    }
  }

  sane_close(sane_handle);
}

void
SaneWorker::setStringValue(ScanDevice* device,
                           const QString& name,
                           const QString& value)
{
  SANE_Handle sane_handle;
  SANE_Status status;
  SANE_Int info;

  status = sane_open(device->name.toStdString().c_str(), &sane_handle);
  device->sane_handle = sane_handle;

  if (status == SANE_STATUS_GOOD) {
    char* c_str = value.toLatin1().data();
    int option_id = device->options->optionId(name);

    if (option_id > -1) {
      status = sane_control_option(
        sane_handle, option_id, SANE_ACTION_SET_VALUE, c_str, &info);
      if (status == SANE_STATUS_GOOD) {
        if ((info & ~(SANE_INFO_RELOAD_OPTIONS | SANE_INFO_RELOAD_PARAMS)) ==
            0) {
          QString str_value = getOptionValue(device, name).toString();
          if (str_value == value) {
            if (name == SANE_NAME_SCAN_SOURCE) {
              device->options->setSource(value);
              emit sourceChanged(device);
            } else if (name == SANE_NAME_SCAN_MODE) {
              device->options->setMode(value);
              emit modeChanged(device);
            }
          }
        }
      } else {
        emit log(LogLevel::FATAL,
                 tr("Unable to set value %1").arg(sane_strstatus(status)));
      }
    }
  }

  sane_close(sane_handle);
}

void
SaneWorker::getIntValue(ScanDevice* device, int option_id, const QString& name)
{
  SANE_Int v;
  SANE_Status status;

  if (option_id > 0) { // 0 is a special case already dealt with
    status = sane_control_option(
      device->sane_handle, option_id, SANE_ACTION_GET_VALUE, &v, nullptr);

    if (status == SANE_STATUS_GOOD) {
      //        value = v;
      //        emit sendIntValue(device, v);
      if (name == SANE_NAME_SCAN_TL_X) {
        device->options->setTopLeftX(v);
      } else if (name == SANE_NAME_SCAN_TL_Y) {
        device->options->setTopLeftY(v);
      } else if (name == SANE_NAME_SCAN_BR_X) {
        device->options->setBottomRightX(v);
      } else if (name == SANE_NAME_SCAN_BR_Y) {
        device->options->setBottomRightY(v);
      } else if (name == SANE_NAME_SCAN_RESOLUTION) {
        device->options->setResolution(v);
      } else if (name == SANE_NAME_SCAN_X_RESOLUTION) {
        device->options->setResolutionX(v);
      } else if (name == SANE_NAME_SCAN_Y_RESOLUTION) {
        device->options->setResolutionY(v);
      } else if (name == SANE_NAME_CONTRAST) {
        device->options->setContrast(v);
      } else if (name == SANE_NAME_BRIGHTNESS) {
        device->options->setBrightness(v);
      }
    }
  }
}

void
SaneWorker::getListValue(ScanDevice* device,
                         int option_id,
                         const QString& name,
                         const SANE_Option_Descriptor* opt)
{

  if (option_id > 0) { // 0 is a special case already dealt with
    SANE_Word value;
    SANE_Status status;

    status = sane_control_option(
      device->sane_handle, option_id, SANE_ACTION_GET_VALUE, &value, nullptr);

    if (status == SANE_STATUS_GOOD) {
      QStringList list;

      switch (opt->type) {
        case SANE_TYPE_STRING: {
          QString s;

          switch (opt->constraint_type) {
            case SANE_CONSTRAINT_STRING_LIST:
              for (int i = 0; opt->constraint.string_list[i] != nullptr; i++) {
                s = QString(opt->constraint.string_list[i]);
                list.append(s);
              }

              break;

            case SANE_CONSTRAINT_WORD_LIST:
              for (int i = 1; i <= opt->constraint.word_list[0]; i++) {
                s = QString(opt->constraint.word_list[i]);
                list.append(s);
              }

              break;

            default:
              break;
          }
        }

        default:
          break;
      }
      if (!list.isEmpty()) {
        if (name == SANE_NAME_SCAN_SOURCE) {
          device->options->setSources(list);
        } else if (name == SANE_NAME_SCAN_MODE) {
          device->options->setModes(list);
        }
      }
    }
  }
}

void
SaneWorker::cancelScan()
{
  sane_cancel(m_handle);
}

/* Allocate the requested memory plus enough room to store some guard bytes. */
void*
SaneWorker::guardedMalloc(size_t size)
{
  unsigned char* ptr;

  size += 2 * GUARDS_SIZE;
  ptr = reinterpret_cast<unsigned char*>(malloc(size));

  //  assert(ptr);

  ptr += GUARDS_SIZE;

  return (ptr);
}

/* Free some memory allocated by guards_malloc. */
void
SaneWorker::guardedFree(void* ptr)
{
  auto* p = reinterpret_cast<unsigned char*>(ptr);

  p -= GUARDS_SIZE;
  free(p);
}

///* Get an option descriptor by the name of the option. */
// const SANE_Option_Descriptor*
// SaneWorker::getOptionDescriptorByName(SANE_Handle device,
//                                      const char* name,
//                                      int* option_num)
//{
//  const SANE_Option_Descriptor* opt;
//  SANE_Int num_dev_options;
//  SANE_Status status;

//  /* Get the number of options. */
//  status = sane_control_option(
//    device, 0, SANE_ACTION_GET_VALUE, &num_dev_options, nullptr);
//  if (status != SANE_STATUS_GOOD) {
//    emit log(LogLevel::FATAL,
//             tr("cannot get option 0 value
//             (%1)").arg(sane_strstatus(status)));
//  }

//  for (*option_num = 0; *option_num < num_dev_options; (*option_num)++) {

//    /* Get the option descriptor */
//    opt = sane_get_option_descriptor(device, *option_num);
//    if (status != SANE_STATUS_GOOD) {
//      emit log(
//        LogLevel::FATAL,
//        tr("cannot get option descriptor for option %1").arg(*option_num));
//    }

//    if (opt->name && strcmp(opt->name, name) == 0) {
//      return (opt);
//    }
//  }
//  return (nullptr);
//}

/* Returns a string with the value of an option. */
QVariant
SaneWorker::getOptionValue(ScanDevice* device, const QString& option_name)
{
  const SANE_Option_Descriptor* opt;
  void* optval; /* value for the option */
  int option_id = device->options->optionId(option_name);
  int option_type = device->options->optionType(option_name);
  int option_size = device->options->optionSize(option_name);
  char str[100];
  SANE_Status status;
  QVariant v;

  //  opt = getOptionDescriptorByName(device->sane_handle, option_name,
  //  &option_id); if (opt) {

  optval = guardedMalloc(option_size);
  status = sane_control_option(
    device->sane_handle, option_id, SANE_ACTION_GET_VALUE, optval, nullptr);

  if (status == SANE_STATUS_GOOD) {
    switch (option_type) {

      case SANE_TYPE_BOOL:
        if (reinterpret_cast<SANE_Word*>(optval) == SANE_FALSE) {
          v = false;
        } else {
          v = true;
        }
        break;

      case SANE_TYPE_INT:
        v = *reinterpret_cast<SANE_Int*>(optval);
        break;

      case SANE_TYPE_FIXED: {
        int i;
        v = SANE_UNFIX(*reinterpret_cast<SANE_Word*>(optval));
      } break;

      case SANE_TYPE_STRING:
        v = QString(reinterpret_cast<const char*>(optval));
        break;
    }
  } else {
    /* Shouldn't happen. */
    //    strcpy(str, "backend default");
  }

  guardedFree(optval);

  return v;
}

///* Set a random value for an option amongst the possible values. */
// void
// SaneWorker::set_random_value(SANE_Handle device,
//                             int option_num,
//                             const SANE_Option_Descriptor* opt)
//{
//  SANE_Status status;
//  SANE_String val_string;
//  SANE_Int val_int;
//  int i;
//  int rc;

//  if (status != SANE_STATUS_GOOD){
//    emit log(LogLevel::DEBUG,
//             tr("option is not settable"));
//  }

//  switch(opt->constraint_type) {
//    case SANE_CONSTRAINT_WORD_LIST:
//      if (status != SANE_STATUS_GOOD){
//        emit log(LogLevel::DEBUG,
//                 tr("no value in the list for option
//                 %1").arg(sane_strstatus(status)));
//        return;
//      }

//      i=1+(rand() % opt->constraint.word_list[0]);
//      val_int = opt->constraint.word_list[i];
//      status = sane_control_option (device, option_num,
//                                   SANE_ACTION_SET_VALUE, &val_int, NULL);
//      check(ERR, (status == SANE_STATUS_GOOD),
//            "cannot set option %s to %d (%s)", opt->name, val_int,
//            sane_strstatus(status));
//      break;

//    case SANE_CONSTRAINT_STRING_LIST:
//      rc = check(ERR, (opt->constraint.string_list[0] != NULL),
//                 "no value in the list for option %s", opt->name);
//      if (!rc) return;
//      for (i=0; opt->constraint.string_list[i] != NULL; i++);
//      i = rand() % i;
//      val_string = strdup(opt->constraint.string_list[0]);
//      assert(val_string);
//      status = sane_control_option (device, option_num,
//                                   SANE_ACTION_SET_VALUE, val_string, NULL);
//      check(ERR, (status == SANE_STATUS_GOOD),
//            "cannot set option %s to [%s] (%s)", opt->name, val_string,
//            sane_strstatus(status));
//      free(val_string);
//      break;

//    case SANE_CONSTRAINT_RANGE:
//      i = opt->constraint.range->max - opt->constraint.range->min;
//      i = rand() % i;
//      val_int = opt->constraint.range->min + i;
//      status = sane_control_option (device, option_num,
//                                   SANE_ACTION_SET_VALUE, &val_int, NULL);
//      check(ERR, (status == SANE_STATUS_GOOD),
//            "cannot set option %s to %d (%s)", opt->name, val_int,
//            sane_strstatus(status));
//      break;

//    default:
//      abort();
//  }
//}
