#include "scanimage.h"

/* ScanImage
 *****************************************************************************/
ScanImage::ScanImage(QWidget* parent)
  : QLabel(parent)
  , m_angle(0)
  , m_rb_start_x(0)
  , m_rb_start_y(0)
  , m_rb_end_x(0)
  , m_rb_end_y(0)
  //  , m_is_rubber_banding(false)
  , m_is_inside(false)
  //  , m_rb_complete(false)
  , m_state(RB_NOTHING)
{
  m_logger = Log4Qt::Logger::logger(QStringLiteral("ScanImage"));

  setStyleSheet("background-color: darkgrey;");
  setMouseTracking(true);
}

void
ScanImage::setImage(const QImage& image)
{
  m_image = image;
  update();
}

void
ScanImage::rotateBy(int angle)
{
  m_angle = angle;
  QImage rotated = m_image.transformed([&angle](QPoint center) {
    QMatrix matrix;
    matrix.translate(center.x(), center.y());
    matrix.rotate(angle);
    return matrix;
  }(m_image.rect().center()));
  m_image = rotated;
  update();
}

QSize
ScanImage::minimumSizeHint() const
{
  return { 182, 182 };
}

QSize
ScanImage::sizeHint() const
{
  return { 232, 232 };
}

bool
ScanImage::hasSelection()
{
  return (m_state == RB_COMPLETE);
}

QRect
ScanImage::selection()
{
  if (hasSelection()) {
    return m_rubber_band;
  }
  return {};
}

void
ScanImage::cropToSelection()
{
  if (hasSelection()) {
    QRect scaled_rect;
    scaled_rect.setX(int(m_rubber_band.x() / m_scaled_by));
    scaled_rect.setY(int(m_rubber_band.y() / m_scaled_by));
    scaled_rect.setWidth(int(m_rubber_band.width() / m_scaled_by));
    scaled_rect.setHeight(int(m_rubber_band.height() / m_scaled_by));

    QImage cropped = m_image.copy(scaled_rect);
    m_image = cropped;
    clearSelection();
    update();
  }
}

/*!
   \brief Copy selection to clipboard.
 */
void
ScanImage::copySelection()
{
  QClipboard* clipboard = QApplication::clipboard();
  if (hasSelection()) {
    QRect scaled_rect;
    scaled_rect.setX(int(m_rubber_band.x() / m_scaled_by));
    scaled_rect.setY(int(m_rubber_band.y() / m_scaled_by));
    scaled_rect.setWidth(int(m_rubber_band.width() / m_scaled_by));
    scaled_rect.setHeight(int(m_rubber_band.height() / m_scaled_by));

    QImage cropped = m_image.copy(scaled_rect);
    clipboard->setImage(cropped);
  } else {
    clipboard->setImage(m_image);
  }
}

void
ScanImage::selectAll()
{
  m_rubber_band = m_scaled_rect;
  m_stretched_band = m_rubber_band;
  m_state = RB_COMPLETE;
  update();
}

void
ScanImage::clearSelection()
{
  m_rubber_band = QRect();
  m_stretched_band = QRect();
  m_state = RB_NOTHING;
  update();
}

void
ScanImage::paintRubberBand(QPainter* painter)
{
  painter->save();
  QPen pen;
  QBrush brush;
  switch (m_state) {
  case RUBBER_BANDING: {
    pen = QPen(QColor("green"));
    brush = QBrush(QColor(128, 128, 255, 128));
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(m_rubber_band);
    break;
  } // end of RUBBER_BANDING
  case RB_COMPLETE: {
    pen = QPen(QColor("blue"));
    brush = QBrush(QColor(128, 128, 255, 128));
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(m_rubber_band);
    painter->drawRect(m_rubber_band.x() - 10, m_rubber_band.y() - 10, 20, 20);
    painter->drawRect(
      m_rubber_band.x() + m_rubber_band.width() - 10, m_rubber_band.y() - 10, 20, 20);
    painter->drawRect(
      m_rubber_band.x() - 10, m_rubber_band.y() + m_rubber_band.height() - 10, 20, 20);
    painter->drawRect(m_rubber_band.x() + m_rubber_band.width() - 10,
                      m_rubber_band.y() + m_rubber_band.height() - 10,
                      20,
                      20);
    break;
  } // end of RB_COMPLETE
  case RB_NOTHING:
    break;
  default:
    pen = QPen(QColor("green"));
    brush = QBrush(QColor(128, 128, 255, 128));
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(m_stretched_band);
    painter->drawRect(m_stretched_band.x() - 10, m_stretched_band.y() - 10, 20, 20);
    painter->drawRect(
      m_stretched_band.x() + m_stretched_band.width() - 10, m_stretched_band.y() - 10, 20, 20);
    painter->drawRect(
      m_stretched_band.x() - 10, m_stretched_band.y() + m_stretched_band.height() - 10, 20, 20);
    painter->drawRect(m_stretched_band.x() + m_stretched_band.width() - 10,
                      m_stretched_band.y() + m_stretched_band.height() - 10,
                      20,
                      20);
    break;
  } // end of switch
  painter->restore();
}

void
ScanImage::paintEvent(QPaintEvent*)
{
  if (!m_image.isNull()) {
    int w = width();
    int h = height();
    int centre_x = int(w / 2.0);
    int centre_y = int(h / 2.0);
    QPen pen;
    QBrush brush;
    // scale image to available size.
    QImage scaled_image = m_image.scaled(w, h, Qt::KeepAspectRatio);
    m_scaled_by = qreal(scaled_image.width()) / qreal(m_image.width());
    m_scaled_rect = scaled_image.rect();
    QPainter painter(this);
    painter.save();
    painter.fillRect(scaled_image.rect(), QBrush(Qt::yellow));
    painter.drawImage(0, 0, scaled_image);

    paintRubberBand(&painter);

    painter.restore();
  }
}

void
ScanImage::enterEvent(QEvent* evt)
{
  m_is_inside = true;
}

void
ScanImage::leaveEvent(QEvent*)
{
  m_is_inside = false;
}

void
ScanImage::mousePressEvent(QMouseEvent* event)
{
  if (!m_image.isNull()) {
    int e_x = event->x();
    int e_y = event->y();
    if (event->button() == Qt::LeftButton) {
      if (m_is_inside) {
        if (m_state == RB_NOTHING) {
          //        m_is_rubber_banding = true;
          m_state = RUBBER_BANDING;
          m_rb_start_x = e_x;
          m_rb_start_y = e_y;
          m_logger->debug(QString("m_start_x:%1, m_start_y:%2, m_end_x:%3, m_end_y:%4")
                            .arg(m_rb_start_x)
                            .arg(m_rb_start_y)
                            .arg(m_rb_end_x)
                            .arg(m_rb_end_y));
          update();
        } else if (m_state == RB_COMPLETE) {
          int left = m_rubber_band.x();
          int top = m_rubber_band.y();
          int bottom = top + m_rubber_band.height();
          int right = left + m_rubber_band.width();

          if ((e_x > left - 10 && e_x < left + 10) && (e_y > bottom - 10 && e_y < bottom + 10)) {
            m_state = STRETCH_BOTTOMLEFT;
          } else if ((e_x > right - 10 && e_x < right + 10) && (e_y > top - 10 && e_y < top + 10)) {
            m_state = STRETCH_TOPRIGHT;
          } else if ((e_x > left - 10 && e_x < left + 10) && (e_y > top - 10 && e_y < top + 10)) {
            m_state = STRETCH_TOPLEFT;
          } else if ((e_x < right + 10 && e_x > right - 10) &&
                     (e_y > bottom - 10 && e_y < bottom + 10)) {
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
            m_state = RB_MOVE;
          }
        }
      }
    }
  }
}

void
ScanImage::mouseReleaseEvent(QMouseEvent* event)
{
  if (!m_image.isNull()) {
    int e_x = event->x();
    int e_y = event->y();

    if (e_y < 0)
      e_y = 0;
    if (e_x < 0)
      e_x = 0;

    switch (m_state) {
    case RUBBER_BANDING: {
      m_rb_end_x = e_x;
      m_rb_end_y = e_y;
      m_rubber_band.setX((m_rb_start_x < m_rb_end_x ? m_rb_start_x : m_rb_end_x));
      m_rubber_band.setWidth(
        (m_rb_start_x < m_rb_end_x ? m_rb_end_x - m_rb_start_x : m_rb_start_x - m_rb_end_x));
      m_rubber_band.setY((m_rb_start_y < m_rb_end_y ? m_rb_start_y : m_rb_end_y));
      m_rubber_band.setHeight(
        (m_rb_start_y < m_rb_end_y ? m_rb_end_y - m_rb_start_y : m_rb_start_y - m_rb_end_y));
      m_stretched_band = m_rubber_band;
      m_logger->debug(QString("x:%1, y:%2, width:%3, height:%4")
                        .arg(m_rubber_band.x())
                        .arg(m_rubber_band.y())
                        .arg(m_rubber_band.width())
                        .arg(m_rubber_band.height()));
      m_state = RB_COMPLETE;
      update();
      break;
    } // end of RUBBER_BANDING
    case RB_NOTHING:
      break;
    default:
      m_rubber_band = m_stretched_band;
      m_state = RB_COMPLETE;
      update();
    } // end of switch.
  }
}

void
ScanImage::mouseMoveEvent(QMouseEvent* event)
{
  if (!m_image.isNull()) {
    int e_x = event->x();
    int e_y = event->y();
    int g_x = event->globalX();
    int g_y = event->globalY();
    int x = this->x();
    int y = this->y();
    int w = width();
    int h = height();

    if (e_y < 0)
      e_y = 0;
    if (e_x < 0)
      e_x = 0;

    if (m_is_inside) {
      switch (m_state) {
      case RUBBER_BANDING: {
        m_rb_end_x = e_x;
        m_rb_end_y = e_y;
        m_rubber_band.setX((m_rb_start_x < m_rb_end_x ? m_rb_start_x : m_rb_end_x));
        m_rubber_band.setWidth(
          (m_rb_start_x < m_rb_end_x ? m_rb_end_x - m_rb_start_x : m_rb_start_x - m_rb_end_x));
        m_rubber_band.setY((m_rb_start_y < m_rb_end_y ? m_rb_start_y : m_rb_end_y));
        m_rubber_band.setHeight(
          (m_rb_start_y < m_rb_end_y ? m_rb_end_y - m_rb_start_y : m_rb_start_y - m_rb_end_y));
        setCursor(Qt::SizeAllCursor);
        break;
      }
      case RB_COMPLETE: {
        int left = m_rubber_band.x();
        int top = m_rubber_band.y();
        int bottom = top + m_rubber_band.height();
        int right = left + m_rubber_band.width();

        if ((e_x > left - 10 && e_x < left + 10) && (e_y > bottom - 10 && e_y < bottom + 10)) {
          setCursor(Qt::SizeBDiagCursor);
          update();
        } else if ((e_x > right - 10 && e_x < right + 10) && (e_y > top - 10 && e_y < top + 10)) {
          setCursor(Qt::SizeBDiagCursor);
          update();
        } else if ((e_x > left - 10 && e_x < left + 10) && (e_y > top - 10 && e_y < top + 10)) {
          setCursor(Qt::SizeFDiagCursor);
          update();
        } else if ((e_x < right + 10 && e_x > right - 10) &&
                   (e_y > bottom - 10 && e_y < bottom + 10)) {
          setCursor(Qt::SizeFDiagCursor);
          update();
        } else if (e_x > left - 10 && e_x < left + 10) {
          setCursor(Qt::SizeHorCursor);
          update();
        } else if (e_x > right - 10 && e_x < right + 10) {
          setCursor(Qt::SizeHorCursor);
          update();
        } else if (e_y > top - 10 && e_y < top + 10) {
          setCursor(Qt::SizeVerCursor);
          update();
        } else if (e_y > bottom - 10 && e_y < bottom + 10) {
          setCursor(Qt::SizeVerCursor);
          update();
        } else {
          setCursor(Qt::CrossCursor);
          update();
        }
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
      case RB_MOVE:
        m_stretched_band.setX(e_x);
        m_stretched_band.setY(e_y);
        break;
      }
      update();
    }
  }
}
