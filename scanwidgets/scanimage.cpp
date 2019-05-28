#include <utility>

#include "scanimage.h"
#include "scaneditor.h"

/* ScanImage
 *****************************************************************************/

ScanImage::ScanImage(QString  datadir, QWidget* parent)
  : BaseScanImage(parent)
  , m_datadir(std::move(datadir))
  , m_rotation(0)
{
  initActions();
  setMouseTracking(true);
  connect(this, &ScanImage::unselected, this, &ScanImage::disableSetDefaultCropSize);
  connect(this, &ScanImage::unselected, this, &ScanImage::disableSelectionActions);
  connect(this, &ScanImage::unselected, this, &ScanImage::enableNoSelectionActions);
  connect(this, &ScanImage::selected, this, &ScanImage::enableSetDefaultCropSize);
  connect(this, &ScanImage::selected, this, &ScanImage::enableSelectionActions);
  connect(this, &ScanImage::selected, this, &ScanImage::disableNoSelectionActions);
}

void ScanImage::rotateByEdge()
{
  m_state = EDGE_SELECTED;
}

void ScanImage::scaleBy()
{
  // TODO scale by.
}

void ScanImage::save()
{
  if (m_filename.isEmpty()) {
    saveAs();

  } else {
    m_image.save(m_filename);
  }
}

void ScanImage::saveAs()
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

void ScanImage::saveAsCover()
{
  emit sendCover(m_image);
}


QRect ScanImage::selection()
{
  if (hasSelection()) {
    return m_rubber_band;
  }

  return {};
}

/*!
   \brief Copy selection to clipboard.
*/
void ScanImage::copySelection()
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

void ScanImage::selectAll()
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


void ScanImage::setDefaultPageCropSize()
{
  m_default_crop_size = m_rubber_band;
  m_def_crop_set = true;
}

QPair<QImage, QImage> ScanImage::splitPages()
{
  int w = m_modified_image.width();
  int w2 = int(w / 2.0);

  m_logger->info(tr("Copying to clipboard"));
  QRect part_rect;
  part_rect.setX(0);
  part_rect.setY(0);
  part_rect.setWidth(w2);
  part_rect.setHeight(m_image.height());

  QImage left = m_modified_image.copy(part_rect);

  part_rect.setX(w2 + 1);
  part_rect.setWidth(w - w2);

  QImage right = m_modified_image.copy(part_rect);

  emit sendImages(left, right);

  return qMakePair<QImage, QImage>(left, right);
}

QImage ScanImage::splitLeftPage()
{
  int w2 = int(m_modified_image.width() / 2.0);

  m_logger->info(tr("Copying to clipboard"));
  QRect part_rect;
  part_rect.setX(0);
  part_rect.setY(0);
  part_rect.setWidth(w2);
  part_rect.setHeight(m_modified_image.height());

  QImage left = m_modified_image.copy(part_rect);

  emit sendImage(left);

  return left;
}

QImage ScanImage::splitRightPage()
{
  int w = m_modified_image.width();
  int w2 = int(w / 2.0);

  m_logger->info(tr("Copying to clipboard"));
  QRect part_rect;
  part_rect.setX(w2 + 1);
  part_rect.setY(0);
  part_rect.setWidth(w - w2);
  part_rect.setHeight(m_modified_image.height());

  QImage right = m_modified_image.copy(part_rect);

  emit sendImage(right);

  return right;
}

QImage ScanImage::makePage()
{
  emit sendImage(m_modified_image);
  return m_modified_image;
}

void ScanImage::contextMenuEvent(QContextMenuEvent* event)
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
    context_menu->addAction(m_save_as_cover_act);
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


void ScanImage::initActions()
{
  m_copy_selection_act = new QAction(tr("Copy selection"), this);
  m_crop_to_selection_act = new QAction(tr("Crop to selection"), this);
  m_clear_selection_act = new QAction(tr("Clear selection"), this);
  m_crop_to_content_act = new QAction(tr("Crop to content"), this);
  m_rotate_cw_act = new QAction(tr("Rotate 90° clockwise"), this);
  m_rotate_acw_act = new QAction(tr("Rotate 90° anti-clockwise"), this);
  m_rotate_180_act = new QAction(tr("Rotate 180°"), this);
  m_rotate_by_angle_act = new QAction(tr("Rotate by angle"), this);
  m_rotate_by_edge_act = new QAction(tr("Rotate by edge"), this);
  m_rescan_act = new QAction(tr("Re-scan image to crop"), this);
  m_scale_act = new QAction(tr("Scale image"), this);
  m_selectall_act = new QAction(tr("Select entire image"), this);
  m_save_act = new QAction(tr("Save image"), this);
  m_save_as_act = new QAction(tr("Save image as"), this);
  m_save_as_cover_act = new QAction(tr("Save image as cover"), this);
  m_set_def_crop_act = new QAction(tr("Set default page crop size"), this);
  //  m_select_all(false)
  m_split_pages_act = new QAction(tr("Split image into two pages"), this);
  m_split_right_act =
    new QAction(tr("Make right half into a single page"), this);
  m_split_left_act = new QAction(tr("Make left half into a single page"), this);
  m_make_page_act = new QAction(tr("Make image into a single page"), this);

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
  connect(
    m_save_as_cover_act, &QAction::triggered, this, &ScanImage::saveAsCover);

  connect(m_split_pages_act, &QAction::triggered, this, &ScanImage::splitPages);
  connect(
    m_split_right_act, &QAction::triggered, this, &ScanImage::splitRightPage);
  connect(
    m_split_left_act, &QAction::triggered, this, &ScanImage::splitLeftPage);
  connect(m_make_page_act, &QAction::triggered, this, &ScanImage::makePage);

  disableSetDefaultCropSize();
  enableSelectionActions();
}

void ScanImage::cropToContent()
{
  // TODO crop to content
}

void ScanImage::rotate180()
{
  rotateBy(180);
}

void ScanImage::rotateCW()
{
  rotateBy(90.0);
}

void ScanImage::rotateACW()
{
  rotateBy(-90.0);
}

void ScanImage::rotateByAngle()
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

void ScanImage::rescan()
{
  // TODO rescan
}

void ScanImage::scale()
{
  // TODO scale
  scaleBy();
}
void ScanImage::enableSetDefaultCropSize()
{
  m_set_def_crop_act->setEnabled(true);
}

void ScanImage::disableSetDefaultCropSize()
{
  m_set_def_crop_act->setEnabled(false);
}

void ScanImage::enableSelectionActions()
{
  m_crop_to_content_act->setEnabled(true);
  m_crop_to_selection_act->setEnabled(true);
  m_copy_selection_act->setEnabled(true);
}

void ScanImage::disableSelectionActions()
{
  m_crop_to_content_act->setEnabled(false);
  m_crop_to_selection_act->setEnabled(false);
  m_copy_selection_act->setEnabled(false);
}

void ScanImage::enableNoSelectionActions()
{
  m_rotate_cw_act->setEnabled(true);
  m_rotate_acw_act->setEnabled(true);
  m_rotate_180_act->setEnabled(true);
  m_rotate_by_edge_act->setEnabled(true);
  m_rotate_by_angle_act->setEnabled(true);
  m_scale_act->setEnabled(true);
}

void ScanImage::disableNoSelectionActions()
{
  m_rotate_cw_act->setEnabled(false);
  m_rotate_acw_act->setEnabled(false);
  m_rotate_180_act->setEnabled(false);
  m_rotate_by_edge_act->setEnabled(false);
  m_rotate_by_angle_act->setEnabled(false);
  m_scale_act->setEnabled(false);
}

QSize ScanImage::minimumSizeHint() const
{
  return { 182, 182 };
}

QSize ScanImage::sizeHint() const
{
  return { 232, 232 };
}
