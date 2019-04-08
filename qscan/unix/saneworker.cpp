#include "saneworker.h"

#include <memory>

SaneWorker::SaneWorker(Device device, QObject* parent)
  : QObject(parent)
  , m_device(device)
{
  m_logger = Log4Qt::Logger::logger(QStringLiteral("SaneWorker"));
}

void
SaneWorker::scan()
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

  int i, len, offset = 0, full_size = 0;
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
            must_buffer = 1;
            offset = 0;
          }

          m_logger->warn(QString("Sane frame Mono depth 1"));

          break;

        } else if (parameters.depth == 8) {
          format = QImage::Format_Grayscale8;
          greyscale = true;

          if (parameters.lines < 0) {
            must_buffer = 1;
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
            must_buffer = 1;
            offset = 0;
          }

          m_logger->warn(QString("Sane frame Grey depth 16"));

          break;

        } else {
          emit scanFailed();
          return;
        }
    }

    m_image =
      Image(new QImage(parameters.pixels_per_line, parameters.lines, format));

  } else {
    // TODO not first frame ?
  }

  double progress;
  int x = 0, y = 0;
  quint32 value;

  while (true) {
    status = sane_read(m_device->sane_handle, buffer.get(), buffer_size, &len);

    if (status == SANE_STATUS_GOOD) {
      if (len > 0) {
        total_bytes += SANE_Word(len);
        progress = ((total_bytes * 100.) / double(full_size));
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
              m_image->setPixel(x, y, value);
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
