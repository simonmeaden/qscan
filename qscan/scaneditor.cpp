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

ScanEditor::ScanEditor(QWidget* parent)
  : QFrame(parent),
    m_prog_dlg(nullptr)
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
#
  m_image = new QLabel(this);
  layout->addWidget(m_image);
}

ScanEditor::~ScanEditor() {}

void ScanEditor::setImage(const QImage& image)
{
  m_prog_dlg->close();
  m_prog_dlg = nullptr;
  int w = m_image->width();
  int h = m_image->height();
  QPixmap pixmap = QPixmap::fromImage(image).scaled(w, h, Qt::KeepAspectRatio);
  m_image->setPixmap(pixmap);
}

void ScanEditor::setScanProgress(const int& progress)
{
  m_prog_dlg->setValue(int(progress));
}

void ScanEditor::scanningStarted()
{
  if (!m_prog_dlg) {
    m_prog_dlg = new QProgressDialog("Scanning Image", "Cancel Scanning", 0, 100, this);
    connect(m_prog_dlg, &QProgressDialog::canceled, this, &ScanEditor::scanCancelled);
    m_prog_dlg->setWindowModality(Qt::WindowModal);
    m_prog_dlg->open();
  }
}
