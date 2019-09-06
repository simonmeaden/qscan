#include "basescanimage.h"

namespace QScanner {

const QColor BaseScanImage::EDGE_COLOR = QColor("blue");
const QColor BaseScanImage::EDGING_COLOR = QColor("green");
const QColor BaseScanImage::RUBBERBAND_COLOR = QColor("blue");
const QColor BaseScanImage::RUBBERBANDING_COLOR = QColor("green");
const QBrush BaseScanImage::RUBBERBAND_BRUSH = QBrush(QColor(128, 128, 255, 128));
const qreal BaseScanImage::ZOOM_IN_FACTOR = 1.1;
const qreal BaseScanImage::ZOOM_OUT_FACTOR = 0.9;

BaseScanImage::BaseScanImage(QWidget* parent)
  : QLabel(parent)
  , m_state(DOING_NOTHING)
  , m_fit_type(FIT_BEST)
  , m_rubber_band(QRect())
  , m_mouse_moved(false)
  , m_rb_start_x(0)
  , m_rb_start_y(0)
  , m_rb_end_x(0)
  , m_rb_end_y(0)
  , m_scale_by(1.0)
  , m_def_crop_set(false)
  , m_is_inside(false)
  , m_inverted(false)
  , m_image_changed(false)
{
  setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

bool BaseScanImage::hasSelection()
{
  return (m_state == RUBBERBAND_COMPLETE);
}

void BaseScanImage::setImage(const QImage& image, const int resolution)
{
  m_image = image;
  m_current_image = image;

  if (resolution > 0) {
    m_resolution = resolution;
  }

  fitByType();
}

QImage BaseScanImage::image()
{
  return m_image;
}

QImage BaseScanImage::currentImage()
{
  return m_current_image;
}

QImage BaseScanImage::selectedSubImage()
{
  if (hasSelection()) {
    QRect selected_rect;
    selected_rect.setX(int(m_rubber_band.x() / m_scale_by));
    selected_rect.setY(int(m_rubber_band.y() / m_scale_by));
    selected_rect.setWidth(int(m_rubber_band.width() / m_scale_by));
    selected_rect.setHeight(int(m_rubber_band.height() / m_scale_by));

    QImage copied_selection = m_current_image.copy(selected_rect);
    return copied_selection;
  }

  return QImage();
}

void BaseScanImage::clearToBackground()
{
  if (hasSelection()) {
    QRect selected_rect;
    selected_rect.setX(int(m_rubber_band.x() / m_scale_by));
    selected_rect.setY(int(m_rubber_band.y() / m_scale_by));
    selected_rect.setWidth(int(m_rubber_band.width() / m_scale_by));
    selected_rect.setHeight(int(m_rubber_band.height() / m_scale_by));

    QColor fill_color(Qt::white);

    if (m_inverted) {
      fill_color = QColor(Qt::black);
    }

    QPainter painter(&m_current_image);
    QPen pen(fill_color);
    painter.setPen(pen);
    painter.setBrush(fill_color);
    painter.drawRect(selected_rect);
    painter.end();

    scaleCurrentImage();
    clearSelection();
  }
}


void BaseScanImage::cropToSelection()
{
  if (hasSelection()) {
    QRect crop_rect;
    crop_rect.setX(int(m_rubber_band.x() / m_scale_by));
    crop_rect.setY(int(m_rubber_band.y() / m_scale_by));
    crop_rect.setWidth(int(m_rubber_band.width() / m_scale_by));
    crop_rect.setHeight(int(m_rubber_band.height() / m_scale_by));

    QImage cropped = m_current_image.copy(crop_rect);

    m_current_image = cropped;
    //    updateImage(/*cropped*/);
    scaleCurrentImage();
    clearSelection();
  }
}

void BaseScanImage::scaleCurrentImage()
{
  int w = int(m_current_image.width() * m_scale_by);
  int h = int(m_current_image.height() * m_scale_by);
  m_display_image = m_current_image.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  update();
}

void BaseScanImage::clearSelection()
{
  qCInfo(LogQScanWidgets) << tr("Clearing selection");
  m_rubber_band = QRect();
  m_stretched_band = QRect();
  m_edge_start = QPoint();
  m_edge_finish = QPoint();
  m_state = DOING_NOTHING;
  setCursor(Qt::ArrowCursor);
  update();
  emit unselected();
}

QRect BaseScanImage::selection()
{
  if (hasSelection()) {
    QRect scaled_rect;
    scaled_rect.setX(int(m_rubber_band.x() / m_scale_by));
    scaled_rect.setY(int(m_rubber_band.y() / m_scale_by));
    scaled_rect.setWidth(int(m_rubber_band.width() / m_scale_by));
    scaled_rect.setHeight(int(m_rubber_band.height() / m_scale_by));
    return scaled_rect;
  }

  return QRect();
}

void BaseScanImage::fitBest()
{
  m_fit_type = FIT_BEST;
  fitByType();
}

void BaseScanImage::fitHeight()
{
  m_fit_type = FIT_HEIGHT;
  fitByType();
}

void BaseScanImage::fitWidth()
{
  m_fit_type = FIT_WIDTH;
  fitByType();
}

void BaseScanImage::fitByType()
{
  QSize size = frameSize();

  if (!m_current_image.isNull()) {
    int w = size.width();
    int h = size.height();

    switch (m_fit_type) {
    case FIT_BEST: {
      auto scale_w = qreal(qreal(qreal(w) / qreal(m_current_image.width())));
      auto scale_h = qreal(qreal(qreal(h) / qreal(m_current_image.height())));
      m_scale_by = (scale_w < scale_h ? scale_w : scale_h);
      break;
    }

    case FIT_WIDTH:
      m_scale_by = qreal(qreal(qreal(w) / qreal(m_current_image.width())));
      break;

    case FIT_HEIGHT:
      m_scale_by = qreal(qreal(qreal(h) / qreal(m_current_image.height())));
      break;

    case ZOOM:
      Q_FALLTHROUGH();

    default:
      break;
    }

    scaleCurrentImage();
  }
}

void BaseScanImage::undoAllChanges()
{
  m_current_image = m_image;
}

int BaseScanImage::resolution() const
{
  return m_resolution;
}

void BaseScanImage::setResolution(int resolution)
{
  m_resolution = resolution;
}

void BaseScanImage::mousePressEvent(QMouseEvent* event)
{
  if (!m_current_image.isNull()) {
    m_mouse_moved = false;
    int e_x = event->x();
    int e_y = event->y();

    if (event->button() == Qt::LeftButton) {
      if (m_is_inside) {
        switch (m_state) {
        case EDGE_SELECTED:
          m_state = EDGE_STARTING;
          qCInfo(LogQScanWidgets) <<
                               tr("Edge draw pressed. (%1, %2)").arg(m_edge_start.x()).arg(m_edge_start.y());
          m_edge_start = event->pos();
          m_edge_finish = m_edge_start;
          update();
          emit selectionUnderway();
          break;

        case DOING_NOTHING:
          m_state = RUBBERBAND_STARTING;
          emit selectionUnderway();
          m_rb_start_x = e_x;
          m_rb_start_y = e_y;
          update();
          break;

        case RUBBERBAND_COMPLETE: {
          int left = m_rubber_band.x();
          int top = m_rubber_band.y();
          int bottom = top + m_rubber_band.height();
          int right = left + m_rubber_band.width();
          emit selectionUnderway();

          if ((e_x > left - 10 && e_x < left + 10) && (e_y > bottom - 10 && e_y < bottom + 10)) {
            m_state = STRETCH_BOTTOMLEFT;

          } else if ((e_x > right - 10 && e_x < right + 10) && (e_y > top - 10 && e_y < top + 10)) {
            m_state = STRETCH_TOPRIGHT;

          } else if ((e_x > left - 10 && e_x < left + 10) && (e_y > top - 10 && e_y < top + 10)) {
            m_state = STRETCH_TOPLEFT;

          } else if ((e_x < right + 10 && e_x > right - 10)
                     && (e_y > bottom - 10 && e_y < bottom + 10)) {
            m_state = STRETCH_BOTTOMRIGHT;

          } else if (e_x > left - 10 && e_x < left + 10) {
            m_state = STRETCH_LEFT;

          } else if (e_x > right - 10 && e_x < right + 10) {
            m_state = STRETCH_RIGHT;

          } else if (e_y > top - 10 && e_y < top + 10) {
            m_state = STRETCH_TOP;

          } else if (e_y > bottom - 10 && e_y < bottom + 10) {
            m_state = STRETCH_BOTTOM;

          } else {
            m_state = RUBBERBAND_MOVE;
          }

          break;

        } // end case

        default:
          break;
        } // end switch
      }   // end if m_is_inside
    }     // end left button check
  }       // end m_image check
}

void BaseScanImage::mouseMoveEvent(QMouseEvent* event)
{
  if (!m_current_image.isNull()) {
    int e_x = event->x();
    int e_y = event->y();

    if ((qAbs(m_rubber_band.x() - e_x) > RUBBERBANDING_MIN &&
         qAbs(m_rubber_band.y()) > RUBBERBANDING_MIN) ||
        (qAbs(m_edge_start.x() - e_x) > RUBBERBANDING_MIN &&
         qAbs(m_edge_start.y()) > RUBBERBANDING_MIN)) {
      m_mouse_moved = true;
    }

    if (e_y < 0) {
      e_y = 0;
    }

    if (e_x < 0) {
      e_x = 0;
    }

    if (m_is_inside) {
      switch (m_state) {
      case EDGE_STARTING:
        if (m_mouse_moved) {
          m_state = EDGE_DRAWING;
        }

        break;

      case EDGE_DRAWING:
        qCInfo(LogQScanWidgets) << tr("Edge moving. (%1, %2)").arg(m_edge_finish.x()).arg(m_edge_finish.y());
        m_edge_finish = event->pos();
        break;

      case RUBBERBAND_STARTING:
        if (m_mouse_moved) {
          m_state = RUBBERBANDING;
        }

        Q_FALLTHROUGH();

      case RUBBERBANDING:
        m_rb_end_x = e_x;
        m_rb_end_y = e_y;
        m_rubber_band.setX((m_rb_start_x < m_rb_end_x ? m_rb_start_x : m_rb_end_x));
        m_rubber_band.setWidth(
          (m_rb_start_x < m_rb_end_x ? m_rb_end_x - m_rb_start_x : m_rb_start_x - m_rb_end_x));
        m_rubber_band.setY((m_rb_start_y < m_rb_end_y ? m_rb_start_y : m_rb_end_y));
        m_rubber_band.setHeight(
          (m_rb_start_y < m_rb_end_y ? m_rb_end_y - m_rb_start_y : m_rb_start_y - m_rb_end_y));
        setCursor(Qt::CrossCursor);
        break;

      case RUBBERBAND_MOVE:
        m_stretched_band.setX(e_x);
        m_stretched_band.setY(e_y);

        if (m_mouse_moved) {
          setCursor(Qt::ClosedHandCursor);
        }

        break;

      case RUBBERBAND_COMPLETE: {
        int left = m_rubber_band.x();
        int top = m_rubber_band.y();
        int bottom = top + m_rubber_band.height();
        int right = left + m_rubber_band.width();

        if ((e_x > left - 10 && e_x < left + 10) && (e_y > bottom - 10 && e_y < bottom + 10)) {
          setCursor(Qt::SizeBDiagCursor);

        } else if ((e_x > right - 10 && e_x < right + 10) && (e_y > top - 10 && e_y < top + 10)) {
          setCursor(Qt::SizeBDiagCursor);

        } else if ((e_x > left - 10 && e_x < left + 10) && (e_y > top - 10 && e_y < top + 10)) {
          setCursor(Qt::SizeFDiagCursor);

        } else if ((e_x < right + 10 && e_x > right - 10)
                   && (e_y > bottom - 10 && e_y < bottom + 10)) {
          setCursor(Qt::SizeFDiagCursor);

        } else if (e_x > left - 10 && e_x < left + 10) {
          setCursor(Qt::SizeHorCursor);

        } else if (e_x > right - 10 && e_x < right + 10) {
          setCursor(Qt::SizeHorCursor);

        } else if (e_y > top - 10 && e_y < top + 10) {
          setCursor(Qt::SizeVerCursor);

        } else if (e_y > bottom - 10 && e_y < bottom + 10) {
          setCursor(Qt::SizeVerCursor);

        } else {
          setCursor(Qt::ArrowCursor);
        }

        update();
        break;
      }

      case STRETCH_TOP:
        m_stretched_band.setY(e_y);
        m_stretched_band.setHeight(m_rubber_band.height() - (e_y - m_rubber_band.y()));
        break;

      case STRETCH_BOTTOM:
        m_stretched_band.setHeight(e_y - m_rubber_band.y());
        break;

      case STRETCH_LEFT:
        m_stretched_band.setX(e_x);
        m_stretched_band.setWidth(m_rubber_band.width() - (e_x - m_rubber_band.x()));
        break;

      case STRETCH_RIGHT:
        m_stretched_band.setWidth(e_x - m_rubber_band.x());
        break;

      case STRETCH_TOPLEFT:
        m_stretched_band.setY(e_y);
        m_stretched_band.setHeight(m_rubber_band.height() - (e_y - m_rubber_band.y()));
        m_stretched_band.setX(e_x);
        m_stretched_band.setWidth(m_rubber_band.width() - (e_x - m_rubber_band.x()));
        break;

      case STRETCH_TOPRIGHT:
        m_stretched_band.setY(e_y);
        m_stretched_band.setHeight(m_rubber_band.height() - (e_y - m_rubber_band.y()));
        m_stretched_band.setWidth(e_x - m_rubber_band.x());
        break;

      case STRETCH_BOTTOMLEFT:
        m_stretched_band.setHeight(e_y - m_rubber_band.y());
        m_stretched_band.setX(e_x);
        m_stretched_band.setWidth(m_rubber_band.width() - (e_x - m_rubber_band.x()));
        break;

      case STRETCH_BOTTOMRIGHT:
        m_stretched_band.setHeight(e_y - m_rubber_band.y());
        m_stretched_band.setWidth(e_x - m_rubber_band.x());
        break;

      case DOING_NOTHING:
        setCursor(Qt::CrossCursor);
        break;

      default:
        break;
      }

      update();
    }
  }
}

void BaseScanImage::mouseReleaseEvent(QMouseEvent* event)
{
  if (!m_current_image.isNull()) {
    int e_x = event->x();
    int e_y = event->y();

    if ((qAbs(m_rubber_band.x() - e_x) > RUBBERBANDING_MIN &&
         qAbs(m_rubber_band.y()) > RUBBERBANDING_MIN) ||
        (qAbs(m_edge_start.x() - e_x) > RUBBERBANDING_MIN &&
         qAbs(m_edge_start.y()) > RUBBERBANDING_MIN)) {
      m_mouse_moved = false;
    }

    if (e_y < 0) {
      e_y = 0;
    }

    if (e_x < 0) {
      e_x = 0;
    }

    switch (m_state) {
    case EDGE_STARTING:
    case RUBBERBAND_STARTING:
      // mouse pressed and released without moving.
      clearSelection();
      emit unselected();
      break;

    case EDGE_DRAWING:
      m_mouse_moved = false;
      m_edge_finish = event->pos();
      m_state = DOING_NOTHING;
      qCInfo(LogQScanWidgets) <<
                           tr("Edge draw released. (%1, %2)").arg(m_edge_finish.x()).arg(m_edge_finish.y());
      rotateUsingEdge();
      emit unselected();
      break;

    case RUBBERBANDING: {
      m_rb_end_x = e_x;
      m_rb_end_y = e_y;
      m_rubber_band.setX((m_rb_start_x < m_rb_end_x ? m_rb_start_x : m_rb_end_x));
      m_rubber_band.setWidth(
        (m_rb_start_x < m_rb_end_x ? m_rb_end_x - m_rb_start_x : m_rb_start_x - m_rb_end_x));
      m_rubber_band.setY((m_rb_start_y < m_rb_end_y ? m_rb_start_y : m_rb_end_y));
      m_rubber_band.setHeight(
        (m_rb_start_y < m_rb_end_y ? m_rb_end_y - m_rb_start_y : m_rb_start_y - m_rb_end_y));
      m_stretched_band = m_rubber_band;
      qCDebug(LogQScanWidgets) << tr("x:%1, y:%2, width:%3, height:%4")
                            .arg(m_rubber_band.x())
                            .arg(m_rubber_band.y())
                            .arg(m_rubber_band.width())
                            .arg(m_rubber_band.height());
      m_state = RUBBERBAND_COMPLETE;
      emit selected();
      update();
      break;
    } // end of RUBBER_BANDING

    case RUBBERBAND_MOVE:
      if (m_mouse_moved) {
        m_state = RUBBERBAND_COMPLETE;
        emit selected();

      } else {
        clearSelection();
      }

      update();
      break;

    case DOING_NOTHING:
      break;

    case STRETCH_TOP:
    case STRETCH_BOTTOM:
    case STRETCH_LEFT:
    case STRETCH_RIGHT:
    case STRETCH_TOPLEFT:
    case STRETCH_TOPRIGHT:
    case STRETCH_BOTTOMLEFT:
    case STRETCH_BOTTOMRIGHT:
      m_rubber_band = m_stretched_band;
      emit selected();
      m_state = RUBBERBAND_COMPLETE;
      update();
      break;

    default:
      m_rubber_band = m_stretched_band;
      clearSelection();
      emit selected();
      update();
    } // end of switch.
  }
}
void BaseScanImage::paintEvent(QPaintEvent* event)
{
  if (!m_current_image.isNull()) {
    QLabel::paintEvent(event);
    QPainter painter(this);
    painter.drawImage(m_display_image.rect().topLeft(), m_display_image);
    paintRubberBand(&painter);
    painter.end();
  }
}

void BaseScanImage::paintRubberBand(QPainter* painter)
{
  QPen pen;
  QBrush brush;

  switch (m_state) {
  //      case EDGE_STARTING:
  //        break;
  case EDGE_DRAWING:
    pen = QPen(EDGING_COLOR);
    pen.setWidth(EDGE_WIDTH);
    painter->setPen(pen);
    painter->drawLine(m_edge_start, m_edge_finish);
    break;

  case RUBBERBANDING: {
    pen = QPen(QColor("green"));
    pen.setWidth(RUBBERBAND_WIDTH);
    brush = QBrush(QColor(128, 128, 255, 128));
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(m_rubber_band);
    break;
  } // end of RUBBER_BANDING

  case RUBBERBAND_COMPLETE: {
    pen = QPen(QColor("blue"));
    pen.setWidth(RUBBERBAND_WIDTH);
    brush = QBrush(QColor(128, 128, 255, 128));
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(m_rubber_band);
    painter->drawRect(m_rubber_band.x() - 10, m_rubber_band.y() - 10, 20, 20);
    painter->drawRect(m_rubber_band.x() + m_rubber_band.width() - 10,
                      m_rubber_band.y() - 10,
                      20,
                      20);
    painter->drawRect(m_rubber_band.x() - 10,
                      m_rubber_band.y() + m_rubber_band.height() - 10,
                      20,
                      20);
    painter->drawRect(m_rubber_band.x() + m_rubber_band.width() - 10,
                      m_rubber_band.y() + m_rubber_band.height() - 10,
                      20,
                      20);
    break;
  } // end of RB_COMPLETE

  case DOING_NOTHING:
    break;

  case STRETCH_TOP:
  case STRETCH_BOTTOM:
  case STRETCH_LEFT:
  case STRETCH_RIGHT:
  case STRETCH_TOPLEFT:
  case STRETCH_TOPRIGHT:
  case STRETCH_BOTTOMLEFT:
  case STRETCH_BOTTOMRIGHT:
    pen = QPen(QColor("green"));
    pen.setWidth(RUBBERBAND_WIDTH);
    brush = QBrush(QColor(128, 128, 255, 128));
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(m_stretched_band);
    painter->drawRect(m_stretched_band.x() - 10, m_stretched_band.y() - 10, 20, 20);
    painter->drawRect(m_stretched_band.x() + m_stretched_band.width() - 10,
                      m_stretched_band.y() - 10,
                      20,
                      20);
    painter->drawRect(m_stretched_band.x() - 10,
                      m_stretched_band.y() + m_stretched_band.height() - 10,
                      20,
                      20);
    painter->drawRect(m_stretched_band.x() + m_stretched_band.width() - 10,
                      m_stretched_band.y() + m_stretched_band.height() - 10,
                      20,
                      20);
    break;

  default:
    break;
  } // end of switch
}

void BaseScanImage::enterEvent(QEvent*)
{
  m_is_inside = true;
}

void BaseScanImage::leaveEvent(QEvent*)
{
  m_is_inside = false;
}

void BaseScanImage::wheelEvent(QWheelEvent* event)
{
  int delta = event->delta();

  if (event->modifiers().testFlag(Qt::ControlModifier)) {
    // zoom in/out
    if (delta < 0) {
      zoomOut();

    } else {
      zoomIn();
    }

    update();

  } else {
    // scroll up/down
  }
}

// void BaseScanImage::resizeEvent(QResizeEvent* event)
//{
//  QLabel::resizeEvent(event);
//}

void BaseScanImage::zoomIn()
{
  m_fit_type = ZOOM;
  m_scale_by = m_scale_by * ZOOM_IN_FACTOR;
  fitByType();
}

void BaseScanImage::zoomOut()
{
  m_fit_type = ZOOM;
  m_scale_by = m_scale_by * ZOOM_OUT_FACTOR;
  fitByType();
}

void BaseScanImage::rotateByEdge()
{
  m_state = EDGE_SELECTED;
}

void BaseScanImage::rotateByAngle()
{
  bool ok;
  qreal angle = QInputDialog::getDouble(this,
                                        tr("Rotate by angle"),
                                        tr("Rotate the image by a fixed angle around the centre."),
                                        0.0,   // value
                                        0.0,   // min
                                        360.0, // max
                                        1,     // decimals
                                        &ok);

  if (angle > 0.0 && ok) {
    rotateBy(angle);
  }
}

void BaseScanImage::rotate180()
{
  m_op_images.append(m_current_image);
  m_image_changed = true;
  //  emit imageHasChanged(m_image_changed);
  rotateBy(180);
}

void BaseScanImage::rotateCW()
{
  m_op_images.append(m_current_image);
  m_image_changed = true;
  //  emit imageHasChanged(m_image_changed);
  rotateBy(90.0);
}

void BaseScanImage::rotateCCW()
{
  m_op_images.append(m_current_image);
  m_image_changed = true;
  //  emit imageHasChanged(m_image_changed);
  rotateBy(-90.0);
}

void BaseScanImage::rotateBy(qreal angle)
{
  qCInfo(LogQScanWidgets) << tr("Rotating by %1°").arg(angle);
  QImage rotated = m_current_image.transformed([&angle](QPoint center) {
    QMatrix matrix;
    matrix.translate(center.x(), center.y());
    matrix.rotate(angle);
    return matrix;
  }
  (m_current_image.rect().center()));
  setImage(rotated);
  m_image_changed = true;
  //  emit imageHasChanged(m_image_changed);
  fitBest();
  update();
}

void BaseScanImage::rotateUsingEdge()
{
  int x_start = m_edge_start.x();
  int x_finish = m_edge_finish.x();
  int y_start = m_edge_start.y();
  int y_finish = m_edge_finish.y();
  qreal w = x_finish - x_start;
  qreal h = y_finish - y_start;
  qreal angle = qRadiansToDegrees(qAtan(qAbs(h) / qAbs(w)));

  if ((w > 0 && h > 0) || (w < 0 && h < 0)) {
    rotateBy(-angle);

  } else {
    rotateBy(angle);
  }

  m_image_changed = true;
  //  emit imageHasChanged(m_image_changed);
  clearSelection();
}

} // end of namespace QScanner
