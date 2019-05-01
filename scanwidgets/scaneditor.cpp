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

#include "ocrtools.h"

/* ScanEditor
 *****************************************************************************/

ScanEditor::ScanEditor(QScan* scan,
                       QString datapath,
                       QString lang,
                       QWidget* parent)
  : QFrame(parent)
  , m_image_display(nullptr)
  , m_prog_dlg(nullptr)
  , m_scan_lib(scan)
  , m_page_view(nullptr)
  , scroll(nullptr)
  , m_ocr_tools(new OcrTools(datapath, lang, this))
{
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  initGui();
  connectActions();
}

// ScanEditor::~ScanEditor(){
//}

void
ScanEditor::initGui()
{
  auto* layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  scroll = new QScrollArea(this);
  scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  scroll->setContentsMargins(0, 0, 0, 0);

  m_image_display = new ScanImage(this);
  m_image_display->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  m_image_display->setScaledContents(true);
  scroll->setWidget(m_image_display);
  layout->addWidget(scroll, 0, 0);

  m_page_view = new PageView(this);
  layout->addWidget(m_page_view, 0, 1);
  layout->setColumnStretch(0, 30);
  layout->setColumnStretch(1, 10);
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
ScanEditor::receiveImage(const QImage& img)
{
  Page page(new ScanPage());
  page->setImage(img);
  m_pages.append(page);
  m_page_view->append(m_pages.last()->thumbnail());
  m_ocr_tools->convertImage(m_pages.size() - 1, img);
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
  m_pages.append(left_page);
  m_page_view->append(m_pages.last()->thumbnail());
  m_ocr_tools->convertImage(m_pages.size() - 1, left);
  Page right_page(new ScanPage());
  right_page->setImage(right);
  m_pages.append(right_page);
  m_page_view->append(m_pages.last()->thumbnail());
  m_ocr_tools->convertImage(m_pages.size() - 1, right);
}

void
ScanEditor::receiveString(int page, const QString& str)
{
  m_pages.at(page)->setText(str);
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
  m_pages.append(page);
  m_page_view->append(m_pages.last()->thumbnail());
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

int
ScanEditor::pageCount()
{
  return m_pages.size();
}

Page
ScanEditor::page(int index)
{
  return m_pages.at(index);
}
