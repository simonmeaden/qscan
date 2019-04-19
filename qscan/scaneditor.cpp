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
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  auto* layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
  m_image_display = new ScanImage(this);
  layout->addWidget(m_image_display);
  initActions();
}

ScanEditor::~ScanEditor() = default;

void
ScanEditor::initActions()
{
  //  m_cut_act = new QAction(tr("Cut selection"), this);
  //  m_cut_act->setShortcut(QKeySequence::Cut);
  //  connect(m_cut_act, &QAction::triggered, this, &ScanEditor::cut);

  m_copy_selection_act = new QAction(tr("Copy selection"), this);
  m_copy_selection_act->setShortcut(QKeySequence::Copy);
  connect(m_copy_selection_act, &QAction::triggered, this, &ScanEditor::copySelection);

  m_crop_to_selection_act = new QAction(tr("Crop to selection"), this);
  connect(m_crop_to_selection_act, &QAction::triggered, this, &ScanEditor::cropToSelection);

  m_crop_to_content_act = new QAction(tr("Crop to content"), this);
  connect(m_crop_to_content_act, &QAction::triggered, this, &ScanEditor::cropToContent);

  m_rotate_cw_act = new QAction(tr("Rotate 90° clockwise"), this);
  connect(m_rotate_cw_act, &QAction::triggered, this, &ScanEditor::rotateCW);

  m_rotate_acw_act = new QAction(tr("Rotate 90° anti-clockwise"), this);
  connect(m_rotate_acw_act, &QAction::triggered, this, &ScanEditor::rotateACW);

  m_rotate_180_act = new QAction(tr("Rotate 180°"), this);
  connect(m_rotate_180_act, &QAction::triggered, this, &ScanEditor::rotate180);

  m_rotate_act = new QAction(tr("Rotate by angle"), this);
  connect(m_rotate_act, &QAction::triggered, this, &ScanEditor::rotateByAngle);

  m_rescan_act = new QAction(tr("Re-scan image to crop"), this);
  connect(m_rescan_act, &QAction::triggered, this, &ScanEditor::rescan);

  m_scale_act = new QAction(tr("Scale image"), this);
  connect(m_scale_act, &QAction::triggered, this, &ScanEditor::scale);

  m_selectall_act = new QAction(tr("Select entire image"), this);
  connect(m_selectall_act, &QAction::triggered, this, &ScanEditor::selectAll);
}

void
ScanEditor::setImage(const QImage& image)
{
  m_prog_dlg->close();
  m_prog_dlg = nullptr;
  m_image_display->setImage(image);
}

void
ScanEditor::setScanProgress(const int& progress)
{
  m_prog_dlg->setValue(int(progress));
}

void
ScanEditor::scanningStarted()
{
  if (!m_prog_dlg) {
    m_prog_dlg = new QProgressDialog("Scanning Image", "Cancel Scanning", 0, 100, this);
    connect(m_prog_dlg, &QProgressDialog::canceled, this, &ScanEditor::scanCancelled);
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
    contextMenu->addAction(m_copy_selection_act);
    contextMenu->addSeparator();
    contextMenu->addAction(m_crop_to_selection_act);
    contextMenu->addSeparator();
    contextMenu->addAction(m_rescan_act);

  } else {
    contextMenu->addAction(m_selectall_act);
    contextMenu->addSeparator();
    contextMenu->addAction(m_crop_to_content_act);
    contextMenu->addSeparator();
    contextMenu->addAction(m_rotate_cw_act);
    contextMenu->addAction(m_rotate_acw_act);
    contextMenu->addAction(m_rotate_180_act);
    contextMenu->addAction(m_rotate_act);
    contextMenu->addSeparator();
    contextMenu->addAction(m_scale_act);
    contextMenu->addSeparator();
    contextMenu->addAction(m_rescan_act);
  }

  contextMenu->popup(event->globalPos());
}

void
ScanEditor::selectAll()
{
  m_image_display->selectAll();
}

void
ScanEditor::rotate180()
{
  m_image_display->rotateBy(180);
}

void
ScanEditor::rotateCW()
{
  m_image_display->rotateBy(90);
}

void
ScanEditor::rotateACW()
{
  m_image_display->rotateBy(-90);
}

void
ScanEditor::rotateByAngle()
{
  // TODO dialog for rotate angle.
  int angle = 0;
  m_image_display->rotateBy(angle);
}
//}

// void
// ScanEditor::cut()
//{}

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
ScanEditor::cropToContent()
{
  // TODO work out actual content.
}

void
ScanEditor::rescan()
{}

void
ScanEditor::scale()
{}
