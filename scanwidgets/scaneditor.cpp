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

#include <yaml-cpp/yaml.h>
#include <qyaml-cpp/qyaml-cpp.h>

#include "qscan.h"
#include "scaneditor.h"

#include "ocrdialog.h"
#include "ocrtools.h"


/* ScanEditor
 *****************************************************************************/
/*!
  \class ScanEditor.

*/

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
                       QString& configdir,
                       QString datadir,
                       QWidget* parent)
  : QFrame(parent)
  , m_scan_lib(scan)
  , m_config_dir(std::move(configdir))
  , m_data_dir(std::move(datadir))
  , m_cover(Page(new ScanPage()))
  , m_lang("eng")
{
  m_logger = Log4Qt::Logger::logger(tr("ScanEditor"));

  m_options_file = m_config_dir + QDir::separator() + OPTIONS_FILE;

  m_ocr_tools = new OcrTools(configdir, m_lang);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  initGui();
  makeConnections();
}

void ScanEditor::initGui()
{
  auto* layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
  m_scroller = new QScrollArea(this);
  m_scroller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_scroller->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  m_scroller->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  m_scroller->setContentsMargins(0, 0, 0, 0);
  m_scan_display = new ScanImage(m_data_dir, this);
  m_scan_display->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  m_scan_display->setScaledContents(true);
  m_scroller->setWidget(m_scan_display);
  layout->addWidget(m_scroller, 0, 0);
  m_page_view = new PageView(this);
  m_page_view->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_page_view, 0, 1);
  layout->setColumnStretch(0, 30);
  layout->setColumnStretch(1, 10);
}

void ScanEditor::makeConnections()
{
  connect(m_scan_display, &ScanImage::selectionUnderway, this, &ScanEditor::selectionUnderway);
  connect(m_scan_display, &ScanImage::selected, this, &ScanEditor::selected);
  connect(m_scan_display, &ScanImage::unselected, this, &ScanEditor::unselected);
  connect(m_scan_display, &ScanImage::imageIsLoaded, this, &ScanEditor::imageIsLoaded);
  connect(m_scan_display, &ScanImage::adjustScrollbar, this, &ScanEditor::adjustScrollbar);
  connect(m_scan_display, &ScanImage::sendImage, this, &ScanEditor::receiveImage);
  connect(m_scan_display, &ScanImage::sendImages, this, &ScanEditor::receiveImages);
  connect(m_page_view, &PageView::sendOcrPage, this, &ScanEditor::receiveOcrRequest);
  connect(m_page_view, &PageView::clearSaveAllFlag, this, &ScanEditor::clearSaveAllTextsFlag);
  connect(m_ocr_tools, &OcrTools::converted, this, &ScanEditor::receiveOcrResult);
  connect(m_scan_display, &ScanImage::sendCover, this, &ScanEditor::saveAsCover);
}

/*
   Receives a single page image from the ScanImage object
   and stores it.
*/
void ScanEditor::receiveImage(const QImage& image)
{
  Page page(new ScanPage());
  int index = m_pages.size() + 1;
  m_page_view->append(thumbnail(image));
  QString path = saveImage(index, image);
  page->setImage(path);
  m_pages.insert(index, page);
}

/*
   Receives two split page images from the ScanImage object
   and stores them.
*/
void ScanEditor::receiveImages(const QImage& left, const QImage& right)
{
  Page left_page(new ScanPage());
  int index = m_pages.size() + 1;
  m_pages.insert(index, left_page);
  m_page_view->append(thumbnail(left));
  QString path = saveImage(index, left);
  left_page->setImage(path);

  Page right_page(new ScanPage());
  index++;
  m_pages.insert(index, right_page);
  m_page_view->append(thumbnail(right));
  path = saveImage(index, right);
  right_page->setImage(path);
}

void ScanEditor::receiveString(int page, const QString& str)
{
  if (page > 0) { // 0 is cover page so no text.
    m_pages.value(page)->setText(str);
  }
}

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
      path += QDir::separator() + QString("image%1.png").arg(index);

      if (!image.save(path, "PNG", 100)) {
        m_logger->info(tr("Failed to save image %1").arg(path));
      }
    }

  } else {
    path = m_data_dir + QDir::separator() + m_current_doc_name + QDir::separator() +
           QString("image%1.png").arg(index);

    if (!image.save(path, "PNG", 100)) {
      m_logger->info(tr("Failed to save image %1").arg(path));
    }
  }

  return path;
}

void ScanEditor::saveAsCover(const QImage& image)
{
  if (!m_current_doc_name.isEmpty()) {
    // TODO no doc name yet.
  } else {
    // always
    m_page_view->setCover(thumbnail(image));
    QString path =
      m_data_dir + QDir::separator() + m_current_doc_name + QDir::separator() + "cover.png";

    if (!image.save(path, "PNG", 100)) {
      m_logger->info(tr("failed to save file path"));
      return;
    }

    m_cover->setImage(path);
  }
}

void ScanEditor::receiveOcrRequest(int index)
{
  if (index > 0) { //
    Page page = m_pages.value(index);

    if (page->text().isEmpty()) {
      m_ocr_tools->convertImageToText(page);
    }

  } else {
    // This will never be called normally as the cover page is filtered out
    // in the PageView. However someone might use this somewhere else.
    QMessageBox::warning(this, tr("Cover image."), tr("You cannot OCR the cover image."));
  }
}

void ScanEditor::saveText(int index, const Page& page)
{
  QString filename = m_data_dir + QDir::separator() + m_current_doc_name + QDir::separator() +
                     QString("text%1.txt").arg(index);
  QFile file(filename);

  if (file.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream stream(&file);
    stream << page->text();
    file.close();
    m_page_view->setHasText(index, true);
  }
}

void ScanEditor::receiveOcrResult(const Page& page)
{
  auto* dlg = new OCRDialog(this);
  QImage image(page->imagePath(), "PNG");
  dlg->setImage(image);

  if (!page->text().isEmpty()) {
    dlg->setOcrText(page->text());
  }

  if (dlg->exec() == QDialog::Accepted) {
    page->setText(dlg->text());
    int index = m_pages.key(page);

    if (m_save_all_texts) {
      saveText(index, page);

    } else {
      int result = QMessageBox::question(this,
                                         tr("Image text exists."),
                                         tr("The image %1 already has text.\n"
                                            "Do you want to overwrite this text?").arg(index),
                                         QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll,
                                         QMessageBox::No);

      if (result == QMessageBox::Yes) {
        saveText(index, page);

      } else if (result == QMessageBox::YesToAll) {
        m_save_all_texts = true;
        saveText(index, page);
      }
    }

    if (dlg->imageChanged()) {
      // TODO maybe save tweaked image. maybe only supply list of tweaks?
    }
  }
}

/*!
   \brief Splits a single page image into two equal size page images.
*/
void ScanEditor::splitPages()
{
  QPair<QImage, QImage> images = m_scan_display->splitPages();
  receiveImages(images.first, images.second);
}

/*!
   \brief Splits a single page image into two equal size page images but
   discards the right hand page.
*/
void ScanEditor::splitLeftPage()
{
  QImage image = m_scan_display->splitLeftPage();
  receiveImage(image);
}

/*!
   \brief Splits a single page image into two equal size page images but
   discards the left hand page.
*/
void ScanEditor::splitRightPage()
{
  QImage image = m_scan_display->splitRightPage();
  receiveImage(image);
}

/*!
   \brief Takes the image and makes it into a single page.
*/
void ScanEditor::makePage()
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
  QString filename = current_path + "cover.png";
  QFile file(filename);
  QImage image;
  Page page;
  QStringList filter;
  filter << "image*.png";

  if (file.exists()) {
    loadCover(filename);
    m_logger->info(tr("Cover file loaded)"));
  }

  QDir dir(current_path);
  int index = 0;
  bool ok;

  for (const QString& fname : dir.entryList(filter, QDir::Files)) {
    filename = current_path + fname;
    file.setFileName(filename);

    if (file.exists()) {
      index = fname.midRef(5, fname.length() - 9).toInt(&ok);

      if (ok) { // is an int value
        image = QImage(filename, "PNG");
        page = Page(new ScanPage(this));
        page->setImage(filename);
        m_pages.insert(index, page);
        m_page_view->insert(index, thumbnail(image), false);
        m_logger->info(tr("Image file %1 loaded)").arg(fname));
      }
    }
  } // end image file list

  filter.clear();
  filter << "page*.txt";

  filename = current_path + QString("page%1.txt").arg(index);
  file.setFileName(filename);

  for (const QString& fname : dir.entryList(filter, QDir::Files)) {
    filename = current_path + fname;
    file.setFileName(filename);

    if (file.exists()) {
      index = fname.midRef(4, fname.length() - 8).toInt(&ok);

      if (ok) { // is an int value
        if (m_pages.contains(index)) {
          page = m_pages.value(index);

        } else {
          // should only happen if an image has been removed.
          page = Page(new ScanPage(this));
          m_page_view->insert(index, QImage(), false);
        }

        if (file.open(QFile::ReadOnly)) {
          QTextStream stream(&file);
          QString text = stream.readAll();
          page->setText(text);
          m_page_view->setHasText(index, true);
          m_logger->info(tr("Text file %1 loaded)").arg(fname));
        }
      }
    }
  } // end text file list
}

/*!
   \brief Loads the options file.

  Currently the options only include the current document name, which is used as a name
  for the image/text storage directory and the Tesseract language. If you do not call
  loadOptions(), for example if you do not want to use the default yaml options then you
  must set these values manually using setTesseractLanguage() and setDocumentname().

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

  Currently the options only include the current document name, which is used as a name
  for the image/text storage directory and the Tesseract language. If you do not call
  saveOptions(), for example if you do not want to use the default yaml options then you
  must set these values manually using setTesseractLanguage() and setDocumentname().

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
  Page page(new ScanPage());
  page->setImage(filename);
  m_cover = page;
  m_page_view->setCover(thumbnail(image));
}

void ScanEditor::setImage(const QImage& image)
{
  if (m_prog_dlg) {
    m_prog_dlg->close();
    m_prog_dlg = nullptr;
  }

  m_scan_display->setImage(image);
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
  scrollbar->setValue(
    int(factor * scrollbar->value() + ((factor - 1) * scrollbar->pageStep() / 2)));
  scrollbar = m_scroller->verticalScrollBar();
  scrollbar->setValue(
    int(factor * scrollbar->value() + ((factor - 1) * scrollbar->pageStep() / 2)));
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
  m_scan_display->rotateACW();
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
  m_scan_display->scale();
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
  return m_pages.size();
}

Page ScanEditor::page(int index)
{
  return m_pages.value(index);
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

void ScanEditor::clearSaveAllTextsFlag()
{
  m_save_all_texts = false;
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

  return  QImage();
}
