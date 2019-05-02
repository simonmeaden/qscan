#include "scanimage.h"
#include "scaneditor.h"

/* ScanImage
 *****************************************************************************/
const QColor ScanImage::EDGE_COLOR = QColor("blue");
const QColor ScanImage::EDGING_COLOR = QColor("green");
const QColor ScanImage::RUBBERBAND_COLOR = QColor("blue");
const QColor ScanImage::RUBBERBANDING_COLOR = QColor("green");
const QBrush ScanImage::RUBBERBAND_BRUSH = QBrush(QColor(128, 128, 255, 128));
const qreal ScanImage::ZOOM_IN_FACTOR = 1.1;
const qreal ScanImage::ZOOM_OUT_FACTOR = 0.9;

ScanImage::ScanImage(const QString& datadir, QWidget* parent)
  : QLabel(parent)
  , m_datadir(datadir)
  , m_scale_by(1.0)
  , m_rotation(0)
  , m_rb_start_x(0)
  , m_rb_start_y(0)
  , m_rb_end_x(0)
  , m_rb_end_y(0)
  , m_rubber_band(QRect())
  , m_def_crop_set(false)
  , m_is_inside(false)
  , m_state(DOING_NOTHING)
  , m_mouse_moved(false)
  , m_copy_selection_act(new QAction(tr("Copy selection"), this))
  , m_crop_to_selection_act(new QAction(tr("Clear selection"), this))
  , m_clear_selection_act(new QAction(tr("Clear selection"), this))
  , m_crop_to_content_act(new QAction(tr("Crop to content"), this))
  , m_rotate_cw_act(new QAction(tr("Rotate 90° clockwise"), this))
  , m_rotate_acw_act(new QAction(tr("Rotate 90° anti-clockwise"), this))
  , m_rotate_180_act(new QAction(tr("Rotate 180°"), this))
  , m_rotate_by_angle_act(new QAction(tr("Rotate by angle"), this))
  , m_rotate_by_edge_act(new QAction(tr("Rotate by edge"), this))
  , m_rescan_act(new QAction(tr("Re-scan image to crop"), this))
  , m_scale_act(new QAction(tr("Scale image"), this))
  , m_selectall_act(new QAction(tr("Select entire image"), this))
  , m_save_act(new QAction(tr("Save image"), this))
  , m_save_as_act(new QAction(tr("Save image as"), this))
  , m_set_def_crop_act(new QAction(tr("Set default page crop size"), this))
  //  , m_select_all(false)
  , m_split_pages_act(new QAction(tr("Split image into two pages"), this))
  , m_split_right_act(
      new QAction(tr("Make right half into a single page"), this))
  , m_split_left_act(new QAction(tr("Make left half into a single page"), this))
  , m_make_page_act(new QAction(tr("Make image into a single page"), this))
{
  m_logger = Log4Qt::Logger::logger(tr("ScanImage"));
  initActions();
  setMouseTracking(true);
}

void
ScanImage::setImage(const QImage& image)
{
  m_image = image;
  //  m_display_scaled_by = 1.0; //qreal(width()) / qreal(m_image.width());
  //  m_scaled_image = m_image;
  setPixmap(QPixmap::fromImage(m_image));
  fitBest();
  emit imageIsLoaded();
  // allows multi use of the same crop size.
  if (m_def_crop_set) {
    m_rubber_band = m_default_crop_size;
    m_state = RUBBERBAND_COMPLETE;
  }
  update();
}

void
ScanImage::rotateBy(qreal angle)
{
  m_logger->info(tr("Rotating by %1°").arg(angle));
  QImage rotated = m_image.transformed([&angle](QPoint center) {
    QMatrix matrix;
    matrix.translate(center.x(), center.y());
    matrix.rotate(angle);
    return matrix;
  }(m_image.rect().center()));
  setImage(rotated);
  fitBest();
  update();
}

void
ScanImage::rotateByEdge()
{
  m_state = EDGE_SELECTED;
}

void
ScanImage::scaleBy()
{ // TODO scale by.
}

void
ScanImage::save(const QString& doc_name)
{
  if (doc_name.isEmpty()) {
    if (m_filename.isEmpty()) {
      saveAs();
    } else {
      m_image.save(m_filename);
    }
  } else {
    QString path = m_datadir + QDir::separator() + doc_name;
    QDir dir;
    dir.mkpath(path);
  }
}

void
ScanImage::saveAs()
{
  QString filename = QFileDialog::getSaveFileName(
    this,
    tr("Save Image"),
    ".",
    tr("PNG Images (*.png);;JPEG images (*.jpg);;BMP Images (*.bmp)"));
  if (!filename.isEmpty()) {
    m_filename = filename;
    m_image.save(m_filename);
  }
}

bool
ScanImage::hasSelection()
{
  return (m_state == RUBBERBAND_COMPLETE);
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
    m_logger->info(tr("Cropping to selection"));
    QRect scaled_rect;
    scaled_rect.setX(int(m_rubber_band.x() / m_scale_by));
    scaled_rect.setY(int(m_rubber_band.y() / m_scale_by));
    scaled_rect.setWidth(int(m_rubber_band.width() / m_scale_by));
    scaled_rect.setHeight(int(m_rubber_band.height() / m_scale_by));

    QImage cropped = m_image.copy(scaled_rect);

    setImage(cropped);
    clearSelection();
  }
}

/*!
   \brief Copy selection to clipboard.
 */
void
ScanImage::copySelection()
{
#ifndef QT_NO_CLIPBOARD
  QClipboard* clipboard = QGuiApplication::clipboard();
  if (hasSelection()) {
    m_logger->info(tr("Copying to clipboard"));
    QRect scaled_rect;
    scaled_rect.setX(int(m_rubber_band.x() / m_scale_by));
    scaled_rect.setY(int(m_rubber_band.y() / m_scale_by));
    scaled_rect.setWidth(int(m_rubber_band.width() / m_scale_by));
    scaled_rect.setHeight(int(m_rubber_band.height() / m_scale_by));

    QImage cropped = m_image.copy(scaled_rect);
    clipboard->setImage(cropped);
  } else {
    clipboard->setImage(m_image);
  }
#endif // !QT_NO_CLIPBOARD
}

void
ScanImage::selectAll()
{
  m_logger->info(tr("Selecting all"));
  //  m_rubber_band = m_scaled_rect;
  m_rubber_band = m_image.rect();
  m_stretched_band = m_rubber_band;
  m_state = RUBBERBAND_COMPLETE;
  emit selected();
  enableSetDefaultCropSize();
  enableSelectionActions();
  disableNoSelectionActions();
  update();
}

void
ScanImage::clearSelection()
{
  m_logger->info(tr("Clearing selection"));
  m_rubber_band = QRect();
  m_stretched_band = QRect();
  m_edge_start = QPoint();
  m_edge_finish = QPoint();
  m_state = DOING_NOTHING;
  setCursor(Qt::ArrowCursor);
  update();
  emit unselected();
  disableSetDefaultCropSize();
  disableSelectionActions();
  enableNoSelectionActions();
}

void
ScanImage::setDefaultPageCropSize()
{
  m_default_crop_size = m_rubber_band;
  m_def_crop_set = true;
}

QPair<QImage, QImage>
ScanImage::splitPages()
{
  int w = m_image.width();
  int w2 = int(w / 2.0);

  m_logger->info(tr("Copying to clipboard"));
  QRect part_rect;
  part_rect.setX(0);
  part_rect.setY(0);
  part_rect.setWidth(w2);
  part_rect.setHeight(m_image.height());

  QImage left = m_image.copy(part_rect);

  part_rect.setX(w2 + 1);
  part_rect.setWidth(w - w2);

  QImage right = m_image.copy(part_rect);

  emit sendImages(left, right);

  return qMakePair<QImage, QImage>(left, right);
}

QImage
ScanImage::splitLeftPage()
{
  int w2 = int(m_image.width() / 2.0);

  m_logger->info(tr("Copying to clipboard"));
  QRect part_rect;
  part_rect.setX(0);
  part_rect.setY(0);
  part_rect.setWidth(w2);
  part_rect.setHeight(m_image.height());

  QImage left = m_image.copy(part_rect);

  emit sendImage(left);

  return left;
}

QImage
ScanImage::splitRightPage()
{
  int w = m_image.width();
  int w2 = int(w / 2.0);

  m_logger->info(tr("Copying to clipboard"));
  QRect part_rect;
  part_rect.setX(w2 + 1);
  part_rect.setY(0);
  part_rect.setWidth(w - w2);
  part_rect.setHeight(m_image.height());

  QImage right = m_image.copy(part_rect);

  emit sendImage(right);

  return right;
}

QImage
ScanImage::makePage()
{
  emit sendImage(m_image);
  return m_image;
}

void
ScanImage::contextMenuEvent(QContextMenuEvent* event)
{
  auto* context_menu = new QMenu();

  if (hasSelection()) {
    context_menu->addAction(m_save_act);
    context_menu->addAction(m_save_as_act);
    context_menu->addSeparator();
    context_menu->addAction(m_copy_selection_act);
    context_menu->addAction(m_crop_to_selection_act);
    context_menu->addAction(m_clear_selection_act);
    context_menu->addSeparator();
    context_menu->addAction(m_rescan_act);
    context_menu->addSeparator();
    context_menu->addAction(m_set_def_crop_act);

  } else {
    context_menu->addAction(m_save_act);
    context_menu->addAction(m_save_as_act);
    context_menu->addSeparator();
    context_menu->addAction(m_selectall_act);
    context_menu->addSeparator();
    context_menu->addAction(m_crop_to_content_act);
    context_menu->addSeparator();
    context_menu->addAction(m_rotate_cw_act);
    context_menu->addAction(m_rotate_acw_act);
    context_menu->addAction(m_rotate_180_act);
    context_menu->addAction(m_rotate_by_angle_act);
    context_menu->addAction(m_rotate_by_edge_act);
    context_menu->addSeparator();
    context_menu->addAction(m_scale_act);
    context_menu->addSeparator();
    context_menu->addAction(m_rescan_act);
    context_menu->addSeparator();
    context_menu->addAction(m_split_pages_act);
    context_menu->addAction(m_split_left_act);
    context_menu->addAction(m_split_right_act);
    context_menu->addAction(m_make_page_act);
  }

  context_menu->popup(event->globalPos());
}

void
ScanImage::paintRubberBand(QPainter* painter)
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
      painter->drawRect(
        m_stretched_band.x() - 10, m_stretched_band.y() - 10, 20, 20);
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

void
ScanImage::rotateUsingEdge()
{
  int x_start = m_edge_start.x();
  int x_finish = m_edge_finish.x();
  int y_start = m_edge_start.y();
  int y_finish = m_edge_finish.y();
  qreal w = x_finish - x_start;
  qreal h = y_finish - y_start;
  qreal angle = qRadiansToDegrees(qAtan(qAbs(w) / qAbs(h)));
  if ((w > 0 && h > 0) || (w < 0 && h < 0)) {
    rotateBy(angle);
  } else {
    rotateBy(-angle);
  }
  clearSelection();
}

void
ScanImage::paintEvent(QPaintEvent* event)
{
  if (!m_image.isNull()) {
    QLabel::paintEvent(event);
    QPainter painter(this);
    paintRubberBand(&painter);
    painter.end();
  }
}

void
ScanImage::enterEvent(QEvent*)
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
    m_mouse_moved = false;
    int e_x = event->x();
    int e_y = event->y();

    if (event->button() == Qt::LeftButton) {

      if (m_is_inside) {

        switch (m_state) {
          case EDGE_SELECTED:
            m_state = EDGE_STARTING;
            m_logger->info(tr("Edge draw pressed. (%1, %2)")
                             .arg(m_edge_start.x())
                             .arg(m_edge_start.y()));
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

            if ((e_x > left - 10 && e_x < left + 10) &&
                (e_y > bottom - 10 && e_y < bottom + 10)) {
              m_state = STRETCH_BOTTOMLEFT;
            } else if ((e_x > right - 10 && e_x < right + 10) &&
                       (e_y > top - 10 && e_y < top + 10)) {
              m_state = STRETCH_TOPRIGHT;
            } else if ((e_x > left - 10 && e_x < left + 10) &&
                       (e_y > top - 10 && e_y < top + 10)) {
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

void
ScanImage::mouseMoveEvent(QMouseEvent* event)
{
  if (!m_image.isNull()) {
    int e_x = event->x();
    int e_y = event->y();
    if ((qAbs(m_rubber_band.x() - e_x) > RUBBERBANDING_MIN &&
         qAbs(m_rubber_band.y()) > RUBBERBANDING_MIN) ||
        (qAbs(m_edge_start.x() - e_x) > RUBBERBANDING_MIN &&
         qAbs(m_edge_start.y()) > RUBBERBANDING_MIN)) {
      m_mouse_moved = true;
    }

    if (e_y < 0)
      e_y = 0;
    if (e_x < 0)
      e_x = 0;

    if (m_is_inside) {
      switch (m_state) {
        case EDGE_STARTING:
          if (m_mouse_moved) {
            m_state = EDGE_DRAWING;
          }
          break;
        case EDGE_DRAWING:
          m_logger->info(tr("Edge moving. (%1, %2)")
                           .arg(m_edge_finish.x())
                           .arg(m_edge_finish.y()));
          m_edge_finish = event->pos();
          break;

        case RUBBERBAND_STARTING:
          if (m_mouse_moved) {
            m_state = RUBBERBANDING;
          }
          [[fallthrough]];
        case RUBBERBANDING:
          m_rb_end_x = e_x;
          m_rb_end_y = e_y;
          m_rubber_band.setX(
            (m_rb_start_x < m_rb_end_x ? m_rb_start_x : m_rb_end_x));
          m_rubber_band.setWidth((m_rb_start_x < m_rb_end_x
                                    ? m_rb_end_x - m_rb_start_x
                                    : m_rb_start_x - m_rb_end_x));
          m_rubber_band.setY(
            (m_rb_start_y < m_rb_end_y ? m_rb_start_y : m_rb_end_y));
          m_rubber_band.setHeight((m_rb_start_y < m_rb_end_y
                                     ? m_rb_end_y - m_rb_start_y
                                     : m_rb_start_y - m_rb_end_y));
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

          if ((e_x > left - 10 && e_x < left + 10) &&
              (e_y > bottom - 10 && e_y < bottom + 10)) {
            setCursor(Qt::SizeBDiagCursor);
          } else if ((e_x > right - 10 && e_x < right + 10) &&
                     (e_y > top - 10 && e_y < top + 10)) {
            setCursor(Qt::SizeBDiagCursor);
          } else if ((e_x > left - 10 && e_x < left + 10) &&
                     (e_y > top - 10 && e_y < top + 10)) {
            setCursor(Qt::SizeFDiagCursor);
          } else if ((e_x < right + 10 && e_x > right - 10) &&
                     (e_y > bottom - 10 && e_y < bottom + 10)) {
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
          m_stretched_band.setHeight(m_rubber_band.height() -
                                     (e_y - m_rubber_band.y()));
          break;

        case STRETCH_BOTTOM:
          m_stretched_band.setHeight(e_y - m_rubber_band.y());
          break;

        case STRETCH_LEFT:
          m_stretched_band.setX(e_x);
          m_stretched_band.setWidth(m_rubber_band.width() -
                                    (e_x - m_rubber_band.x()));
          break;

        case STRETCH_RIGHT:
          m_stretched_band.setWidth(e_x - m_rubber_band.x());
          break;

        case STRETCH_TOPLEFT:
          m_stretched_band.setY(e_y);
          m_stretched_band.setHeight(m_rubber_band.height() -
                                     (e_y - m_rubber_band.y()));
          m_stretched_band.setX(e_x);
          m_stretched_band.setWidth(m_rubber_band.width() -
                                    (e_x - m_rubber_band.x()));
          break;

        case STRETCH_TOPRIGHT:
          m_stretched_band.setY(e_y);
          m_stretched_band.setHeight(m_rubber_band.height() -
                                     (e_y - m_rubber_band.y()));
          m_stretched_band.setWidth(e_x - m_rubber_band.x());
          break;

        case STRETCH_BOTTOMLEFT:
          m_stretched_band.setHeight(e_y - m_rubber_band.y());
          m_stretched_band.setX(e_x);
          m_stretched_band.setWidth(m_rubber_band.width() -
                                    (e_x - m_rubber_band.x()));
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

void
ScanImage::mouseReleaseEvent(QMouseEvent* event)
{
  if (!m_image.isNull()) {
    int e_x = event->x();
    int e_y = event->y();
    if ((qAbs(m_rubber_band.x() - e_x) > RUBBERBANDING_MIN &&
         qAbs(m_rubber_band.y()) > RUBBERBANDING_MIN) ||
        (qAbs(m_edge_start.x() - e_x) > RUBBERBANDING_MIN &&
         qAbs(m_edge_start.y()) > RUBBERBANDING_MIN)) {
      m_mouse_moved = false;
    }

    if (e_y < 0)
      e_y = 0;
    if (e_x < 0)
      e_x = 0;

    switch (m_state) {
      case EDGE_STARTING:
      case RUBBERBAND_STARTING:
        // mouse pressed and released without moving.
        clearSelection();
        emit unselected();
        disableSetDefaultCropSize();
        disableSelectionActions();
        enableNoSelectionActions();
        break;

      case EDGE_DRAWING:
        m_mouse_moved = false;
        m_edge_finish = event->pos();
        m_state = DOING_NOTHING;
        m_logger->info(tr("Edge draw released. (%1, %2)")
                         .arg(m_edge_finish.x())
                         .arg(m_edge_finish.y()));
        rotateUsingEdge();
        emit unselected();
        disableSetDefaultCropSize();
        disableSelectionActions();
        enableNoSelectionActions();
        break;

      case RUBBERBANDING: {
        m_rb_end_x = e_x;
        m_rb_end_y = e_y;
        m_rubber_band.setX(
          (m_rb_start_x < m_rb_end_x ? m_rb_start_x : m_rb_end_x));
        m_rubber_band.setWidth((m_rb_start_x < m_rb_end_x
                                  ? m_rb_end_x - m_rb_start_x
                                  : m_rb_start_x - m_rb_end_x));
        m_rubber_band.setY(
          (m_rb_start_y < m_rb_end_y ? m_rb_start_y : m_rb_end_y));
        m_rubber_band.setHeight((m_rb_start_y < m_rb_end_y
                                   ? m_rb_end_y - m_rb_start_y
                                   : m_rb_start_y - m_rb_end_y));
        m_stretched_band = m_rubber_band;
        m_logger->debug(tr("x:%1, y:%2, width:%3, height:%4")
                          .arg(m_rubber_band.x())
                          .arg(m_rubber_band.y())
                          .arg(m_rubber_band.width())
                          .arg(m_rubber_band.height()));
        m_state = RUBBERBAND_COMPLETE;
        emit selected();
        enableSetDefaultCropSize();
        enableSelectionActions();
        disableNoSelectionActions();
        update();
        break;
      } // end of RUBBER_BANDING

      case RUBBERBAND_MOVE:
        if (m_mouse_moved) {
          m_state = RUBBERBAND_COMPLETE;
          emit selected();
          enableSetDefaultCropSize();
          enableSelectionActions();
          disableNoSelectionActions();
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
        enableSetDefaultCropSize();
        enableSelectionActions();
        disableNoSelectionActions();
        m_state = RUBBERBAND_COMPLETE;
        update();
        break;

      default:
        m_rubber_band = m_stretched_band;
        clearSelection();
        emit selected();
        enableSetDefaultCropSize();
        enableSelectionActions();
        disableNoSelectionActions();
        update();
    } // end of switch.
  }
}

void
ScanImage::wheelEvent(QWheelEvent* event)
{
  int delta = event->delta();
  if (event->modifiers().testFlag(Qt::ControlModifier)) {
    // zoom in/out
    if (delta < 0)
      zoomOut();
    else
      zoomIn();
    update();
  } else {
    // scroll up/down
  }
}

void
ScanImage::zoomIn()
{
  qreal factor = m_scale_by * ZOOM_IN_FACTOR;
  m_logger->info(tr("ZoomIn scale %1 to %2").arg(m_scale_by).arg(factor));
  m_scale_by = factor;
  scaleImage(m_scale_by);
}

void
ScanImage::zoomOut()
{
  qreal factor = m_scale_by * ZOOM_OUT_FACTOR;
  m_logger->info(tr("ZoomIn scale %1 to %2").arg(m_scale_by).arg(factor));
  m_scale_by = factor;
  scaleImage(m_scale_by);
}

void
ScanImage::scaleImage(qreal factor)
{
  QSize size = m_image.size();
  size *= factor;
  resize(size);
  emit adjustScrollbar(factor);
}

void
ScanImage::initActions()
{
  m_copy_selection_act->setShortcut(QKeySequence::Copy);
  m_copy_selection_act->setToolTip(tr("Copies selection to clipboard."));
  connect(
    m_copy_selection_act, &QAction::triggered, this, &ScanImage::copySelection);
  m_crop_to_selection_act->setToolTip(
    tr("Crops the image to the selection rectangle."));
  connect(m_crop_to_selection_act,
          &QAction::triggered,
          this,
          &ScanImage::cropToSelection);
  m_clear_selection_act->setToolTip(tr("Removes selection rectangle."));
  connect(m_clear_selection_act,
          &QAction::triggered,
          this,
          &ScanImage::cropToSelection);
  connect(m_crop_to_content_act,
          &QAction::triggered,
          this,
          &ScanImage::cropToContent);
  connect(m_set_def_crop_act,
          &QAction::triggered,
          this,
          &ScanImage::setDefaultPageCropSize);
  connect(m_rotate_cw_act, &QAction::triggered, this, &ScanImage::rotateCW);
  connect(m_rotate_acw_act, &QAction::triggered, this, &ScanImage::rotateACW);
  connect(m_rotate_180_act, &QAction::triggered, this, &ScanImage::rotate180);
  connect(m_rotate_by_angle_act,
          &QAction::triggered,
          this,
          &ScanImage::rotateByAngle);
  connect(
    m_rotate_by_edge_act, &QAction::triggered, this, &ScanImage::rotateByEdge);
  connect(m_rescan_act, &QAction::triggered, this, &ScanImage::rescan);
  connect(m_scale_act, &QAction::triggered, this, &ScanImage::scale);
  connect(m_selectall_act, &QAction::triggered, this, &ScanImage::selectAll);
  connect(m_save_act, &QAction::triggered, this, &ScanImage::save);
  connect(m_save_as_act, &QAction::triggered, this, &ScanImage::saveAs);

  connect(m_split_pages_act, &QAction::triggered, this, &ScanImage::splitPages);

  disableSetDefaultCropSize();
  enableSelectionActions();
}

void
ScanImage::cropToContent()
{
  // TODO crop to content
}

void
ScanImage::rotate180()
{
  rotateBy(180);
}

void
ScanImage::rotateCW()
{
  rotateBy(90.0);
}

void
ScanImage::rotateACW()
{
  rotateBy(-90.0);
}

void
ScanImage::rotateByAngle()
{
  bool ok;
  qreal angle = QInputDialog::getDouble(
    this,
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

void
ScanImage::fitBest()
{
  QSize frame_size = frameSize();
  qreal scale_w = qreal(frame_size.width()) / qreal(m_image.width());
  qreal scale_h = qreal(frame_size.height()) / qreal(m_image.height());
  qreal factor = (scale_w < scale_h ? scale_w : scale_h);
  m_logger->info(tr("Fit Best scale %1 to %2").arg(m_scale_by).arg(factor));
  m_scale_by = factor;
  scaleImage(m_scale_by);
}

void
ScanImage::fitHeight()
{
  qreal factor = qreal(height()) / qreal(m_image.height());
  m_logger->info(tr("Fit Height scale %1 to %2").arg(m_scale_by).arg(factor));
  m_scale_by = factor;
  scaleImage(m_scale_by);
}

void
ScanImage::fitWidth()
{
  qreal factor = qreal(width()) / qreal(m_image.width());
  m_logger->info(tr("Fit Width scale %1 to %2").arg(m_scale_by).arg(factor));
  m_scale_by = factor;
  scaleImage(m_scale_by);
}

void
ScanImage::rescan()
{
  // TODO rescan
}

void
ScanImage::scale()
{
  // TODO scale
  scaleBy();
}
void
ScanImage::enableSetDefaultCropSize()
{
  m_set_def_crop_act->setEnabled(true);
}

void
ScanImage::disableSetDefaultCropSize()
{
  m_set_def_crop_act->setEnabled(false);
}

void
ScanImage::enableSelectionActions()
{
  m_crop_to_content_act->setEnabled(true);
  m_crop_to_selection_act->setEnabled(true);
  m_copy_selection_act->setEnabled(true);
}

void
ScanImage::disableSelectionActions()
{
  m_crop_to_content_act->setEnabled(false);
  m_crop_to_selection_act->setEnabled(false);
  m_copy_selection_act->setEnabled(false);
}

void
ScanImage::enableNoSelectionActions()
{
  m_rotate_cw_act->setEnabled(true);
  m_rotate_acw_act->setEnabled(true);
  m_rotate_180_act->setEnabled(true);
  m_rotate_by_edge_act->setEnabled(true);
  m_rotate_by_angle_act->setEnabled(true);
  m_scale_act->setEnabled(true);
}

void
ScanImage::disableNoSelectionActions()
{
  m_rotate_cw_act->setEnabled(false);
  m_rotate_acw_act->setEnabled(false);
  m_rotate_180_act->setEnabled(false);
  m_rotate_by_edge_act->setEnabled(false);
  m_rotate_by_angle_act->setEnabled(false);
  m_scale_act->setEnabled(false);
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
