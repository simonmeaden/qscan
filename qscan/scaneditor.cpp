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
#include "scaneditor.h"
#include "qscan.h"

/* ScanEditor
 *****************************************************************************/
ScanEditor::ScanEditor(QScan* scan, QWidget* parent)
  : QFrame(parent)
  , m_image_display(nullptr)
  , m_prog_dlg(nullptr)
  , m_scan_lib(scan)
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
  , m_select_all(false)
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  auto* layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  initActions();

  scroll = new QScrollArea(this);
  scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scroll->setContentsMargins(0, 0, 0, 0);

  //  auto* scroll_layout = new QHBoxLayout;
  //  scroll->setLayout(scroll_layout);
  //  scroll_layout->setContentsMargins(0, 0, 0, 0);

  m_image_display = new ScanImage(this);
  m_image_display->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  m_image_display->setScaledContents(true);
  scroll->setWidget(m_image_display);
  layout->addWidget(scroll);

  connect(m_image_display,
          &ScanImage::selectionUnderway,
          this,
          &ScanEditor::selectionUnderway);
  connect(m_image_display, &ScanImage::selected, this, &ScanEditor::selected);
  connect(m_image_display,
          &ScanImage::selected,
          this,
          &ScanEditor::enableSetDefaultCropSize);
  connect(m_image_display,
          &ScanImage::unselected,
          this,
          &ScanEditor::disableSetDefaultCropSize);
  connect(
    m_image_display, &ScanImage::unselected, this, &ScanEditor::unselected);
  connect(m_image_display,
          &ScanImage::imageIsLoaded,
          this,
          &ScanEditor::imageIsLoaded);
  connect(m_image_display,
          &ScanImage::adjustScrollbar,
          this,
          &ScanEditor::adjustScrollbar);
}

ScanEditor::~ScanEditor() = default;

void
ScanEditor::initActions()
{
  m_copy_selection_act->setShortcut(QKeySequence::Copy);
  m_copy_selection_act->setToolTip(tr("Copies selection to clipboard."));
  connect(m_copy_selection_act,
          &QAction::triggered,
          this,
          &ScanEditor::copySelection);
  m_crop_to_selection_act->setToolTip(
    tr("Crops the image to the selection rectangle."));
  connect(m_crop_to_selection_act,
          &QAction::triggered,
          this,
          &ScanEditor::cropToSelection);
  m_clear_selection_act->setToolTip(tr("Removes selection rectangle."));
  connect(m_clear_selection_act,
          &QAction::triggered,
          this,
          &ScanEditor::cropToSelection);
  connect(m_crop_to_content_act,
          &QAction::triggered,
          this,
          &ScanEditor::cropToContent);
  connect(m_set_def_crop_act,
          &QAction::triggered,
          this,
          &ScanEditor::setDefaultPageCropSize);
  connect(m_rotate_cw_act, &QAction::triggered, this, &ScanEditor::rotateCW);
  connect(m_rotate_acw_act, &QAction::triggered, this, &ScanEditor::rotateACW);
  connect(m_rotate_180_act, &QAction::triggered, this, &ScanEditor::rotate180);
  connect(m_rotate_by_angle_act,
          &QAction::triggered,
          this,
          &ScanEditor::rotateByAngle);
  connect(
    m_rotate_by_edge_act, &QAction::triggered, this, &ScanEditor::rotateByEdge);
  connect(m_rescan_act, &QAction::triggered, this, &ScanEditor::rescan);
  connect(m_scale_act, &QAction::triggered, this, &ScanEditor::scale);
  connect(m_selectall_act, &QAction::triggered, this, &ScanEditor::selectAll);
  connect(m_save_act, &QAction::triggered, this, &ScanEditor::save);
  connect(m_save_as_act, &QAction::triggered, this, &ScanEditor::saveAs);

  disableSetDefaultCropSize();
}

void
ScanEditor::enableSetDefaultCropSize()
{
  m_set_def_crop_act->setEnabled(true);
}

void
ScanEditor::disableSetDefaultCropSize()
{
  m_set_def_crop_act->setEnabled(false);
}

void
ScanEditor::setImage(const QImage& image)
{
  if (m_prog_dlg) {
    m_prog_dlg->close();
    m_prog_dlg = nullptr;
  }
  m_image_display->setImage(image);
}

void
ScanEditor::setScanProgress(const int& progress)
{
  if (m_prog_dlg) {
    m_prog_dlg->setValue(int(progress));
  }
}

void
ScanEditor::scanningStarted()
{
  if (!m_prog_dlg) {
    m_prog_dlg =
      new QProgressDialog("Scanning Image", "Cancel Scanning", 0, 100, this);
    connect(
      m_prog_dlg, &QProgressDialog::canceled, this, &ScanEditor::scanCancelled);
    m_prog_dlg->setWindowModality(Qt::WindowModal);
    m_prog_dlg->open();
  }
}

void
ScanEditor::setSelectedName(const QString& selected_name)
{
  m_selected_name = selected_name;
}

void
ScanEditor::contextMenuEvent(QContextMenuEvent* event)
{
  auto* contextMenu = new QMenu();

  if (m_image_display->hasSelection()) {
    contextMenu->addAction(m_save_act);
    contextMenu->addAction(m_save_as_act);
    contextMenu->addSeparator();
    contextMenu->addAction(m_copy_selection_act);
    contextMenu->addAction(m_crop_to_selection_act);
    contextMenu->addAction(m_clear_selection_act);
    contextMenu->addSeparator();
    contextMenu->addAction(m_rescan_act);
    contextMenu->addSeparator();
    contextMenu->addAction(m_set_def_crop_act);

  } else {
    contextMenu->addAction(m_save_act);
    contextMenu->addAction(m_save_as_act);
    contextMenu->addSeparator();
    contextMenu->addAction(m_selectall_act);
    contextMenu->addSeparator();
    contextMenu->addAction(m_crop_to_content_act);
    contextMenu->addSeparator();
    contextMenu->addAction(m_rotate_cw_act);
    contextMenu->addAction(m_rotate_acw_act);
    contextMenu->addAction(m_rotate_180_act);
    contextMenu->addAction(m_rotate_by_angle_act);
    contextMenu->addAction(m_rotate_by_edge_act);
    contextMenu->addSeparator();
    contextMenu->addAction(m_scale_act);
    contextMenu->addSeparator();
    contextMenu->addAction(m_rescan_act);
  }

  contextMenu->popup(event->globalPos());
}

bool
ScanEditor::eventFilter(QObject* obj, QEvent* event)
{
  bool result;
  if (event->type() == QEvent::KeyPress) {
    auto* key_event = dynamic_cast<QKeyEvent*>(event);
    if (key_event) {
      bool is_keypad = ((key_event->modifiers()) & Qt::KeypadModifier);
      int key = key_event->key();
      switch (key_event->key()) {
        case Qt::Key_Escape:
          clearSelection();
          break;
        case Qt::Key_Clear:
          clearSelection();
          break;
        case Qt::Key_Plus:
          zoomIn();
          break;
        case Qt::Key_Minus:
          zoomOut();
          break;
      }
    }
    result = true;
  } else if (event->type() == QEvent::Show || event->type() == QEvent::Resize) {
    m_image_display->resize(frameRect().size());
    result = true;
  } else {
    result = QObject::eventFilter(obj, event);
  }
  return result;
}

void
ScanEditor::adjustScrollbar(qreal factor)
{
  QScrollBar* scrollbar = scroll->horizontalScrollBar();
  scrollbar->setValue(int(factor * scrollbar->value() +
                          ((factor - 1) * scrollbar->pageStep() / 2)));
  scrollbar = scroll->verticalScrollBar();
  scrollbar->setValue(int(factor * scrollbar->value() +
                          ((factor - 1) * scrollbar->pageStep() / 2)));
}

void
ScanEditor::selectAll()
{
  m_image_display->selectAll();
}

void
ScanEditor::rotate180()
{
  m_image_display->rotateBy(180.0);
}

void
ScanEditor::rotateCW()
{
  m_image_display->rotateBy(90.0);
}

void
ScanEditor::rotateACW()
{
  m_image_display->rotateBy(-90.0);
}

void
ScanEditor::rotateByAngle()
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
    m_image_display->rotateBy(angle);
  }
}

void
ScanEditor::rotateByEdge()
{
  m_image_display->rotateByEdge();
}

void
ScanEditor::copySelection()
{
  m_image_display->copySelection();
}

void
ScanEditor::cropToSelection()
{
  m_image_display->cropToSelection();
}

void
ScanEditor::clearSelection()
{
  m_image_display->clearSelection();
}

void
ScanEditor::cropToContent()
{
  // TODO work out actual content.
}

void
ScanEditor::rescan()
{}

void
ScanEditor::scale()
{
  m_image_display->scaleBy();
}

void
ScanEditor::save()
{
  m_image_display->save();
}

void
ScanEditor::saveAs()
{
  m_image_display, saveAs();
}

void
ScanEditor::zoomIn()
{
  m_image_display->zoomIn();
}

void
ScanEditor::zoomOut()
{
  m_image_display->zoomOut();
}

void
ScanEditor::fitBest()
{
  m_image_display->fitBest();
}

void
ScanEditor::fitHeight()
{
  m_image_display->fitHeight();
}

void
ScanEditor::fitWidth()
{
  m_image_display->fitWidth();
}

void
ScanEditor::setDefaultPageCropSize()
{
  m_image_display->setDefaultPageCropSize();
}
