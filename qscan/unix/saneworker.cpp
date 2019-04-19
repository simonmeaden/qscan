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

SaneWorker::SaneWorker(ScanDevice* device, QObject* parent)
  : QObject(parent)
  , m_device(device)
{
  m_logger = Log4Qt::Logger::logger(QStringLiteral("SaneWorker"));
}

void SaneWorker::scan()
{
  SANE_Handle handle = m_device->sane_handle;
  SANE_Status status;
  SANE_Parameters parameters;
  status = sane_start(handle);

  if (status != SANE_STATUS_GOOD) {
    emit scanFailed();
    return;
  }

  status = sane_get_parameters(handle, &parameters);

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
        m_logger->warn(QString("Sane frame RED/GREEN/BLUE"));
        break;
      }

      m_logger->warn(QString("Sane frame RED/GREEN/BLUE but depth not 8"));
      emit scanFailed();
      return;

    case SANE_FRAME_RGB:
      if (parameters.depth == 8) {
        format = QImage::Format_RGB32;
        m_logger->warn(QString("Sane frame RGB depth 8"));
        break;

      } else if (parameters.depth == 16) {
        format = QImage::QImage::Format_RGBX64;
        m_logger->warn(QString("Sane frame RGB depth 16"));
        break;
      }

      m_logger->warn(QString("Sane frame RGB but depth 1"));
      emit scanFailed();
      return;

    case SANE_FRAME_GRAY:
      if (parameters.depth == 1) {
        format = QImage::Format_Mono;

        if (parameters.lines < 0) {
          must_buffer = true;
          offset = 0;
        }

        m_logger->warn(QString("Sane frame Mono depth 1"));
        break;

      } else if (parameters.depth == 8) {
        format = QImage::Format_Grayscale8;
        greyscale = true;

        if (parameters.lines < 0) {
          must_buffer = true;
          offset = 0;
        }

        m_logger->warn(QString("Sane frame Grey depth 8"));
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

        m_logger->warn(QString("Sane frame Grey depth 16"));
        break;

      } else {
        emit scanFailed();
        return;
      }
    }

    m_image = QImage(parameters.pixels_per_line, parameters.lines, format);

  } else {
    // TODO not first frame ?
  }

  int progress;
  int x = 0, y = 0;
  quint32 value;

  while (true) {
    status = sane_read(m_device->sane_handle, buffer.get(), buffer_size, &len);

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
              m_image.setPixel(x, y, value);
            }

          } else if (format == QImage::Format_RGBX64) {
          }
        } else {
          // SANE_FRAME_RED,GREEN & BLUE
          // TODO
        }
      }
    }

    if (total_bytes == full_size) {
      break;
    }
  }

  emit scanCompleted(m_image);
  emit finished();
  //  emit scanFailed();
  //  return;
}

void SaneWorker::getAvailableScannerOptions(ScanDevice* device, ScanOptions options)
{
  QMutexLocker locker(&m_mutex);
  SANE_Status current_status;
  SANE_Int number_of_options = 0;
  SANE_Int option_id = 1;
  current_status =
    sane_control_option(device->sane_handle, 0, SANE_ACTION_GET_VALUE, &number_of_options, nullptr);

  if (current_status == SANE_STATUS_GOOD) {
    for (SANE_Int i = 1; i < number_of_options; ++option_id) {
      if (const auto current_option = sane_get_option_descriptor(device->sane_handle, option_id)) {
        QString name(current_option->name);
        QString title(current_option->title);

        if (name == SANE_NAME_SCAN_TL_X) {
          options.setTopLeftX(options.getIntScannerValue(device, option_id, current_option));

        } else if (name == SANE_NAME_SCAN_TL_Y) {
          options.setTopLeftY(options.getIntScannerValue(device, option_id, current_option));

        } else if (name == SANE_NAME_SCAN_BR_X) {
          options.setBottomRightX(options.getIntScannerValue(device, option_id, current_option));

        } else if (name == SANE_NAME_SCAN_BR_Y) {
          options.setBottomRightY(options.getIntScannerValue(device, option_id, current_option));

        } else if (name == SANE_NAME_SCAN_RESOLUTION) {
          options.setScanResolution(options.getIntScannerValue(device, option_id, current_option));

        } else if (name == SANE_NAME_SCAN_X_RESOLUTION) {
          options.setScanResolutionX(options.getIntScannerValue(device, option_id, current_option));

        } else if (name == SANE_NAME_SCAN_Y_RESOLUTION) {
          options.setScanResolutionY(options.getIntScannerValue(device, option_id, current_option));

        } else if (name == SANE_NAME_CONTRAST) {
          options.setContrast(options.getIntScannerValue(device, option_id, current_option));

        } else if (name == SANE_NAME_BRIGHTNESS) {
          options.setBrightness(options.getIntScannerValue(device, option_id, current_option));

        } else if (name == SANE_NAME_SCAN_SOURCE) {
          // TODO get this working. At the moment this crashes with an empty 'this' value.
          // getStringScannerValue creates the QStringList internally but fails on return ????
          // getStringScannerValue works OK in SANE_NAME_SCAN_MODE.
          //          QStringList list = options.getStringScannerValue(device, option_id, current_option);
          //          options.setSources(list);
        } else if (name == SANE_NAME_SCAN_MODE) {
          options.setScanModes(options.getStringScannerValue(device, option_id, current_option));
        }

        if (!name.isEmpty()) {
          options.setOptionId(name, option_id);
        }

        ++i;
      }
    }
  }

  emit availableScannerOptions(device->name, options);
}
