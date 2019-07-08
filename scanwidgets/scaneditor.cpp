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
#include <utility>

//#include <qyaml-cpp/qyaml-cpp.h>
#include "QYamlCpp"
#include <yaml-cpp/yaml.h>

#include "documentdata.h"
#include "qscan.h"
#include "scaneditor.h"

#include "ocrframe.h"
#include "ocrtools.h"

/* ScanEditor
 *****************************************************************************/
/*!
  \class ScanEditor.

*/

const QString ScanEditor::OCR_IMAGE_NAME = QStringLiteral("ocrimage%1.png");
const QString ScanEditor::INTERNAL_IMAGE_NAME = QStringLiteral("docimage%1_%2.png");
const QString ScanEditor::OCR_IMAGE_FILTER = QStringLiteral("ocrimage*.png");
const QString ScanEditor::INTERNAL_IMAGE_FILTER = QStringLiteral("docimage*.png");

const QString ScanEditor::OPTIONS_FILE = QStringLiteral("scanoptions.yaml");
const QString ScanEditor::CURRENT_DOCUMENT = QStringLiteral("current document");
const QString ScanEditor::TESSERACT = "tesseract";
const QString ScanEditor::LANGUAGE = "language";

/*!
   \brief Constructor for the ScanEditor class.

   \param scan - a pointer to the QScan library.
   \param configdir - the directory for configuration files.
   \param datadir - the base directory for image and text files.
   \param lang - the tesseract language.
   \param parent - the parent QObject.
*/
ScanEditor::ScanEditor(QScan* scan,
                       const QString& configdir,
                       const QString& datadir,
                       QWidget* parent)
  : QFrame(parent)
  , m_scan_lib(scan)
  , m_config_dir(configdir)
  , m_data_dir(datadir)
  , m_lang("eng") // default language is english
{
  m_logger = Log4Qt::Logger::logger(tr("ScanEditor"));

  crop_key = QPixmapCache::insert(QPixmap(":/qscan_icons/crop"));
  move_up_key = QPixmapCache::insert(QPixmap(":/qscan_icons/up"));
  move_down_key = QPixmapCache::insert(QPixmap(":/qscan_icons/down"));
  move_to_key = QPixmapCache::insert(QPixmap(":/qscan_icons/move_to_edit"));
  //  move_to_internal_key = QPixmapCache::insert(QPixmap(":/qscan_icons/move_to_images"));
  cover_key = QPixmapCache::insert(QPixmap(":/qscan_icons/cover64"));
  left_key = QPixmapCache::insert(QPixmap(":/qscan_icons/left64"));
  right_key = QPixmapCache::insert(QPixmap(":/qscan_icons/right64"));
  single_key = QPixmapCache::insert(QPixmap(":/qscan_icons/single64"));
  both_key = QPixmapCache::insert(QPixmap(":/qscan_icons/both64"));
  scale_key = QPixmapCache::insert(QPixmap(":/qscan_icons/scale"));
  remove_text_key = QPixmapCache::insert(QPixmap(":/qscan_icons/del_text"));
  remove_image_key = QPixmapCache::insert(QPixmap(":/qscan_icons/del_image"));
  remove_both_key = QPixmapCache::insert(QPixmap(":/qscan_icons/del_image_text"));

  m_options_file = m_config_dir + QDir::separator() + OPTIONS_FILE;

  QString datapath = configdir + QStringLiteral("/tesseract/tessdata");
  m_ocr_tools = new OcrTools(datapath, m_lang);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  initGui();
  makeConnections();
}

ScanEditor::~ScanEditor()
{
  cleanupImages();
  saveDataStore();
}

void ScanEditor::initPageListBtns()
{
  QPixmap rem_img_icon, rem_txt_icon, rem_img_txt_icon;
  QPixmap move_up_icon, move_down_icon, move_to_icon;
  QPixmapCache::find(remove_image_key, &rem_img_icon);
  QPixmapCache::find(remove_text_key, &rem_txt_icon);
  QPixmapCache::find(remove_both_key, &rem_img_txt_icon);
  QPixmapCache::find(move_up_key, &move_up_icon);
  QPixmapCache::find(move_down_key, &move_down_icon);
  QPixmapCache::find(move_to_key, &move_to_icon);

  page_list_group = new QGroupBox(tr("Edit Document"), this);
  page_list_group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  auto* group2_layout = new QVBoxLayout;
  page_list_group->setLayout(group2_layout);

  m_move_up_btn = new QPushButton(move_up_icon, "", this);
  m_move_up_btn->setIconSize(QSize(96, 32));
  m_move_up_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_move_up_btn->setToolTip(tr("Move selection up one position"));
  group2_layout->addWidget(m_move_up_btn);
  connect(m_move_up_btn, &QPushButton::clicked, m_page_view, &PageView::moveUp);

  m_move_down_btn = new QPushButton(move_down_icon, "", this);
  m_move_down_btn->setIconSize(QSize(96, 32));
  m_move_down_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_move_down_btn->setToolTip(tr("Move selection down one position"));
  group2_layout->addWidget(m_move_down_btn);
  connect(m_move_down_btn, &QPushButton::clicked, m_page_view, &PageView::moveDown);

  m_move_to_btn = new QPushButton(move_to_icon, "", this);
  m_move_to_btn->setIconSize(QSize(96, 32));
  m_move_to_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_move_to_btn->setToolTip(tr("Work with selection"));
  group2_layout->addWidget(m_move_to_btn);
  connect(m_move_to_btn, &QPushButton::clicked, m_page_view, &PageView::workOnImage);

  m_rem_txt_btn = new QPushButton(rem_txt_icon, "", this);
  m_rem_txt_btn->setIconSize(QSize(96, 32));
  m_rem_txt_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_rem_txt_btn->setToolTip(tr("Remove the text"));
  group2_layout->addWidget(m_rem_txt_btn);
  connect(m_rem_txt_btn, &QPushButton::clicked, this, &ScanEditor::removeText);

  m_rem_img_btn = new QPushButton(rem_img_icon, "", this);
  m_rem_img_btn->setIconSize(QSize(96, 32));
  m_rem_img_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_rem_img_btn->setToolTip(tr("Remove the image"));
  group2_layout->addWidget(m_rem_img_btn);
  connect(m_rem_img_btn, &QPushButton::clicked, this, &ScanEditor::removeImage);

  m_rem_both_btn = new QPushButton(rem_img_txt_icon, "", this);
  m_rem_both_btn->setIconSize(QSize(96, 32));
  m_rem_both_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_rem_both_btn->setToolTip(tr("Remove the both the image and the text"));
  group2_layout->addWidget(m_rem_both_btn);
  connect(m_rem_both_btn, &QPushButton::clicked, this, &ScanEditor::removeText);
}

void ScanEditor::initImageEditBtns()
{
  QPixmap crop_icon;
  QPixmapCache::find(crop_key, &crop_icon);

  image_edit_group = new QGroupBox(tr("Edit Image"), this);
  image_edit_group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  auto* group1_layout = new QVBoxLayout;
  image_edit_group->setLayout(group1_layout);

  m_crop_btn = new QPushButton(crop_icon, "", this);
  m_crop_btn->setIconSize(QSize(96, 32));
  m_crop_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_crop_btn->setToolTip(tr("Crop image to selection"));
  group1_layout->addWidget(m_crop_btn);
  connect(m_crop_btn, &QPushButton::clicked, this, &ScanEditor::cropToSelection);
}

void ScanEditor::initTransferBtns()
{
  QPixmap left_icon, right_icon, single_icon, both_icon, cover_icon;
  QPixmap scale_icon /*, move_to_internal_icon*/;
  QPixmapCache::find(cover_key, &cover_icon);
  QPixmapCache::find(left_key, &left_icon);
  QPixmapCache::find(right_key, &right_icon);
  QPixmapCache::find(single_key, &single_icon);
  QPixmapCache::find(both_key, &both_icon);
  QPixmapCache::find(scale_key, &scale_icon);
  //  QPixmapCache::find(move_to_internal_key, &move_to_internal_icon);

  image_transfer_group = new QGroupBox(tr("Transfer Image"), this);
  image_transfer_group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  auto* group3_layout = new QVBoxLayout;
  image_transfer_group->setLayout(group3_layout);

  m_cover_btn = new QPushButton(cover_icon, "", this);
  m_cover_btn->setIconSize(QSize(96, 32));
  m_cover_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_cover_btn->setToolTip(tr("Move image to cover"));
  group3_layout->addWidget(m_cover_btn);
  connect(m_cover_btn, &QPushButton::clicked, this, &ScanEditor::makeCover);

  m_left_btn = new QPushButton(left_icon, "", this);
  m_left_btn->setIconSize(QSize(96, 32));
  m_left_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_left_btn->setToolTip(tr("Move left image to page list"));
  group3_layout->addWidget(m_left_btn);
  connect(m_left_btn, &QPushButton::clicked, this, &ScanEditor::moveLeftPageToDocument);

  m_right_btn = new QPushButton(right_icon, "", this);
  m_right_btn->setIconSize(QSize(96, 32));
  m_right_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_right_btn->setToolTip(tr("Move right image to page list"));
  group3_layout->addWidget(m_right_btn);
  connect(m_right_btn, &QPushButton::clicked, this, &ScanEditor::moveRightPageToDocument);

  m_single_btn = new QPushButton(single_icon, "", this);
  m_single_btn->setIconSize(QSize(96, 32));
  m_single_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_single_btn->setToolTip(tr("Move image as single page to list list"));
  group3_layout->addWidget(m_single_btn);
  connect(m_single_btn, &QPushButton::clicked, this, &ScanEditor::movePageToDocument);

  m_both_btn = new QPushButton(both_icon, "", this);
  m_both_btn->setIconSize(QSize(96, 32));
  m_both_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_both_btn->setToolTip(tr("Move both images to page list"));
  group3_layout->addWidget(m_both_btn);
  connect(m_both_btn, &QPushButton::clicked, this, &ScanEditor::moveBothPagesToDocument);

  //  m_move_to_doc_btn = new QPushButton(move_to_internal_icon, "", this);
  //  m_move_to_doc_btn->setIconSize(QSize(96, 32));
  //  m_move_to_doc_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  //  m_move_to_doc_btn->setToolTip(tr("Move Image to Document Images"));
  //  group3_layout->addWidget(m_move_to_doc_btn);
  //  connect(m_move_to_doc_btn,
  //          &QPushButton::clicked,
  //          this,
  //          qOverload<>(&ScanEditor::saveDocumentImage));
}

void ScanEditor::initGui()
{
  m_scan_layout = new QStackedLayout;
  setLayout(m_scan_layout);

  QFrame* main_frame = new QFrame(this);
  auto* grid_layout = new QGridLayout(this);
  grid_layout->setContentsMargins(0, 0, 0, 0);
  main_frame->setLayout(grid_layout);

  m_stack_id = m_scan_layout->addWidget(main_frame);

  m_ocr_frame = new OcrFrame(this);
  connect(m_ocr_frame, &OcrFrame::saveModifiedImage, this, &ScanEditor::saveModifiedImage);
  connect(m_ocr_frame, &OcrFrame::saveModifiedData, this, &ScanEditor::saveModifiedData);
  connect(m_ocr_frame, &OcrFrame::sendOcrRequest, this, &ScanEditor::receiveOcrImageRequest);
  connect(m_ocr_frame, &OcrFrame::saveSelectedDocumentImage, this, &ScanEditor::saveDocumentImage);
  connect(m_ocr_frame, &OcrFrame::makeCompleted, this, &ScanEditor::receiveModifyActionFinished);
  connect(m_ocr_frame, &OcrFrame::rejectChanges, this, &ScanEditor::receiveModificationCancelled);
  m_ocr_stack_id = m_scan_layout->addWidget(m_ocr_frame);

  m_scroller = new QScrollArea(this);
  m_scroller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_scroller->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  m_scroller->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  m_scroller->setContentsMargins(0, 0, 0, 0);

  m_scan_display = new ScanImage(m_data_dir, this);
  m_scan_display->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  m_scan_display->setScaledContents(true);
  m_scroller->setWidget(m_scan_display);
  grid_layout->addWidget(m_scroller, 0, 0);

  m_page_view = new PageView(this);
  m_page_view->setContentsMargins(0, 0, 0, 0);

  auto* btn_frame = new QFrame(this);
  btn_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  auto* btn_layout = new QGridLayout;
  btn_frame->setLayout(btn_layout);

  initImageEditBtns();
  btn_layout->addWidget(image_edit_group, 0, 0);

  initPageListBtns();
  btn_layout->addWidget(page_list_group, 1, 0);

  initTransferBtns();
  btn_layout->addWidget(image_transfer_group, 2, 0);

  enableMoveBtns(false);
  disableEditBtns();
  disableListBtns();

  grid_layout->addWidget(btn_frame, 0, 1);
  grid_layout->addWidget(m_page_view, 0, 2);
  grid_layout->setColumnStretch(0, 30);
  grid_layout->setColumnStretch(1, 3);
  grid_layout->setColumnStretch(2, 10);

  m_scan_layout->setCurrentIndex(m_stack_id);
}

void ScanEditor::makeConnections()
{
  connect(m_scan_display, &ScanImage::selectionUnderway, this, &ScanEditor::selectionUnderway);

  connect(m_scan_display, &ScanImage::selected, this, &ScanEditor::selected);
  connect(m_scan_display, &ScanImage::selected, this, &ScanEditor::enableEditBtns);
  connect(m_scan_display, &ScanImage::unselected, this, &ScanEditor::disableEditBtns);
  connect(m_page_view, &PageView::selected, this, &ScanEditor::enableListBtns);
  connect(m_page_view, &PageView::unselected, this, &ScanEditor::disableListBtns);
  connect(m_page_view, &PageView::ocrPageMoved, this, &ScanEditor::receivePageMoved);

  connect(m_scan_display, &ScanImage::imageIsLoaded, this, &ScanEditor::imageIsLoaded);
  connect(m_scan_display, &ScanImage::adjustScrollbar, this, &ScanEditor::adjustScrollbar);
  connect(m_page_view, &PageView::workOn, this, &ScanEditor::receiveWorkOnRequest);
  connect(m_page_view, &PageView::sendOcrPage, this, &ScanEditor::receiveOcrPageRequest);
  connect(m_page_view, &PageView::loadText, this, &ScanEditor::receiveLoadText);
  //  connect(m_page_view, &PageView::removeCurrentText, this, &ScanEditor::removeText);
  //  connect(m_page_view, &PageView::removeCurrentImage, this, &ScanEditor::removeImage);
  connect(m_ocr_tools, &OcrTools::convertedPage, this, &ScanEditor::receiveOcrPageResult);
  connect(m_ocr_tools, &OcrTools::convertedImage, this, &ScanEditor::receiveOcrImageResult);
  connect(m_ocr_tools, &OcrTools::convertedImageRect, this, &ScanEditor::receiveOcrImageRectResult);
}

/*
   Receives a single page image from the ScanImage object
   and stores it.
*/
void ScanEditor::receiveImage(const QImage& image)
{
  int index = DocumentDataStore::nextPageNumber();

  DocumentData doc_data(new DocData());
  doc_data->setResolution(m_scan_display->resolution());
  doc_data->setPageNumber(index);

  m_page_view->appendOcrThumbnail(thumbnail(image));

  QString path = saveImage(index, image);
  doc_data->setFilename(path);
  m_doc_data_store->appendData(doc_data);
}

/*
   Receives two split page images from the ScanImage object
   and stores them.
*/
void ScanEditor::receiveImages(const QImage& left, const QImage& right)
{
  receiveImage(left);
  receiveImage(right);
}

//void ScanEditor::receiveString(int page_no, const QString& text)
//{
//  if (page_no > 0) { // 0 is cover page so no text.
//    DocumentData doc_data = m_doc_data_store->documentData(page_no);

//    if (doc_data) {
//      doc_data->setText(page_no, Util::cleanText(text));
//    }
//  }
//}

QString ScanEditor::saveImage(int index, const QImage& image)
{
  QString path;

  if (m_current_doc_name.isEmpty()) {
    QString doc_name = QInputDialog::getText(this,
                       tr("Get Document Name"),
                       tr("Please supply a name for this document.\n"
                          "Do not add file extension as this is\n"
                          "created internally."));

    if (!doc_name.isEmpty()) {
      m_current_doc_name = doc_name;
      path = m_data_dir + QDir::separator() + doc_name;
      QDir dir;
      dir.mkpath(path);
      path += QDir::separator() + OCR_IMAGE_NAME.arg(index);

      if (!image.save(path, "PNG", 100)) {
        qInfo() << tr("Failed to save image %1").arg(path);
      }
    }

  } else {
    path = m_data_dir + OCR_IMAGE_NAME.arg(index);

    if (!image.save(path, "PNG", 100)) {
      qInfo() << tr("Failed to save image %1").arg(path);
    }
  }

  return path;
}

void ScanEditor::saveDataStore()
{
  m_doc_data_store->save(m_data_filename);
}

QString ScanEditor::saveDocumentImage(int index,
                                      int image_index,
                                      const QImage& image,
                                      const DocumentData& doc_data)
{
  QString path;

  if (m_current_doc_name.isEmpty()) {
    QString doc_name = QInputDialog::getText(this,
                       tr("Get Document Name"),
                       tr("Please supply a name for this document.\n"
                          "Do not add file extension as this is\n"
                          "created internally."));

    if (!doc_name.isEmpty()) {
      m_current_doc_name = doc_name;
      path = m_data_dir + QDir::separator() + doc_name;
      QDir dir;
      dir.mkpath(path);
      path += QDir::separator() + INTERNAL_IMAGE_NAME.arg(index).arg(image_index);
      doc_data->setImage(image_index, path);
      saveDataStore();

      if (!image.save(path, "PNG", 100)) {
        qInfo() << tr("Failed to save image %1").arg(path);
      }
    }

  } else {
    path = m_data_dir + INTERNAL_IMAGE_NAME.arg(index).arg(image_index);
    doc_data->setImage(image_index, path);
    saveDataStore();

    if (!image.save(path, "PNG", 100)) {
      qInfo() << tr("Failed to save image %1").arg(path);
    }
  }

  return path;
}

void ScanEditor::saveModifiedImage(int page_no, const QImage& image)
{
  DocumentData doc_data = m_doc_data_store->documentData(page_no);
  QString path = doc_data->filename();

  if (!image.save(path, "PNG", 100)) {
    qInfo() << tr("Failed to save image %1").arg(path);
    return;
  }

  if (doc_data) {
    m_page_view->replaceOcrThumbnail(page_no, thumbnail(image));
  }
}

void ScanEditor::makeCover()
{
  if (!m_current_doc_name.isEmpty()) {
    // TODO no doc name yet.
  } else {
    // always
    QImage image = m_scan_display->saveAsCover();
    m_page_view->setCover(thumbnail(image));
    QString path = m_data_dir + QDir::separator() + m_current_doc_name + QDir::separator()
                   + "cover.png";

    if (!image.save(path, "PNG", 100)) {
      qInfo() << tr("failed to save file path");
      return;
    }
  }
}

void ScanEditor::receiveLoadText(int page_no)
{
  if (page_no > 0) { //
    DocumentData doc_data = m_doc_data_store->documentData(page_no);

    if (doc_data) {
      receiveOcrPageResult(doc_data);
    }
  }
}

void ScanEditor::receiveWorkOnRequest(int page_no)
{
  DocumentData doc_data = m_doc_data_store->documentData(page_no);

  if (!doc_data.isNull()) {
    m_scan_layout->setCurrentIndex(m_ocr_stack_id);
    QImage image(doc_data->filename(), "PNG");
    m_ocr_frame->setData(page_no, image, doc_data);
    emit editingImage(true);
  }
}

void ScanEditor::receiveOcrPageRequest(int page_no)
{
  if (page_no > 0) { //
    DocumentData doc_data = m_doc_data_store->documentData(page_no);

    if (!doc_data) {
      m_ocr_tools->convertImageToText(doc_data);

    } else {
      if (QMessageBox::warning(this,
                               tr("Overwrite Text"),
                               tr("Text already exists for this image.\n"
                                  "Continuing will overwrite the existing text.\n"
                                  "Do you want to continue?"),
                               QMessageBox::Yes | QMessageBox::No,
                               QMessageBox::No)
          == QMessageBox::Yes) {
        m_ocr_tools->convertImageToText(doc_data);
      }
    }

  } else {
    // This will never be called normally as the cover page is filtered out
    // in the PageView. However someone might use this somewhere else.
    QMessageBox::warning(this, tr("Cover image."), tr("You cannot OCR the cover image."));
  }
}

void ScanEditor::receiveOcrImageRequest(int page_no, const QImage& image, const QRect& rect)
{
  if (page_no > 0) {
    m_ocr_tools->convertImageToText(page_no, image, rect);
  }
}

void ScanEditor::saveModifiedData(const DocumentData& doc_data)
{
  if (doc_data) {
    saveDataStore();
  }
}

void ScanEditor::receiveOcrPageResult(const DocumentData& doc_data)
{
  m_scan_layout->setCurrentIndex(m_ocr_stack_id);

  int page_no = doc_data->pageNumber();
  QImage image(doc_data->filename(), "PNG");
  m_ocr_frame->setData(page_no, image, doc_data);
}

void ScanEditor::receiveModifyActionFinished(DocumentData doc_data)
{
  doc_data->setCompleted(true);
  m_scan_layout->setCurrentIndex(m_stack_id);
  enableMoveBtns(false);
  disableEditBtns();
  disableListBtns();
  emit editingImage(false);
}

void ScanEditor::receiveModificationCancelled()
{
  m_scan_layout->setCurrentIndex(m_stack_id);
  enableMoveBtns(false);
  disableEditBtns();
  disableListBtns();
  emit editingImage(false);
}

void ScanEditor::receivePageMoved(int start_row, int dest_row)
{
  // TODO
}

void ScanEditor::removeBoth(int page_no)
{
  int page = page_no;

  if (page < 0) { // no page supplied.
    page = m_page_view->currentPageNumber();

    if (page < 0) { // no current selection
      return;
    }
  }

  if (page > 0) {
    DocumentData doc_data = m_doc_data_store->documentData(page_no);

    if (doc_data) {
      if (doc_data->hasText()) {
        int answer
          = QMessageBox::warning(this,
                                 tr("Remove Both Image and Text"),
                                 tr("You are about to remove the both the image and the text\n"
                                    "This cannot be undone.\n"
                                    "Are you sure?"),
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::No);

        if (answer == QMessageBox::Yes) {
          m_page_view->removeOcrThumbnail();
          DocumentData doc_data = m_doc_data_store->documentData(page);
          doc_data->setRemoveTextLater(true);
          doc_data->setRemoveImageLater(true);
        }
      }

      return;
    }
  }
}

void ScanEditor::removeImage(int page_no)
{
  int page = page_no;

  if (page < 0) { // no page supplied.
    page = m_page_view->currentPageNumber();

    if (page < 0) { // no current selection
      return;
    }
  }

  if (page == 0) {
    int answer = QMessageBox::warning(this,
                                      tr("Remove Cover Image"),
                                      tr("You are about to remove the Cover image\n"
                                         "This cannot be undone.\n"
                                         "Are you sure?"),
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No);

    if (answer == QMessageBox::Yes) {
      m_page_view->removeOcrThumbnail();
      DocumentData doc_data = m_doc_data_store->documentData(page_no);
      doc_data->setRemoveImageLater(true);
    }

  } else if (page > 0) {
    int answer = QMessageBox::warning(this,
                                      tr("Remove Internal Image"),
                                      tr("You are about to remove the image\n"
                                         "This cannot be undone.\n"
                                         "Are you sure?"),
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No);

    if (answer == QMessageBox::Yes) {
      m_page_view->removeOcrThumbnail();
      DocumentData doc_data = m_doc_data_store->documentData(page_no);
      doc_data->setRemoveImageLater(true);
    }
  }
}

void ScanEditor::removeText(int page_no)
{
  int page = page_no;

  if (page < 0) { // no page supplied.
    page = m_page_view->currentPageNumber();

    if (page < 0) { // no current selection
      return;
    }
  }

  if (page > 0) {
    int answer = QMessageBox::warning(this,
                                      tr("Remove Text"),
                                      tr("You are about to remove the text\n"
                                         "for this OCR image.\n"
                                         "This cannot be undone.\n"
                                         "Are you sure?"),
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No);

    if (answer == QMessageBox::Yes) {
      DocumentData doc_data = m_doc_data_store->documentData(page);
      doc_data->setRemoveTextLater(true);
    }
  }
}

void ScanEditor::receiveOcrImageResult(int page_no, const QString& text)
{
  if (m_ocr_frame) {
    m_ocr_frame->setOcrText(page_no, text);
  }
}

void ScanEditor::receiveOcrImageRectResult(int page_no, const QString& text)
{
  if (m_ocr_frame) {
    m_ocr_frame->appendOcrText(page_no, text);
  }
}

/*!
   \brief Splits a single page image into two equal size page images.
*/
void ScanEditor::moveBothPagesToDocument()
{
  QPair<QImage, QImage> images = m_scan_display->splitPages();
  receiveImages(images.first, images.second);
}

/*!
   \brief Splits a single page image into two equal size page images but
   discards the right hand page.
*/
void ScanEditor::moveLeftPageToDocument()
{
  QImage image = m_scan_display->splitLeftPage();
  receiveImage(image);
}

/*!
   \brief Splits a single page image into two equal size page images but
   discards the left hand page.
*/
void ScanEditor::moveRightPageToDocument()
{
  QImage image = m_scan_display->splitRightPage();
  receiveImage(image);
}

//QString ScanEditor::saveDocumentImage(int page_no, int image_index, const QImage &image)
//{
//  DocumentData doc_data = m_doc_data_store->documentData(page_no);

//  QFileInfo info(m_data_dir);
//  QDir dir = info.dir();
//  QString filename = dir.path() + QDir::separator()
//                     + INTERNAL_IMAGE_NAME.arg(page_no).arg(image_index);
//  image.save(filename);

//  doc_data->setImage(image_index, filename);
//  doc_data->setResolution(m_scan_display->resolution());

//  return filename;
//}

/*!
   \brief Takes the image and makes it into a single page.
*/
void ScanEditor::movePageToDocument()
{
  QImage image = m_scan_display->makePage();
  receiveImage(image);
}

/*!
   \brief Loads all existing images and text on the current document path.

  Please note that either loadOptions() or setDocumentName() must be used before
  loadExistingFile(), otherwise no files will be  imported.
*/
void ScanEditor::loadExistingFiles()
{
  QString current_path = m_data_dir + QDir::separator() + m_current_doc_name + QDir::separator();
  setDocumentPath(current_path);
  m_data_filename = current_path + m_current_doc_name + ".yaml";
  QString filename;
  QImage image;
  QStringList filter;
  filter << OCR_IMAGE_FILTER;

  m_doc_data_store = new DocumentDataStore(this);
  m_doc_data_store->load(m_data_filename);

  QList<int> keys = m_doc_data_store->documentKeys();

  for (int key : keys) {
    DocumentData doc_data = m_doc_data_store->documentData(key);
    filename = doc_data->filename();

    if (key == 0) {
      loadCover(filename);
      qInfo() << tr("Cover file loaded)");

    } else {
      if (!doc_data->isCompleted()) {
        image = QImage(filename, "PNG");
        int page_no = m_page_view->appendOcrThumbnail(thumbnail(image));

        if (page_no >= 0) {
          doc_data->setPageNumber(page_no);

          if (key != page_no) {
            m_doc_data_store->moveKey(key, page_no);
          }
        }

        qInfo() << tr("Image file %1 loaded.").arg(doc_data->filename());

      } else {
        int page_no = m_page_view->appendOcrCompleted(true);

        if (page_no >= 0) {
          doc_data->setPageNumber(page_no);

          if (key != page_no) {
            m_doc_data_store->moveKey(key, page_no);
          }
        }

        qInfo() << tr("File %1 is completed.").arg(doc_data->filename());
      }
    }
  }
}

/*!
   \brief Loads the options file.

  Currently the options only include the current document name, which is used as
  a name for the image/text storage directory and the Tesseract language. If you
  do not call loadOptions(), for example if you do not want to use the default
  yaml options then you must set these values manually using
  setTesseractLanguage() and setDocumentname().

  If you call loadOptions() with no parameter then the default options file
  "{configdir}/scanoptions.yaml" will be used, otherwise to use a different
  filename for the options file use
  loadOptions("path/to/your/options/file.yaml").

  Please note that either loadOptions() or setDocumentName() must be used before
  loadExistingFile(), otherwise no files will be  imported.

  \param filename - the optional options file name with path.
*/
void ScanEditor::loadOptions(const QString& filename)
{
  QString fn;

  if (filename.isEmpty()) {
    fn = m_options_file;

  } else {
    fn = filename;
  }

  QFile file(fn);

  if (file.exists()) {
    YAML::Node m_options = YAML::LoadFile(file);

    if (m_options[CURRENT_DOCUMENT]) {
      m_current_doc_name = m_options[CURRENT_DOCUMENT].as<QString>();
      setWindowTitle(m_current_doc_name);
    }

    if (m_options[TESSERACT]) {
      YAML::Node tesseract_options = m_options[TESSERACT];

      if (tesseract_options[LANGUAGE]) {
        m_lang = tesseract_options[LANGUAGE].as<QString>();
      }
    }
  }
}

/*!
   \brief Saves the options into the options file.

  Currently the options only include the current document name, which is used as
  a name for the image/text storage directory and the Tesseract language. If you
  do not call saveOptions(), for example if you do not want to use the default
  yaml options then you must set these values manually using
  setTesseractLanguage() and setDocumentname().

  If you call saveOptions() with no parameter then the default options file
  "{configdir}/scanoptions.yaml" will be used, otherwise to use a different
  filename for the options file use
  saveOptions("path/to/your/options/file.yaml").

  \param filename - the optional options file name with path.
*/
void ScanEditor::saveOptions(const QString& filename)
{
  QString fn;

  if (filename.isEmpty()) {
    fn = m_options_file;

  } else {
    fn = filename;
  }

  QFile* file;
  file = new QFile(fn);

  if (file->open((QFile::ReadWrite | QFile::Truncate))) {
    YAML::Emitter emitter;
    emitter << YAML::BeginMap;
    emitter << YAML::Key << CURRENT_DOCUMENT;
    emitter << YAML::Value << m_current_doc_name;
    emitter << YAML::Key << TESSERACT;
    emitter << YAML::Value;
    emitter << YAML::BeginMap;
    emitter << YAML::Key << LANGUAGE;
    emitter << YAML::Value << m_lang;
    emitter << YAML::EndMap;
    emitter << YAML::EndMap;
    QTextStream out(file);
    out << emitter.c_str();
    file->close();
  }
}

void ScanEditor::loadCover(const QString& filename)
{
  QImage image = QImage(filename, "PNG");
  DocumentData data(new DocData());
  data->setFilename(filename);
  data->setCompleted(!image.isNull());
  m_page_view->setCover(thumbnail(image));
}

void ScanEditor::setImage(const QImage& image, const int resolution)
{
  if (m_prog_dlg) {
    m_prog_dlg->close();
    m_prog_dlg = nullptr;
  }

  m_resolution = resolution;
  m_scan_display->setImage(image, resolution);
  enableMoveBtns(true);
}

void ScanEditor::setScanProgress(const int& progress)
{
  if (m_prog_dlg) {
    m_prog_dlg->setValue(int(progress));
  }
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

/*!
  \brief Sets up a new document name.

  \param name - the new document name.
*/
void ScanEditor::setDocumentName(const QString& name)
{
  m_current_doc_name = name;
}

/*!
  \brief Sets up a new document path.

  This defines where files are saved by default.

  \param name - the new document path.
*/
void ScanEditor::setDocumentPath(const QString& path)
{
  m_data_dir = path;
}

void ScanEditor::completeDocument()
{
  // TODO action when completing document.
}

QString ScanEditor::documentName() const
{
  return m_current_doc_name;
}

bool ScanEditor::eventFilter(QObject* obj, QEvent* event)
{
  bool result;

  if (event->type() == QEvent::KeyPress) {
    auto* key_event = dynamic_cast<QKeyEvent*>(event);

    if (key_event) {
      //      bool is_keypad = ((key_event->modifiers()) & Qt::KeypadModifier);
      //      int key = key_event->key();

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
    m_scan_display->resize(frameRect().size());
    result = true;

  } else {
    result = QObject::eventFilter(obj, event);
  }

  return result;
}

void ScanEditor::adjustScrollbar(qreal factor)
{
  QScrollBar* scrollbar = m_scroller->horizontalScrollBar();
  scrollbar->setValue(int(factor * scrollbar->value() + ((factor - 1) * scrollbar->pageStep() / 2)));
  scrollbar = m_scroller->verticalScrollBar();
  scrollbar->setValue(int(factor * scrollbar->value() + ((factor - 1) * scrollbar->pageStep() / 2)));
}

void ScanEditor::selectAll()
{
  m_scan_display->selectAll();
}

void ScanEditor::rotate180()
{
  m_scan_display->rotate180();
}

void ScanEditor::rotateCW()
{
  m_scan_display->rotateCW();
}

void ScanEditor::rotateACW()
{
  m_scan_display->rotateCCW();
}

void ScanEditor::rotateByAngle()
{
  m_scan_display->rotateByAngle();
}

void ScanEditor::rotateByEdge()
{
  m_scan_display->rotateByEdge();
}

void ScanEditor::copySelection()
{
  m_scan_display->copySelection();
}

void ScanEditor::cropToSelection()
{
  m_scan_display->cropToSelection();
}

void ScanEditor::clearSelection()
{
  m_scan_display->clearSelection();
}

void ScanEditor::cropToContent()
{
  m_scan_display->cropToContent();
}

void ScanEditor::rescan()
{
  m_scan_display->rescan();
}

void ScanEditor::scale()
{
  m_scan_display->rescale();
}

void ScanEditor::save()
{
  m_scan_display->save();
  // TODO save text.
}

void ScanEditor::saveAs()
{
  m_scan_display->saveAs();
}

void ScanEditor::zoomIn()
{
  m_scan_display->zoomIn();
}

void ScanEditor::zoomOut()
{
  m_scan_display->zoomOut();
}

void ScanEditor::fitBest()
{
  m_scan_display->fitBest();
}

void ScanEditor::fitHeight()
{
  m_scan_display->fitHeight();
}

void ScanEditor::fitWidth()
{
  m_scan_display->fitWidth();
}

void ScanEditor::setDefaultPageCropSize()
{
  m_scan_display->setDefaultPageCropSize();
}

int ScanEditor::pageCount()
{
  return m_doc_data_store->size();
}

DocumentData ScanEditor::documentData(int page_no)
{
  return m_doc_data_store->documentData(page_no);
}

/*!
   \brief gets the current options file.

   \return the options file name.
*/
QString ScanEditor::optionsFile() const
{
  return m_options_file;
}

/*!
   \brief sets the options file name.

   By default this is {configdir}/scanoptions.yaml.

   \param options_file - the new file name.
*/
void ScanEditor::setOptionsFile(const QString& options_file)
{
  m_options_file = options_file;
}

/*!
   \brief gets the tesseract language.

   \return the tesseract language string.
*/
QString ScanEditor::tesseractLanguage() const
{
  return m_lang;
}

/*!
   \brief sets the tesseract language.

   \param lang - tesseract language string.
*/
void ScanEditor::setTesseractLanguage(const QString& lang)
{
  m_lang = lang;
}

void ScanEditor::cleanupImages()
{
  //  m_doc_data->cleanUpData();

  QDir dir(m_data_dir);
  QStringList filter;
  filter << OCR_IMAGE_FILTER;
  QStringList existing_files = dir.entryList(filter);
  QStringList document_files;
  QStringList internal_files;
  QString filepath, filename;

  for (int page_no : m_doc_data_store->documentKeys()) {
    DocumentData doc_data = m_doc_data_store->documentData(page_no);
    filepath = doc_data->filename();
    QFileInfo info(filepath);
    filename = info.fileName();

    if (existing_files.contains(filename)) {
      if (doc_data->isRemoveImageLater()) {
        // normally because they have been OCR'd and completed.
        dir.remove(filepath);

        if (doc_data->pageNumber() > 0) {
          m_doc_data_store->remove(doc_data);

        } else {
          // don't want to remove cover slot, just block loading an image.
          doc_data->setFilename(QString());
        }

      } else if (doc_data->isDocImage()) {
        // file exists and it has been converted to an internal image.
        internal_files.append(filepath);
        // rename actual file.
        filename = dir.path() + INTERNAL_IMAGE_NAME.arg(page_no);

        if (filepath != filename) {
          // file hasn't already been converted.
          dir.rename(filepath, filename);
          doc_data->setFilename(filename);
          existing_files.removeOne(filepath);
        }

      } else {
        // image still needed
        //        bool ok;
        int len = filename.lastIndexOf(".") - 8;
        QString file_int = filename.mid(8, len);
        int file_no = file_int.toInt(/*&ok*/);

        if (file_no == doc_data->pageNumber()) {
          document_files.append(filepath);
          existing_files.removeOne(filepath);

        } else {
          filename.replace(8, len, QString::number(doc_data->pageNumber()));
          filename.prepend(QDir::separator());
          filename.prepend(dir.path());

          if (dir.exists(filename)) {
            dir.remove(filename);
          }

          dir.rename(filepath, filename);
          doc_data->setFilename(filename);
        }
      }

    } else {
      // TODO ???
    }
  }

  // remove files for which there is no data.
  //  for (const QString &fn : existing_files) {
  //    dir.remove(fn);
  //  }
}

int ScanEditor::resolution() const
{
  return m_resolution;
}

void ScanEditor::setResolution(int resolution)
{
  m_resolution = resolution;
}

QImage ScanEditor::thumbnail(const QImage& image) const
{
  if (!image.isNull()) {
    int w = image.width();
    qreal factor = w / 100.0;
    w = 100;
    int h = int(image.height() / factor);
    QSize size(w, h);
    QImage thumbnail = image.scaled(w, h, Qt::KeepAspectRatio);
    return thumbnail;
  }

  return QImage();
}

void ScanEditor::enableMoveBtns(bool enable)
{
  //  m_cover_btn->setEnabled(enable);
  //  m_left_btn->setEnabled(enable);
  //  m_right_btn->setEnabled(enable);
  //  m_single_btn->setEnabled(enable);
  //  m_both_btn->setEnabled(enable);
  image_transfer_group->setEnabled(enable);
}

void ScanEditor::enableListBtns()
{
  //  m_move_up_btn->setEnabled(true);
  //  m_move_down_btn->setEnabled(true);
  page_list_group->setEnabled(true);
}

void ScanEditor::disableListBtns()
{
  //  m_move_up_btn->setEnabled(false);
  //  m_move_down_btn->setEnabled(false);
  page_list_group->setEnabled(false);
}

void ScanEditor::enableEditBtns()
{
  //  m_crop_btn->setEnabled(true);
  image_edit_group->setEnabled(true);
}

void ScanEditor::disableEditBtns()
{
  //  m_crop_btn->setEnabled(false);
  image_edit_group->setEnabled(false);
}
