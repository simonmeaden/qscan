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
#include "saneworker.h"

#include <memory>
#include <stdlib.h>

#include "logging.h"
#include "scandevice.h"

namespace QScanner {

SaneWorker::SaneWorker(QObject* parent)
  : QThread(parent)
  , m_running(true)
{
}

void SaneWorker::run()
{
  while (m_running) {
    if (!m_tasks.isEmpty()) {
      Task* task = m_tasks.dequeue();

      switch (task->task) {
      case SET_BOOL:
        break;

      case SET_STRING:
        break;

      case DEVICE_INT:
        do_setDeviceIntValue(task);
        break;

      case DEVICE_STRING:
        do_setDeviceStringValue(task);
        break;

      case SCAN:
        do_scan(task);
        break;

      case CANCEL_SCAN:
        break;

      case LOAD_OPTIONS:
        do_loadAvailableScannerOptions(task);
        break;
      }

    } else {
      this->msleep(200);
    }
  }
}
void SaneWorker::scan(ScanDevice* device)
{
  Task* task = new Task();
  task->device = device;
  m_tasks.enqueue(task);
}

void SaneWorker::do_scan(Task* task)
{
  SANE_Status status;
  SANE_Parameters parameters;
  status = sane_open(task->device->descriptor().toStdString().c_str(), &m_sane_handle);
  QImage image;

  if (status != SANE_STATUS_GOOD) {
    emit scanFailed();
    return;
  }

  status = sane_start(m_sane_handle);

  if (status != SANE_STATUS_GOOD) {
    emit scanFailed();
    return;
  }

  status = sane_get_parameters(m_sane_handle, &parameters);

  if (status != SANE_STATUS_GOOD) {
    emit scanFailed();
    return;
  }

  int len;
  int full_size = 0;
  bool first_frame = true;
  //  SANE_Byte min = 0xff, max = 0;
  SANE_Word total_bytes = 0;
  //  SANE_Word expected_bytes;
  //  SANE_Int hang_over = -1;
  SANE_Int buffer_size = (32 * 1024);
  std::unique_ptr<uchar[]> buffer(new uchar[ulong(buffer_size)]);
  //  SANE_Byte* buffer[buffer_size];
  QImage::Format format = QImage::Format_RGB32;
  full_size = parameters.bytes_per_line * parameters.lines;

  if (first_frame) {
    //    int offset = 0;
    //    bool must_buffer = true;
    //    bool greyscale = false;

    switch (parameters.format) {
    case SANE_FRAME_RED:
    case SANE_FRAME_GREEN:
    case SANE_FRAME_BLUE:
      if (parameters.depth == 8) {
        //        must_buffer = true;
        //        offset = parameters.format - SANE_FRAME_RED;
        format = QImage::Format_RGB32;
        full_size *= 3;
        qCWarning(LogQScan) << tr("Sane frame RED/GREEN/BLUE");
        break;
      }

      qCWarning(LogQScan) << tr("Sane frame RED/GREEN/BLUE but depth not 8");

      emit scanFailed();
      return;

    case SANE_FRAME_RGB:
      if (parameters.depth == 8) {
        format = QImage::Format_RGB32;
        qCWarning(LogQScan) << tr("Sane frame RGB depth 8");
        break;

      } else if (parameters.depth == 16) {
        format = QImage::QImage::Format_RGBX64;
        qCWarning(LogQScan) << tr("Sane frame RGB depth 16");
        break;
      }

      emit scanFailed();
      return;

    case SANE_FRAME_GRAY:
      if (parameters.depth == 1) {
        format = QImage::Format_Grayscale8;

        //          if (parameters.lines < 0) {
        //            must_buffer = true;
        //            offset = 0;
        //          }

        qCWarning(LogQScan) << tr("Sane frame Mono depth 1");
        break;

      } else if (parameters.depth == 8) {
        format = QImage::Format_Grayscale8;

        //        greyscale = true;

        if (parameters.lines < 0) {
          //          must_buffer = true;
          //          offset = 0;
        }

        qCWarning(LogQScan) << tr("Sane frame Grey depth 8");
        break;

      } else if (parameters.depth == 16) {
        // QImage doesn't have a 16 bit greyscale
        // so use RGB and set all three channels
        // to the same value.
        format = QImage::Format_RGBX64;

        //        greyscale = true;

        if (parameters.lines < 0) {
          //          must_buffer = true;
          //          offset = 0;
        }

        qCWarning(LogQScan) << tr("Sane frame Grey depth 16");
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
    status = sane_read(m_sane_handle, buffer.get(), buffer_size, &len);

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

  qCInfo(LogQScan) << tr("Scan has completed");

  sane_close(m_sane_handle);
  m_sane_handle = nullptr;

  emit scanCompleted(image, task->device->options()->resolution());

  //  emit finished();
  //  emit scanFailed();
  //  return;
}

//void SaneWorker::storeResolution(SANE_Handle sane_handle, ScanDevice* device,
//                                 const SANE_Option_Descriptor* descriptor, SANE_Int option_id)
//{

//  device->options()->setOptionValue(ScanOptions::QSCAN_UNITS, ScanOptions::getScanUnits(descriptor->unit));

//  if (descriptor->constraint_type == SANE_CONSTRAINT_RANGE) {
//    RangeData range;

//    switch (descriptor->type) {
//    case SANE_TYPE_INT:
//      range.min = descriptor->constraint.range->min;
//      range.max = descriptor->constraint.range->max;

//      break;

//    case SANE_TYPE_FIXED:
//      range.min = SANE_FIX(descriptor->constraint.range->min);
//      range.max = SANE_FIX(descriptor->constraint.range->max);
//      break;

//    default:
//      break;
//    }

//    device->options()->setResulutionRange(range);

//  } else if (descriptor->constraint_type == SANE_CONSTRAINT_WORD_LIST) {
//    QList<int> list;

//    switch (descriptor->type) {
//    case SANE_TYPE_INT: {
//      for (int i = 1; i < descriptor->constraint.word_list[0]; i++) {
//        list.append(descriptor->constraint.word_list[i]);
//      }

//      break;
//    }

//    case SANE_TYPE_FIXED: {
//      for (int i = 1; i < descriptor->constraint.word_list[0]; i++) {
//        list.append(SANE_FIX(descriptor->constraint.word_list[i]));
//      }

//      break;
//    }

//    default:
//      break;
//    }

//    device->options()->setResulutionRange(list);
//  }


//  int value = -1;
//  SANE_Status status = sane_control_option(sane_handle, option_id, SANE_ACTION_GET_VALUE, &value, nullptr);

//  if (status == SANE_STATUS_GOOD) {
//    switch (descriptor->type) {
//    case SANE_TYPE_INT:
//      device->options()->setOptionValue(ScanOptions::QSCAN_RESOLUTION, value);
//      break;

//    case SANE_TYPE_FIXED:
//      device->options()->setOptionValue(ScanOptions::QSCAN_RESOLUTION, SANE_UNFIX(value));
//      break;

//    default:
//      break;
//    }
//  }
//}

//void SaneWorker::storeStringOption(SANE_Handle sane_handle, ScanDevice* device, const QString& descriptor,
//                                   const SANE_Option_Descriptor* option, SANE_Int)
//{
//  QStringList list;

//  switch (option->type) {
//  case SANE_TYPE_STRING: {
//    QString s;

//    switch (option->constraint_type) {
//    case SANE_CONSTRAINT_STRING_LIST:
//      for (int i = 0; option->constraint.string_list[i] != nullptr; i++) {
//        s = QString(option->constraint.string_list[i]);
//        list.append(s);
//      }

//      break;

//    case SANE_CONSTRAINT_WORD_LIST:
//      for (int i = 1; i <= option->constraint.word_list[0]; i++) {
//        s = QString(option->constraint.word_list[i]);
//        list.append(s);
//      }

//      break;

//    default:
//      break;
//    }

//    break;
//  }

//  default:
//    break;
//  }

//  ScanOptions::AvailableOptions available_op = ScanOptions::getAvailableOption(descriptor);
//  ScanOptions::AvailableOptions matching_list_op = ScanOptions::getMatchingListOption(available_op);
//  QString str_value = getDeviceValue(sane_handle, device, descriptor).toString();

//  if (matching_list_op != ScanOptions::QSCAN_OPTION_UNAVAILABLE && !list.isEmpty()) {
//    device->options()->setOptionValue(matching_list_op, list);
//  }

//  device->options()->setOptionValue(available_op, str_value);
//}

void SaneWorker::loadAvailableScannerOptions(ScanDevice* device)
{
  if (!device->areAvailableOptionsSet()) {
    Task* task = new Task();
    task->task = LOAD_OPTIONS;
    task->device = device;
    m_tasks.enqueue(task);
  }
}

void SaneWorker::do_loadAvailableScannerOptions(Task* task)
{
  QMutexLocker locker(&m_mutex);
  SANE_Handle sane_handle;
  SANE_Status status;
  SANE_Int number_of_options = 0;
  SANE_Int option_id = 1;
  SANE_Int info;
  ScanDevice* device = task->device;

  if (!device || device->descriptor().isEmpty()) {
    return;
  }

  status = sane_open(device->descriptor().toStdString().c_str(), &sane_handle);
  //  device->setHandle(sane_handle);

  if (status != SANE_STATUS_GOOD) {
    emit scanOpenFailed(device->descriptor());
    return;
  }

  // option id == 0 returns the number of optionas for this scanner.
  status = sane_control_option(sane_handle, 0, SANE_ACTION_GET_VALUE, &number_of_options, &info);

  if (status == SANE_STATUS_GOOD) {
    for (SANE_Int i = 1; i < number_of_options; ++option_id) {
      if (const auto descriptor = sane_get_option_descriptor(sane_handle, option_id)) {
        qCDebug(LogQScan) << tr("Count = %1").arg(i);

        QString name(descriptor->name);
        QString title(descriptor->title);
        QString description(descriptor->desc);

        qCInfo(LogQScan) << tr("Name : %1, Title : %2").arg(name, title);

        if (!name.isEmpty()) {
          device->options()->setOptionId(ScanOptions::getAvailableOption(name), option_id);
          device->options()->setOptionSize(ScanOptions::getAvailableOption(name), size_t(descriptor->size));
          device->options()->setOptionType(ScanOptions::getAvailableOption(name), descriptor->type);

        } else {
          i++;
          continue;
        }

        storeOptionData(sane_handle, device, option_id, descriptor);

        ++i;
      }
    }

    emit optionsSet();
    device->setAvailableOptionsSet(true);
  }

  sane_close(sane_handle);
}

void SaneWorker::setDeviceBoolValue(ScanDevice* device, int option_id, const QString& descriptor, bool value)
{
  Task* task = new Task();
  task->task = SET_BOOL;
  task->device = device;
  task->option_id = option_id;
  task->descriptor = descriptor;
  task->value = value;
  m_tasks.enqueue(task);
}

void SaneWorker::do_setDeviceBoolValue(Task* task)
{
  QMutexLocker locker(&m_mutex);
  SANE_Handle sane_handle;
  SANE_Status status;
  SANE_Int info;
  ScanDevice device = task->device;

  status = sane_open(device.descriptor().toStdString().c_str(), &sane_handle);

  if (status == SANE_STATUS_GOOD) {
    if (task->option_id > 0) {
      status = sane_control_option(sane_handle, task->option_id, SANE_ACTION_SET_VALUE, &task->value, &info);

      if (status == SANE_STATUS_GOOD) {
        if ((info & ~(SANE_INFO_RELOAD_OPTIONS | SANE_INFO_RELOAD_PARAMS)) == 0) {
          sane_close(sane_handle);
          return;
        }

        // TODO set whichever bool values need setting.
      }
    }
  }

  sane_close(sane_handle);
}

void SaneWorker::setDeviceIntValue(ScanDevice* device, int option_id, const QString& descriptor, int value)
{
  Task* task = new Task();
  task->task = SET_BOOL;
  task->device = device;
  task->option_id = option_id;
  task->descriptor = descriptor;
  task->value = value;
  m_tasks.enqueue(task);
}

void SaneWorker::do_setDeviceIntValue(Task* task)
{
  QMutexLocker locker(&m_mutex);
  SANE_Handle sane_handle;
  SANE_Status status;
  SANE_Int info;
  ScanDevice* device = task->device;
  int option_id = task->option_id;
  int value = task->value.toInt();
  QString descriptor = task->descriptor;

  status = sane_open(device->descriptor().toStdString().c_str(), &sane_handle);

  if (status == SANE_STATUS_GOOD) {
    if (option_id > 0) {
      status = sane_control_option(sane_handle, option_id, SANE_ACTION_SET_VALUE, &value, &info);

      if (status == SANE_STATUS_GOOD) {
        // check that the value has actually been set by recovering new value.
        int recovered_val = -1;
        status = sane_control_option(sane_handle, option_id, SANE_ACTION_GET_VALUE, &recovered_val, &info);

        if (status == SANE_STATUS_GOOD) {
          if (recovered_val == value) {
            device->options()->setOptionValue(ScanOptions::getAvailableOption(descriptor), value);
            //            if (descriptor == SANE_NAME_SCAN_TL_X) {
            //              device->options()->setTopLeftX(value);

            //            } else if (descriptor == SANE_NAME_SCAN_TL_Y) {
            //              device->options()->setTopLeftY(value);

            //            } else if (descriptor == SANE_NAME_SCAN_BR_X) {
            //              device->options()->setBottomRightX(value);

            //            } else if (descriptor == SANE_NAME_SCAN_BR_Y) {
            //              device->options()->setBottomRightY(value);

            //            } else if (descriptor == SANE_NAME_SCAN_RESOLUTION) {
            //              device->options()->setResolution(value);

            //            } else if (descriptor == SANE_NAME_SCAN_X_RESOLUTION) {
            //              device->options()->setResolutionX(value);

            //            } else if (descriptor == SANE_NAME_SCAN_Y_RESOLUTION) {
            //              device->options()->setResolutionY(value);

            //            } else if (descriptor == SANE_NAME_CONTRAST) {
            //              device->options()->setContrast(value);

            //            } else if (descriptor == SANE_NAME_BRIGHTNESS) {
            //              device->options()->setBrightness(value);
            //            }

            // only sent when change is confirmed.
            emit optionChanged(descriptor, task->value);
          }
        }

        if ((info & ~(SANE_INFO_RELOAD_OPTIONS | SANE_INFO_RELOAD_PARAMS)) == 0) {
          sane_close(sane_handle);
          return;
        }
      }
    }
  }

  sane_close(sane_handle);
}

void SaneWorker::setDeviceStringValue(ScanDevice* device, const QString& descriptor, const QString& value)
{
  Task* task = new Task();
  task->task = SET_BOOL;
  task->device = device;
  //  task->option_id = option_id;
  task->descriptor = descriptor;
  task->value = value;
  m_tasks.enqueue(task);
}

void SaneWorker::do_setDeviceStringValue(Task* task)
{
  SANE_Handle sane_handle;
  SANE_Status status;
  SANE_Int info;
  ScanDevice* device = task->device;
  QString value = task->value.toString();
  QString descriptor = task->descriptor;

  status = sane_open(device->descriptor().toStdString().c_str(), &sane_handle);
  //  device->setHandle(sane_handle);

  if (status == SANE_STATUS_GOOD) {
    char* c_str = value.toLatin1().data();
    int option_id = device->options()->optionId(ScanOptions::getAvailableOption(descriptor));

    if (option_id > -1) {
      status = sane_control_option(sane_handle, option_id, SANE_ACTION_SET_VALUE, c_str, &info);

      if (status == SANE_STATUS_GOOD) {
        if ((info & ~(SANE_INFO_RELOAD_OPTIONS | SANE_INFO_RELOAD_PARAMS)) == 0) {
          QVariant var = getDeviceValue(sane_handle, device, descriptor);

          if (var.isValid()) {
            QString recovered_val = var.toString();

            if (recovered_val == value) {
              device->options()->setOptionValue(ScanOptions::getAvailableOption(descriptor), value);

              // only sent when change is confirmed.
              emit optionChanged(descriptor, value);
            }
          }
        }

      } else {
        qCritical() << tr("Unable to set value %1").arg(sane_strstatus(status));
      }
    }
  }

  sane_close(sane_handle);
}

void SaneWorker::storeOptionData(SANE_Handle sane_handle,
                                 ScanDevice* device,
                                 const int option_id,
                                 const SANE_Option_Descriptor* descriptor)
{
  QVariant value = handleDescriptorType(sane_handle, device,
                                        option_id, descriptor);
  QVariant constraint = handleConstraintType(descriptor);

  ScanOptions::AvailableOptions operation = ScanOptions::QSCAN_OPTION_UNAVAILABLE;
  //  ScanOptions::AvailableOptions match_op;
  ScanOptions* options = device->options();

  QString name(descriptor->name);
  QString title(descriptor->title);
  QString description(descriptor->desc);

  options->setOptionName(operation, name);
  options->setOptionTitle(operation, title);
  options->setOptionDesc(operation, description);

  if (value.isValid() &&
      (operation = ScanOptions::getAvailableOption(descriptor->name)) != ScanOptions::QSCAN_OPTION_UNAVAILABLE) {
    options->setOptionValue(operation, value);

  } else {
    options->setOptionValue(descriptor->name, value);
  }

  if (constraint.isValid() && operation != ScanOptions::QSCAN_OPTION_UNAVAILABLE) {
    if (constraint.canConvert<QVariantList>()) {
      options->setOptionValue(operation, constraint);

    } else if (constraint.canConvert<RangeData>()) {
      options->setOptionValue(operation, constraint);
    }

  }

  ScanOptions::ScanUnits units = ScanOptions::getScanUnits(descriptor->unit);

  if (units != ScanOptions::NONE) {
    if (value.isValid() &&
        (operation = ScanOptions::getAvailableOption(descriptor->name)) != ScanOptions::QSCAN_OPTION_UNAVAILABLE) {

      options->setUnits(operation, units);

    } else {
      options->setUnits(descriptor->name, units);
    }
  }

}

QVariant SaneWorker::handleDescriptorType(SANE_Handle sane_handle,
    ScanDevice* device,
    const int option_id,
    const SANE_Option_Descriptor* descriptor)
{
  SANE_Status status;
  void* value = nullptr;
  QVariant var;

  size_t option_size = device->options()->optionSize(ScanOptions::getAvailableOption(descriptor->name));
  value = guardedMalloc(option_size);
  status = sane_control_option(sane_handle, option_id, SANE_ACTION_GET_VALUE, value, nullptr);

  if (status == SANE_STATUS_GOOD) {
    switch (descriptor->type) {
    case SANE_TYPE_BOOL:
      var = *static_cast<SANE_Word*>(value);
      break;

    case SANE_TYPE_INT:
      var = *static_cast<SANE_Int*>(value);
      break;

    case SANE_TYPE_FIXED:
      var = SANE_UNFIX(*static_cast<SANE_Word*>(value));
      break;

    case SANE_TYPE_STRING:
      var = QString(static_cast<const char*>(value));
      break;

    case SANE_TYPE_BUTTON:
      // TODO
      break;

    case SANE_TYPE_GROUP:
      // TODO
      break;
    }
  }

  return var;
}

QVariant SaneWorker::handleConstraintType(const SANE_Option_Descriptor* descriptor)
{
  QVariant v;

  switch (descriptor->type) {
  case SANE_TYPE_STRING: {
    QStringList list;
    QString s;

    switch (descriptor->constraint_type) {
    case SANE_CONSTRAINT_STRING_LIST: {
      for (int i = 0; descriptor->constraint.string_list[i] != nullptr; i++) {
        s = QString(descriptor->constraint.string_list[i]);
        list.append(s);
      }

      v.setValue(list);
      break;
    } // end SANE_CONSTRAINT_STRING_LIST

    case SANE_CONSTRAINT_WORD_LIST: {
      for (int i = 1; i <= descriptor->constraint.word_list[0]; i++) {
        s = QString(descriptor->constraint.word_list[i]);
        list.append(s);
      }

      v.setValue(list);
      break;
    } // end SANE_CONSTRAINT_WORD_LIST

    default:
      break;
    }

    break;
  } // end SANE_TYPE_STRING

  case SANE_TYPE_INT: {
    QList<int> list;

    switch (descriptor->constraint_type) {
    case SANE_CONSTRAINT_WORD_LIST: {
      for (int i = 1; i < descriptor->constraint.word_list[0]; i++) {
        list.append(descriptor->constraint.word_list[i]);
      }

      v.setValue(list);
      break;
    } // end SANE_CONSTRAINT_WORD_LIST

    case SANE_CONSTRAINT_RANGE: {
      RangeData range;
      range.min = descriptor->constraint.range->min;
      range.max = descriptor->constraint.range->max;
      v.setValue(range);
      break;
    } // end SANE_CONSTRAINT_RANGE

    default:
      break;
    } // end constraint_type

    break;
  } // end SANE_TYPE_INT

  case SANE_TYPE_FIXED: {
    QList<int> list;

    switch (descriptor->constraint_type) {
    case SANE_CONSTRAINT_WORD_LIST: {

      for (int i = 1; i < descriptor->constraint.word_list[0]; i++) {
        list.append(int(SANE_UNFIX(descriptor->constraint.word_list[i])));
      }

      v.setValue(list);
      break;
    } // end SANE_CONSTRAINT_WORD_LIST

    case SANE_CONSTRAINT_RANGE: {
      RangeData range;
      range.min = int(SANE_UNFIX(descriptor->constraint.range->min));
      range.max = int(SANE_UNFIX(descriptor->constraint.range->max));
      v.setValue(range);
      break;
    } // end SANE_CONSTRAINT_RANGE

    default:
      break;
    } // end switch constraint_type.

    break;
  } // end SANE_TYPE_FIXED.

  case SANE_TYPE_BOOL: {
    // TODO - possibly.
    break;
  }

  case SANE_TYPE_BUTTON: {
    // TODO - possibly
    break;
  }

  case SANE_TYPE_GROUP: {
    // TODO - possibly
    break;
  }
  }

  return v;
}

//void SaneWorker::storeIntOption(SANE_Handle sane_handle, ScanDevice* device,
//                                int option_id, const QString& name,
//                                const SANE_Value_Type type)
//{
//  SANE_Int v;
//  qreal value = 0;
//  SANE_Status status;

//  if (option_id > 0) {  // 0 is a special case already dealt with
//    status = sane_control_option(sane_handle, option_id, SANE_ACTION_GET_VALUE, &v, nullptr);

//    switch (type) {
//    case SANE_TYPE_INT:
//      value = v;
//      break;

//    case SANE_TYPE_FIXED:
//      value = SANE_UNFIX(v);
//      break;

//    default:
//      break;
//    }

//    if (status == SANE_STATUS_GOOD) {
//      device->options()->setOptionValue(ScanOptions::getAvailableOption(name), int(value));
//    }
//  }
//}


void SaneWorker::cancelScan()
{
  if (m_sane_handle) {
    sane_cancel(m_sane_handle);
  }
}

/* Allocate the requested memory plus enough room to store some guard bytes. */
void* SaneWorker::guardedMalloc(size_t size)
{
  unsigned char* ptr;

  size += 2 * GUARDS_SIZE;
  ptr = reinterpret_cast<unsigned char*>(malloc(size));

  //  assert(ptr);

  ptr += GUARDS_SIZE;

  return (ptr);
}

/* Free some memory allocated by guards_malloc. */
void SaneWorker::guardedFree(void* ptr)
{
  auto* p = reinterpret_cast<unsigned char*>(ptr);

  p -= GUARDS_SIZE;
  free(p);
}

/* Returns a string with the value of an option. */
QVariant SaneWorker::getDeviceValue(SANE_Handle sane_handle, ScanDevice* device, const QString& option_name)
{
  void* optval; /* value for the option */
  int option_id = device->options()->optionId(ScanOptions::getAvailableOption(option_name));
  int option_type = device->options()->optionType(ScanOptions::getAvailableOption(option_name));
  size_t option_size = device->options()->optionSize(ScanOptions::getAvailableOption(option_name));
  SANE_Status status;
  QVariant v;

  optval = guardedMalloc(option_size);
  status = sane_control_option(sane_handle, option_id, SANE_ACTION_GET_VALUE, optval, nullptr);

  if (status == SANE_STATUS_GOOD) {
    switch (option_type) {
    case SANE_TYPE_BOOL:
      v = *static_cast<SANE_Word*>(optval);
      break;

    case SANE_TYPE_INT:
      v = *static_cast<SANE_Int*>(optval);
      break;

    case SANE_TYPE_FIXED:
      v = SANE_UNFIX(*static_cast<SANE_Word*>(optval));
      break;

    case SANE_TYPE_STRING:
      v = QString(static_cast<const char*>(optval));
      break;
    }

  } else {
    /* Shouldn't happen. */
    //    strcpy(str, "backend default");
  }

  guardedFree(optval);

  return v;
}

} // end of namespace QScanner
