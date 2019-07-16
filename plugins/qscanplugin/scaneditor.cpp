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

#include <qyaml-cpp/QYamlCpp>
#include <yaml-cpp/yaml.h>

#include "documentdata.h"
#include "qscan.h"
#include "scaneditor.h"
#include "scanimage.h"

//#include "ocrframe.h"

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

/*!
   \brief Constructor for the ScanEditor class.

   \param scan - a pointer to the QScan library.
   \param configdir - the directory for configuration files.
   \param datadir - the base directory for image and text files.
   \param lang - the tesseract language.
   \param parent - the parent QObject.
*/
ScanEditor::ScanEditor(QWidget* parent)
  : StackableFrame(parent)
{
  QDir dir;

  QString config_path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
  config_path += "/Biblos";
  dir.mkpath(config_path);
  m_options_file = config_path + QDir::separator() + OPTIONS_FILE;

  m_data_dir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);

  m_lang = "eng";
  m_scan_lib = new QScan(this);
  m_scan_lib->init();

  connect(m_scan_lib, &QScan::optionsSet, this, &ScanEditor::receiveOptionsSet);
  connect(
    m_scan_lib, &QScan::modeChanged, this, &ScanEditor::receiveModeChange);
  connect(
    m_scan_lib, &QScan::sourceChanged, this, &ScanEditor::receiveSourceChange);
  connect(
    m_scan_lib, &QScan::scanCompleted, this, &ScanEditor::setImage);
  connect(m_scan_lib,
          &QScan::scanProgress,
          this,
          &ScanEditor::setScanProgress);
  connect(m_scan_lib, &QScan::scanFailed, this, &ScanEditor::scanHasFailed);


  loadOptions();
  loadCurrentDocument();

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  initGui();

  //  QStringList scanners = m_scan_lib->devices();

  //  if (scanners.isEmpty()) {
  //    m_scanner_box->addItem(tr("No scanners available"));

  //  } else {

  //    m_selected_name = scanners.at(0);

  //    if (!m_selected_name.isEmpty()) {
  //      if (m_scan_lib->openDevice(m_selected_name)) {
  //        ScanDevice* device = m_scan_lib->device(m_selected_name);
  //        receiveOptionsSet(device);
  //      }

  //    } else {
  //      qCDebug(QscanWidgets) << tr("Unable to open %1").arg(m_selected_name);
  //    }
  //  }

  makeConnections();
}

ScanEditor::~ScanEditor()
{
  cleanupImages();
  saveDataStore();
  saveOptions();
}

void ScanEditor::initPageListBtns()
{
  QPixmap rem_img_icon, rem_txt_icon, rem_img_txt_icon;
  QPixmap move_up_icon, move_down_icon, move_to_icon;
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/del_image")), &rem_img_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/del_text")), &rem_txt_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/del_image_text")), &rem_img_txt_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/up")), &move_up_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/down")), &move_down_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/move_to_edit")), &move_to_icon);

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
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/crop")), &crop_icon);

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
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/cover64")), &cover_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/left64")), &left_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/right64")), &right_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/single64")), &single_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/both64")), &both_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/scale")), &scale_icon);

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
}

//QFrame* ScanEditor::initMenuAndToolbars()
//{
//  QFrame* bar_frame = new QFrame(this);
//  QVBoxLayout* bar_layout = new QVBoxLayout;
//  bar_frame->setLayout(bar_layout);

//  QToolBar* toolbar1 = new QToolBar(this);
//  bar_layout->addWidget(toolbar1);

//  QPixmap scan_icon;
//  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/scan")), &scan_icon);
//  QAction* scan_act = new QAction(scan_icon, tr("Scan"), this);
//  toolbar1->addAction(scan_act);
//  connect(scan_act, &QAction::triggered, this, &ScanEditor::startScanning);

//  QToolBar* toolbar2 = new QToolBar(this);
//  auto* stack_frame = new QFrame(this);
//  toolbar2->addWidget(stack_frame);
//  bar_layout->addWidget(toolbar2);

//  m_res_layout = new QStackedLayout;
//  stack_frame->setLayout(m_res_layout);
//  {
//    m_res_range = new QFrame(this);
//    auto* range_layout = new QGridLayout;
//    m_res_range->setLayout(range_layout);
//    /*===*/
//    auto* lbl = new QLabel(tr("Min Resolution :"), this);
//    lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    range_layout->addWidget(lbl, 0, 0);

//    QString s(tr("%1 %2"));
//    m_min_res = new QLabel(s.arg(1).arg("dpi"), this);
//    m_min_res->setAlignment(Qt::AlignLeft);
//    m_min_res->setContentsMargins(0, 0, 0, 0);
//    m_min_res->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    range_layout->addWidget(m_min_res, 0, 1);
//    /*===*/
//    lbl = new QLabel(tr("Resolution :"), this);
//    lbl->setContentsMargins(30, 0, 0, 0);
//    lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    range_layout->addWidget(lbl, 0, 2);

//    // the edit lineedit will only accept integers between 100 and 999
//    m_res_edit = new QLineEdit(this);
//    m_res_edit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    m_res_validator = new QIntValidator(1, 1, this);
//    m_res_edit->setValidator(new QIntValidator(1, 1, this));
//    m_res_edit->setText("1");
//    range_layout->addWidget(m_res_edit, 0, 3);
//    lbl = new QLabel("dpi", this);
//    lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    range_layout->addWidget(lbl, 0, 4);

//    /*===*/
//    lbl = new QLabel("Max Resolution :", this);
//    lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    range_layout->addWidget(lbl, 0, 5);

//    m_max_res = new QLabel(s.arg(1).arg("dpi"), this);
//    m_max_res->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    m_max_res->setAlignment(Qt::AlignLeft);
//    m_max_res->setContentsMargins(0, 0, 0, 0);
//    range_layout->addWidget(m_max_res, 0, 6);

//    m_stack_range_id = m_res_layout->addWidget(m_res_range);
//  }
//  {
//    m_res_list = new QFrame(this);
//    m_res_list->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    auto* list_layout = new QGridLayout;
//    m_res_list->setLayout(list_layout);
//    m_res_list->setVisible(true);

//    auto* lbl = new QLabel(tr("Resolution :"), this);
//    lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    list_layout->addWidget(lbl, 0, 0);

//    m_res_combo = new QComboBox(this);
//    m_res_combo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    m_res_combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
//    list_layout->addWidget(m_res_combo, 0, 1);

//    auto* spacer_widget = new QWidget(this);
//    spacer_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
//    list_layout->addWidget(spacer_widget, 0, 2);

//    m_stack_list_id = m_res_layout->addWidget(m_res_list);
//  }

//  return bar_frame;
//}

void ScanEditor::initGui()
{
  m_scan_layout = new QStackedLayout;
  setLayout(m_scan_layout);

  QFrame* main_frame = new QFrame(this);
  m_editor_layout = new QGridLayout;
  m_editor_layout->setContentsMargins(0, 0, 0, 0);
  main_frame->setLayout(m_editor_layout);
  m_main_id = m_scan_layout->addWidget(main_frame);

  int row = 0;

  m_scroller = new QScrollArea(this);
  m_scroller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_scroller->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  m_scroller->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  m_scroller->setContentsMargins(2, 0, 0, 0);

  m_scan_display = new ScanImage(m_data_dir, this);
  m_scan_display->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  m_scan_display->setScaledContents(true);
  m_scroller->setWidget(m_scan_display);
  m_editor_layout->addWidget(m_scroller, row, 0);

  // needs to be created before the buttons are connected.
  m_page_view = new PageView(this);

  auto* btn_frame = new QFrame(this);
  btn_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  auto* btn_layout = new QGridLayout;
  btn_frame->setLayout(btn_layout);
  m_editor_layout->addWidget(btn_frame, row, 1);

  initImageEditBtns();
  btn_layout->addWidget(image_edit_group, 0, 0);

  initPageListBtns();
  btn_layout->addWidget(page_list_group, 1, 0);

  initTransferBtns();
  btn_layout->addWidget(image_transfer_group, 2, 0);

  m_page_view->setContentsMargins(0, 0, 0, 0);
  m_editor_layout->addWidget(m_page_view, row, 2);

  m_editor_layout->setColumnStretch(0, 30);
  m_editor_layout->setColumnStretch(1, 3);
  m_editor_layout->setColumnStretch(2, 10);

  enableMoveBtns(false);
  disableEditBtns();
  disableListBtns();

  m_scan_layout->setCurrentIndex(m_main_id);
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
  connect(m_page_view, &PageView::loadText, this, &ScanEditor::receiveLoadText);
  //  connect(m_page_view, &PageView::sendOcrPage, this, &ScanEditor::sendOcrRequest);

  //  connect(m_ocr_tools, &OcrTools::convertedPage, this, &ScanEditor::receiveOcrPageResult);
  //  connect(m_ocr_tools, &OcrTools::convertedImage, this, &ScanEditor::receiveOcrImageResult);
  //  connect(m_ocr_tools, &OcrTools::convertedImageRect, this, &ScanEditor::receiveOcrImageRectResult);
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
        qCInfo(QscanOcr) << tr("Failed to save image %1").arg(path);
      }
    }

  } else {
    path = m_data_dir + OCR_IMAGE_NAME.arg(index);

    if (!image.save(path, "PNG", 100)) {
      qCInfo(QscanOcr) << tr("Failed to save image %1").arg(path);
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
        qCInfo(QscanOcr) << tr("Failed to save image %1").arg(path);
      }
    }

  } else {
    path = m_data_dir + INTERNAL_IMAGE_NAME.arg(index).arg(image_index);
    doc_data->setImage(image_index, path);
    saveDataStore();

    if (!image.save(path, "PNG", 100)) {
      qCInfo(QscanOcr) << tr("Failed to save image %1").arg(path);
    }
  }

  return path;
}

void ScanEditor::saveModifiedImage(int page_no, const QImage& image)
{
  DocumentData doc_data = m_doc_data_store->documentData(page_no);
  QString path = doc_data->filename();

  if (!image.save(path, "PNG", 100)) {
    qCInfo(QscanOcr) << tr("Failed to save image %1").arg(path);
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
      qCInfo(QscanOcr) << tr("failed to save file path");
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
    QImage image(doc_data->filename(), "PNG");
    emit sendWorkData(page_no, image, doc_data, m_doc_data_store->lang());
  }
}

//void ScanEditor::receiveOcrPageRequest(int page_no)
//{
//  if (page_no > 0) { //
//    DocumentData doc_data = m_doc_data_store->documentData(page_no);

//    if (!doc_data) {
//      m_ocr_tools->convertImageToText(doc_data);

//    } else {
//      if (QMessageBox::warning(this,
//                               tr("Overwrite Text"),
//                               tr("Text already exists for this image.\n"
//                                  "Continuing will overwrite the existing text.\n"
//                                  "Do you want to continue?"),
//                               QMessageBox::Yes | QMessageBox::No,
//                               QMessageBox::No)
//          == QMessageBox::Yes) {
//        m_ocr_tools->convertImageToText(doc_data);
//      }
//    }

//  } else {
//    // This will never be called normally as the cover page is filtered out
//    // in the PageView. However someone might use this somewhere else.
//    QMessageBox::warning(this, tr("Cover image."), tr("You cannot OCR the cover image."));
//  }
//}

//void ScanEditor::receiveOcrImageRequest(int page_no, const QImage& image, const QRect& rect)
//{
//  if (page_no > 0) {
//    m_ocr_tools->convertImageToText(page_no, image, rect);
//  }
//}

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
  emit sendWorkData(page_no, image, doc_data, m_doc_data_store->lang());
}

void ScanEditor::receiveModifyActionFinished(DocumentData doc_data)
{
  doc_data->setCompleted(true);
  m_scan_layout->setCurrentIndex(m_main_id);
  enableMoveBtns(false);
  disableEditBtns();
  disableListBtns();
  //  emit editingImage(false);
}

void ScanEditor::receiveModificationCancelled()
{
  m_scan_layout->setCurrentIndex(m_main_id);
  enableMoveBtns(false);
  disableEditBtns();
  disableListBtns();
  //  emit editingImage(false);
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

//void ScanEditor::receiveOcrImageResult(int page_no, const QString& text)
//{
//  if (m_ocr_frame) {
//    m_ocr_frame->setOcrText(page_no, text);
//  }
//}

//void ScanEditor::receiveOcrImageRectResult(int page_no, const QString& text)
//{
//  if (m_ocr_frame) {
//    m_ocr_frame->appendOcrText(page_no, text);
//  }
//}

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
void ScanEditor::loadCurrentDocument()
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
      qCInfo(QscanOcr) << tr("Cover file loaded)");

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

        qCInfo(QscanOcr) << tr("Image file %1 loaded.").arg(doc_data->filename());

      } else {
        int page_no = m_page_view->appendOcrCompleted(true);

        if (page_no >= 0) {
          doc_data->setPageNumber(page_no);

          if (key != page_no) {
            m_doc_data_store->moveKey(key, page_no);
          }
        }

        qCInfo(QscanOcr) << tr("File %1 is completed.").arg(doc_data->filename());
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
void ScanEditor::loadOptions()
{
  QFile file(m_options_file);

  if (file.exists()) {
    YAML::Node m_options = YAML::LoadFile(file);

    if (m_options[CURRENT_DOCUMENT]) {
      m_current_doc_name = m_options[CURRENT_DOCUMENT].as<QString>();
      setWindowTitle(m_current_doc_name);
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
void ScanEditor::saveOptions()
{
  QFile* file;
  file = new QFile(m_options_file);

  if (file->open((QFile::ReadWrite | QFile::Truncate))) {
    YAML::Emitter emitter;
    emitter << YAML::BeginMap;
    emitter << YAML::Key << CURRENT_DOCUMENT;
    emitter << YAML::Value << m_current_doc_name;

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

  m_image_resolution = resolution;
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
  loadCurrentDocument();
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

void ScanEditor::completeDocument(DocumentData doc_data)
{
  // TODO action when completing document.
  doc_data->setCompleted(true);
  m_doc_data_store->save(m_data_filename);
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

void ScanEditor::receiveOptionsSet(ScanDevice* device)
{
  ScanOptions* options = device->options;
  //  m_mode_box->clear();
  QStringList list = options->modes();
  //  m_mode_box->addItems(list);
  m_modes_menu->clear();

  for (auto mode : list) {
    QAction* act = new QAction(mode);
    m_modes_menu->addAction(act);
    connect(act, &QAction::triggered, this, &ScanEditor::modeChangeSelected);
  }

  QStringList sources =  options->sources();
  m_src_menu->clear();

  for (auto source : sources) {
    QAction* act = new QAction(source);
    m_src_menu->addAction(act);
    connect(act, &QAction::triggered, this, &ScanEditor::sourceChangeSelected);
  }

  ScanRange scan_range = options->resolutionRange();
  ScanUnits units = options->units();
  QString u = (units == ScanUnits::DPI ? "dpi" : "mm");

  if (scan_range.range_data.canConvert<ScanRange>()) {
    QString s = tr("Change ranged resolution (%1)");
    QAction* act = new QAction(s.arg(u));
    act->setData(scan_range.range_data);
    m_res_menu->addAction(act);
    connect(act, &QAction::triggered, this, &ScanEditor::resolutionRangedChangeSelected);

  } else if (scan_range.range_data.canConvert<QList<int>>()) {
    QString s = tr("%1%2");

    for (auto res : scan_range.range_data.value<QList<int>>()) {
      QAction* act = new QAction(s.arg(res).arg(u));
      act->setData(res);
      m_res_menu->addAction(act);
      connect(act, &QAction::triggered, this, &ScanEditor::resolutionChangeSelected);
    }
  }
}

void ScanEditor::receiveScannerChange(bool)
{
  QObject* o = sender();
  QAction* act = qobject_cast<QAction*>(o);

  if (act) {
    //    ScanDevice* scanner = act->data().value<ScanDevice*>();
    QString name = act->text();
    emit currentScanner(name);
    m_scan_lib->openDevice(name);
  }
}

void ScanEditor::receiveModeChange(ScanDevice* device)
{
  m_curr_mode->setText(device->options->mode());
}

void ScanEditor::receiveSourceChange(ScanDevice* device)
{
  m_curr_src->setText(device->options->source());
}

void ScanEditor::resolutionEdited(const QString& value)
{
  ScanDevice* device = m_scan_lib->device(m_selected_name);
  m_scan_lib->setResolution(device, value.toInt());
}

void ScanEditor::modeChangeSelected(const bool /*triggered*/)
{
  QObject* o = sender();
  QAction* act = qobject_cast<QAction*>(o);

  if (act) {
    QString mode = act->text();

    if (!mode.isEmpty() && !m_selected_name.isEmpty()) {
      ScanDevice* device = m_scan_lib->device(m_selected_name);
      m_scan_lib->setScanMode(device, mode);
    }
  }
}

void ScanEditor::sourceChangeSelected(bool /*triggered*/)
{
  QObject* o = sender();
  QAction* act = qobject_cast<QAction*>(o);

  if (act) {
    QString source = act->text();

    if (!source.isEmpty() && !m_selected_name.isEmpty()) {
      ScanDevice* device = m_scan_lib->device(m_selected_name);
      m_scan_lib->setSource(device, source);
    }
  }
}

void ScanEditor::resolutionChangeSelected(bool)
{
  QObject* o = sender();
  QAction* act = qobject_cast<QAction*>(o);

  if (act && !m_selected_name.isEmpty()) {
    ScanDevice* device = m_scan_lib->device(m_selected_name);
    bool ok = false;
    int res = act->data().toInt(&ok);

    if (ok) {
      m_scan_lib->setResolution(device, res);
      emit currentResolution(res);
    }
  }
}

void ScanEditor::resolutionRangedChangeSelected(bool)
{
  QObject* o = sender();
  QAction* act = qobject_cast<QAction*>(o);

  if (act && !m_selected_name.isEmpty()) {
    ScanRange scan_range = act->data().value<ScanRange>();

    if (scan_range.range_data.canConvert<RangeData>()) {
      if (!m_selected_name.isEmpty()) {
        RangeData range_data = scan_range.range_data.value<RangeData>();
        QString s = tr("Resolution (Min %1, Max %2)");
        bool ok = false;
        ScanDevice* device = m_scan_lib->device(m_selected_name);
        int res = QInputDialog::getInt(this,
                                       tr("Select resolution"),
                                       s.arg(range_data.min).arg(range_data.max),
                                       scan_range.value,
                                       range_data.min,
                                       range_data.max,
                                       1,
                                       &ok);

        if (ok) {
          m_scan_lib->setResolution(device, res);
          emit currentResolution(res);
        }
      }
    }
  }
}

void ScanEditor::scannerSelectionChanged(int index)
{
  if (!m_scan_lib->devices().isEmpty()) {
    QString name = m_scan_lib->devices().at(index);

    if (!name.isEmpty()) {
      if (m_scan_lib->openDevice(name)) {
        m_selected_name = name;
        m_scanners_menu->setEnabled(true);

      } else {
        qCDebug(QscanWidgets) << tr("Unable to open %1").arg(m_selected_name);
      }
    }
  }
}

void ScanEditor::scanHasFailed()
{
  QMessageBox::warning(this,
                       tr("Scan failed"),
                       tr("The scanner returned with a Scan Failed message."),
                       QMessageBox::Ok);
}

void ScanEditor::scanOpenHasFailed()
{
  QMessageBox::warning(this,
                       tr("Scan failed to access scanner"),
                       tr("The scanner returned with a invalid scanner message."),
                       QMessageBox::Ok);
}

void ScanEditor::startScanning()
{
  m_scan_lib->startScanning(m_selected_name);
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

///*!
//   \brief sets the options file name.

//   By default this is {configdir}/scanoptions.yaml.

//   \param options_file - the new file name.
//*/
//void ScanEditor::setOptionsFile(const QString& options_file)
//{
//  m_options_file = options_file;
//}

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
  return m_image_resolution;
}

void ScanEditor::setResolution(int resolution)
{
  m_image_resolution = resolution;
}

void ScanEditor::setConfigDir(const QString& config_dir)
{
  m_config_dir = config_dir;
}

void ScanEditor::setDataDir(const QString& data_dir)
{
  m_data_dir = data_dir;
}

void ScanEditor::setLang(const QString& lang)
{
  m_lang = lang;
}

QStringList ScanEditor::scanners()
{
  return m_scan_lib->devices();
}

void ScanEditor::initScannerMenu()
{
  QStringList scanners = m_scan_lib->devices();

  if (scanners.isEmpty()) {
    m_scanners_menu->addAction(new QAction(tr("No scanners available")));

  } else {
    m_scanners_menu->clear(); // may not be needed.

    for (int i = 0; i < scanners.size(); i++) {
      ScanDevice* scanner = m_scan_lib->device(scanners.at(i));
      QString s("%1 %2 %3");
      QString n = s
                  .arg(scanner->vendor)
                  .arg(scanner->model)
                  .arg(scanner->type);
      QAction* act = new QAction(n, this);
      act->setData(QVariant::fromValue(scanner));
      m_scanners_menu->addAction(act);
      connect(act, &QAction::triggered, this, &ScanEditor::receiveScannerChange);
      // TODO add appropriate connection
      //      m_scanner_box->addItem(
      //        s.arg(scanner->model, scanner->type, scanner->name));
    }

    m_selected_name = scanners.at(0);

    if (!m_selected_name.isEmpty()) {
      if (m_scan_lib->openDevice(m_selected_name)) {
        ScanDevice* device = m_scan_lib->device(m_selected_name);
        receiveOptionsSet(device);
      }

    } else {
      qCDebug(QscanWidgets) << tr("Unable to open %1").arg(m_selected_name);
    }
  }
}

QList<QMenu*> ScanEditor::menus()
{
  QPixmap scan_icon;
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/scan")), &scan_icon);

  QList<QMenu*> menu_list;
  QString scan_name =  tr("Scan");
  QMenu* scan_menu = new QMenu(scan_name);

  // scan action
  QAction* scan_act = new QAction(scan_icon, tr("Start Scanning"));
  scan_menu->addAction(scan_act);

  // scanners sub-menu
  m_scanners_menu = new QMenu(tr("Scanners"));
  scan_menu->addMenu(m_scanners_menu);

  m_modes_menu = new QMenu(tr("Modes"));
  scan_menu->addMenu(m_modes_menu);

  m_src_menu = new QMenu(tr("Source"));
  scan_menu->addMenu(m_src_menu);

  m_res_menu = new QMenu(tr("Resolution"));
  scan_menu->addMenu(m_res_menu);

  initScannerMenu();

  menu_list.append(scan_menu);
  return menu_list;
}

//QList<QToolBar*> ScanEditor::toolbars()
//{
//  QList<QToolBar*> list;
//  // TODO make toolbars
//  return list;
//}

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
