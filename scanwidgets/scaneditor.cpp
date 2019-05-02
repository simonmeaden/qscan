#include <utility>

#include <utility>

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
#include "qscan.h"
#include "scaneditor.h"

#include "ocrdialog.h"
#include "ocrtools.h"

/* ScanEditor
 *****************************************************************************/

// ScanEditor::~ScanEditor(){
//}

void
ScanEditor::initGui()
{
  auto* layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  m_scroller = new QScrollArea(this);
  m_scroller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_scroller->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  m_scroller->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  m_scroller->setContentsMargins(0, 0, 0, 0);

  m_image_display = new ScanImage(m_datadir, this);
  m_image_display->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  m_image_display->setScaledContents(true);
  m_scroller->setWidget(m_image_display);
  layout->addWidget(m_scroller, 0, 0);

  m_page_view = new PageView(this);
  layout->addWidget(m_page_view, 0, 1);
  layout->setColumnStretch(0, 30);
  layout->setColumnStretch(1, 10);

  connect(
    m_image_display, &ScanImage::sendCover, this, &ScanEditor::saveAsCover);
}

void
ScanEditor::connectActions()
{
  connect(m_image_display,
          &ScanImage::selectionUnderway,
          this,
          &ScanEditor::selectionUnderway);
  connect(m_image_display, &ScanImage::selected, this, &ScanEditor::selected);
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
  connect(
    m_image_display, &ScanImage::sendImage, this, &ScanEditor::receiveImage);
  connect(
    m_image_display, &ScanImage::sendImages, this, &ScanEditor::receiveImages);
}

/*
   Receives a single page image from the ScanImage object
   and stores it.
*/
void
ScanEditor::receiveImage(const QImage& image)
{
  Page page(new ScanPage());
  page->setImage(image);
  int index = m_pages.size();
  m_pages.insert(index, page);
  m_page_view->append(m_pages.last()->thumbnail());
  saveImage(index, image);
}

/*
   Receives two split page images from the ScanImage object
   and stores them.
 */
void
ScanEditor::receiveImages(const QImage& left, const QImage& right)
{
  Page left_page(new ScanPage());
  left_page->setImage(left);
  int index = m_pages.size();
  m_pages.insert(index, left_page);
  m_page_view->append(m_pages.last()->thumbnail());
  saveImage(index, left);

  Page right_page(new ScanPage());
  right_page->setImage(right);
  index = m_pages.size();
  m_pages.insert(index, right_page);
  m_page_view->append(m_pages.last()->thumbnail());
  saveImage(index, right);
}

void
ScanEditor::receiveString(int page, const QString& str)
{
  m_pages.value(page)->setText(str);
}

void
ScanEditor::saveImage(int index, const QImage& image)
{
  QString path;
  if (m_document_name.isEmpty()) {
    QString doc_name =
      QInputDialog::getText(this,
                            tr("Get Document Name"),
                            tr("Please supply a name for this document.\n"
                               "Do not add file extension as this is\n"
                               "created internally."));
    if (!doc_name.isEmpty()) {
      m_document_name = doc_name;
      path = m_datadir + QDir::separator() + doc_name;
      QDir dir;
      dir.mkpath(path);
      path += QDir::separator() + QString("image%1.png").arg(index);
      if (!image.save(path, "PNG", 100)) {
        m_logger->info(tr("Failed to save image %1").arg(path));
      }
    }
  } else {
    path = m_datadir + QDir::separator() + m_document_name + QDir::separator() +
           QString("image%1.png").arg(index);
    if (!image.save(path, "PNG", 100)) {
      m_logger->info(tr("Failed to save image %1").arg(path));
    }
  }
}

void
ScanEditor::saveAsCover(const QImage& image)
{
  if (!m_document_name.isEmpty()) {
    // TODO no doc name yet.
  } else {
    // always
    m_cover->setImage(image);
    m_page_view->setCover(m_cover->thumbnail());
    QString path = m_datadir + QDir::separator() + m_document_name +
                   QDir::separator() + "cover.png";
    if (!image.save(path, "PNG", 100)) {
      m_logger->info(tr("failed to save file path"));
    }
  }
}

/*!
   \brief Splits a single page image into two equal size page images.
 */
void
ScanEditor::splitPages()
{
  QPair<QImage, QImage> images = m_image_display->splitPages();
  receiveImages(images.first, images.second);
}

/*!
   \brief Splits a single page image into two equal size page images but
   discards the right hand page.
 */
void
ScanEditor::splitLeftPage()
{
  QImage image = m_image_display->splitLeftPage();
  receiveImage(image);
}

/*!
   \brief Splits a single page image into two equal size page images but
   discards the left hand page.
 */
void
ScanEditor::splitRightPage()
{
  QImage image = m_image_display->splitRightPage();
  receiveImage(image);
}

void
ScanEditor::makePage()
{
  QImage image = m_image_display->makePage();
  Page page(new ScanPage());
  page->setImage(image);
  int index = m_pages.size();
  m_pages.insert(index, page);
  m_page_view->append(page->thumbnail());
}

void
ScanEditor::receiveOcrPage(int index)
{
  Page page = m_pages.value(index);
  auto* dlg = new OCRDialog(this);
  dlg->setImage(page->image());
  if (!page->text().isEmpty()) {
    dlg->receiveOcrText(page->text());
  }
  if (dlg->exec() == QDialog::Accepted) {
    page->setText(dlg->text());
    if (dlg->imageChanged()) {
      // TODO maybe save tweaked image. maybe only supply list of tweaks?
    }
  }
}

void
ScanEditor::loadCover(Page cover)
{
  m_cover = std::move(cover);
  m_page_view->setCover(m_cover->thumbnail());
}

void
ScanEditor::loadImage(int index, const Page& page)
{
  m_page_view->insert(index, page->thumbnail());
  m_pages.insert(index, page);
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
ScanEditor::setDocumentName(const QString& name)
{
  m_document_name = name;
}

QString
ScanEditor::documentName() const
{
  return m_document_name;
}

void
ScanEditor::setSelectedName(const QString& selected_name)
{
  m_selected_name = selected_name;
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
  QScrollBar* scrollbar = m_scroller->horizontalScrollBar();
  scrollbar->setValue(int(factor * scrollbar->value() +
                          ((factor - 1) * scrollbar->pageStep() / 2)));
  scrollbar = m_scroller->verticalScrollBar();
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
  m_image_display->rotate180();
}

void
ScanEditor::rotateCW()
{
  m_image_display->rotateCW();
}

void
ScanEditor::rotateACW()
{
  m_image_display->rotateACW();
}

void
ScanEditor::rotateByAngle()
{
  m_image_display->rotateByAngle();
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
  m_image_display->cropToContent();
}

void
ScanEditor::rescan()
{
  m_image_display->rescan();
}

void
ScanEditor::scale()
{
  m_image_display->scale();
}

void
ScanEditor::save()
{
  //  if (m_document_name.isEmpty()) {
  //    QString doc_name =
  //      QInputDialog::getText(this,
  //                            tr("Get Document Name"),
  //                            tr("Please supply a name for this document.\n"
  //                               "Do not add file extension as this is\n"
  //                               "created internally."));
  //    if (!doc_name.isEmpty()) {
  //      m_document_name = doc_name;
  //      QString path = m_datadir + QDir::separator() + doc_name;
  //      QDir dir;
  //      dir.mkpath(path);
  //      for (int i = 0; i < m_pages.size();i++) {
  //        Page page = m_pages.at(i);
  //      }
  //      return;
  //    }
  //  }
  m_image_display->save();
  // TODO save text.
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

int
ScanEditor::pageCount()
{
  return m_pages.size();
}

Page
ScanEditor::page(int index)
{
  return m_pages.value(index);
}
