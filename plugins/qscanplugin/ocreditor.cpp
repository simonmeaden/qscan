#include "ocreditor.h"

#include <qyaml-cpp/QYamlCpp>
#include <yaml-cpp/yaml.h>

#include "imageeditdialog.h"
#include "ocrimage.h"
#include "scanlist.h"
#include "texteditdialog.h"

const QString OcrEditor::TESSERACT = "tesseract";
const QString OcrEditor::LANGUAGE = "language";
const QString OcrEditor::OPTIONS_FILE = QStringLiteral("ocroptions.yaml");

OcrEditor::OcrEditor(QWidget* parent)
  : StackableFrame(parent)
  , m_scan_list(nullptr)
  , m_image_display(nullptr)
  , m_change_type(None)
{
  QDir dir;
  QString config_path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
  config_path += QStringLiteral("/Biblos");
  dir.mkpath(config_path);
  m_options_file = config_path + QDir::separator() + OPTIONS_FILE;

  QString data_dir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
  data_dir = data_dir + QStringLiteral("/Biblos/ocr");

  loadOptions();

  QString datapath = config_path + QStringLiteral("/tesseract/tessdata");
  m_ocr_tools = new OcrTools(datapath, m_lang);

  initGui();
}

QImage OcrEditor::image()
{
  return m_image_display->image();
}

void OcrEditor::setResolution(const QImage& image)
{
  int x_dpi = Util::dpmToDpi(image.dotsPerMeterX());
  int y_dpi = Util::dpmToDpi(image.dotsPerMeterY());
  setResLabel(x_dpi, y_dpi);
}

void OcrEditor::setData(int page_no, const QImage& image, const DocumentData& doc_data, const QString& lang)
{
  m_page_no = page_no;
  m_doc_data = doc_data;
  m_lang = lang;
  m_image_display->setImage(image);
  m_image_display->setInverted(doc_data->inverted());
  QMap<int, StyledString> text_data = doc_data->textList();
  QMap<int, QString> image_data = doc_data->imageList();
  QMap<int, QVariant> data;
  m_item_count = 0;

  for (auto it = text_data.keyBegin(), end = text_data.keyEnd(); it != end; ++it) {
    int key = *it;
    StyledString styled_string(text_data.value(key));
    data.insert(key, styled_string);
    m_item_count = (m_item_count >= *it ? m_item_count : *it);
  }

  for (auto it = image_data.keyBegin(), end = image_data.keyEnd(); it != end; ++it) {
    int key = *it;
    QString path = image_data.value(key);
    QImage image;
    image.load(path, "PNG");
    data.insert(key, image);
    m_item_count = (m_item_count >= *it ? m_item_count : *it);
  }

  if (!data.isEmpty()) {
    m_scan_list->setData(data);
  }

  QFileInfo info(m_doc_data->filename());
  setNameLabel(m_page_no, info.fileName());
  setSizeLabel(image.width(), image.height());

  setResolution(image);

  if (image.format() == QImage::Format_Mono || image.format() == QImage::Format_Grayscale8) {
    m_greyscale_btn->setEnabled(false);
    enableCleanImageBtns(true);
  }
}

void OcrEditor::setOcrImage(int page_no, const QImage& image)
{
  m_page_no = page_no, m_image_display->setImage(image);

  if (image.format() == QImage::Format_Grayscale8) {
    m_greyscale_btn->setEnabled(false);
  }
}

void OcrEditor::setOcrText(int page_no, const QString& text)
{
  if (page_no == m_page_no) {
    m_scan_list->setText(text);
    m_doc_data->appendText(text);
  }
}

void OcrEditor::appendOcrText(int page_no, const QString& text)
{
  if (page_no == m_page_no) {
    m_scan_list->appendText(text);
    m_doc_data->appendText(text);
    m_image_display->clearSelection();
  }
}

QList<StyledString> OcrEditor::texts()
{
  return m_scan_list->texts();
}

bool OcrEditor::imageChanged() const
{
  return m_image_display->imageChanged();
}

bool OcrEditor::hasText()
{
  return (m_scan_list->size() > 0);
}

QImage OcrEditor::modifiedImage()
{
  return m_image_display->currentImage();
}

int OcrEditor::pageNumber()
{
  return m_page_no;
}

DocumentData OcrEditor::documentData() const
{
  return m_doc_data;
}

QList<QMenu*> OcrEditor::menus()
{
  QList<QMenu*> menu_map;
  QString ocr_name =  tr("OCR");
  QMenu* ocr_menu = new QMenu(ocr_name);

  // TODO build menu map.

  menu_map.append(ocr_menu);
  return menu_map;
}

void OcrEditor::loadOptions()
{
  QFile file(m_options_file);

  if (file.exists()) {
    YAML::Node m_options = YAML::LoadFile(file);

    if (m_options[TESSERACT]) {
      YAML::Node tesseract_options = m_options[TESSERACT];

      if (tesseract_options[LANGUAGE]) {
        m_lang = tesseract_options[LANGUAGE].as<QString>();
      }
    }
  }
}

//void OcrEditor::receiveOcrPageRequest(int page_no)
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


QFrame* OcrEditor::initIntSliderFrame()
{
  auto* intvalue_frame = new QFrame(this);
  auto* intvalue_layout = new QGridLayout;
  intvalue_frame->setLayout(intvalue_layout);

  // initialise threshold frame
  int row = 0;
  m_intvalue_slider = new QwtSlider(Qt::Vertical, this);
  m_intvalue_slider->setScalePosition(QwtSlider::LeadingScale);
  m_intvalue_slider->setTrough(true);
  m_intvalue_slider->setGroove(true);
  connect(m_intvalue_slider, &QwtSlider::valueChanged, this, &OcrEditor::setIntValueLabel);
  intvalue_layout->addWidget(m_intvalue_slider, row, 0, Qt::AlignHCenter);

  m_intvalue_lbl = new QLabel(QString::number(m_intvalue_slider->value()), this);
  QFont font;
  font.setWeight(QFont::Bold);
  font.setPixelSize(40);
  m_intvalue_lbl->setFont(font);
  m_intvalue_lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
  intvalue_layout->addWidget(m_intvalue_lbl, row++, 1);

  auto* apply_value_btn = new QPushButton(tr("Apply"), this);
  connect(apply_value_btn, &QPushButton::clicked, this, &OcrEditor::applyValue);
  intvalue_layout->addWidget(apply_value_btn, row++, 0, 1, 2);

  auto* accept_value_btn = new QPushButton(tr("Accept Changes"), this);
  connect(accept_value_btn, &QPushButton::clicked, this, &OcrEditor::acceptChanges);
  intvalue_layout->addWidget(accept_value_btn, row++, 0, 1, 2);

  auto* cancel_change_btn = new QPushButton(tr("Cancel Changes"), this);
  connect(cancel_change_btn,
          &QPushButton::clicked,
          m_image_display,
          &OcrImage::cancelChanges);
  connect(cancel_change_btn, &QPushButton::clicked, this, &OcrEditor::rejectChanges);
  intvalue_layout->addWidget(cancel_change_btn, row++, 0, 1, 2);

  return intvalue_frame;
}

QFrame* OcrEditor::initDoubleSliderFrame()
{
  auto* dblvalue_frame = new QFrame();
  auto* dblvalue_layout = new QGridLayout;
  dblvalue_frame->setLayout(dblvalue_layout);

  // initialise threshold frame
  int row = 0;
  m_dblvalue_slider = new QwtSlider(Qt::Vertical, this);
  m_dblvalue_slider->setScalePosition(QwtSlider::LeadingScale);
  m_dblvalue_slider->setTrough(true);
  m_dblvalue_slider->setGroove(true);
  connect(m_dblvalue_slider, &QwtSlider::valueChanged, this, &OcrEditor::setDoubleValueLabel);
  dblvalue_layout->addWidget(m_dblvalue_slider, row, 0, Qt::AlignHCenter);

  m_dblvalue_lbl = new QLabel(QString::number(m_intvalue_slider->value()), this);
  QFont font;
  font.setWeight(QFont::Bold);
  font.setPixelSize(40);
  m_dblvalue_lbl->setFont(font);
  m_dblvalue_lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
  dblvalue_layout->addWidget(m_dblvalue_lbl, row++, 1);

  auto* apply_value_btn = new QPushButton(tr("Apply"), this);
  connect(apply_value_btn, &QPushButton::clicked, this, &OcrEditor::applyValue);
  dblvalue_layout->addWidget(apply_value_btn, row++, 0, 1, 2);

  auto* accept_value_btn = new QPushButton(tr("Accept Changes"), this);
  connect(accept_value_btn, &QPushButton::clicked, this, &OcrEditor::acceptChanges);
  dblvalue_layout->addWidget(accept_value_btn, row++, 0, 1, 2);

  auto* cancel_change_btn = new QPushButton(tr("Cancel Changes"), this);
  connect(cancel_change_btn,
          &QPushButton::clicked,
          m_image_display,
          &OcrImage::cancelChanges);
  dblvalue_layout->addWidget(cancel_change_btn, row++, 0, 1, 2);

  return dblvalue_frame;
}

QFrame* OcrEditor::initDenoiseSliderFrame()
{
  auto* denoisevalue_frame = new QFrame();
  auto* denoisevalue_layout = new QGridLayout;
  denoisevalue_frame->setLayout(denoisevalue_layout);

  // initialise threshold frame
  int row = 0;
  m_denoise_filter_slider = new QwtSlider(Qt::Vertical, this);
  m_denoise_filter_slider->setScalePosition(QwtSlider::LeadingScale);
  m_denoise_filter_slider->setTrough(true);
  m_denoise_filter_slider->setGroove(true);
  connect(m_denoise_filter_slider, &QwtSlider::valueChanged, this, &OcrEditor::setDenoiseFilterLabel);
  denoisevalue_layout->addWidget(m_denoise_filter_slider, row, 0, Qt::AlignHCenter);

  m_denoise_filter_lbl = new QLabel(QString::number(m_intvalue_slider->value()), this);
  QFont font;
  font.setWeight(QFont::Bold);
  font.setPixelSize(40);
  m_denoise_filter_lbl->setFont(font);
  m_denoise_filter_lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
  denoisevalue_layout->addWidget(m_denoise_filter_lbl, row++, 1);

  m_denoise_template_slider = new QwtSlider(Qt::Vertical, this);
  m_denoise_template_slider->setScalePosition(QwtSlider::LeadingScale);
  m_denoise_template_slider->setTrough(true);
  m_denoise_template_slider->setGroove(true);
  connect(m_denoise_template_slider, &QwtSlider::valueChanged, this, &OcrEditor::setDenoiseTemplateLabel);
  denoisevalue_layout->addWidget(m_denoise_template_slider, row, 0, Qt::AlignHCenter);

  m_denoise_template_lbl = new QLabel(QString::number(m_denoise_template_slider->value()), this);
  m_denoise_template_lbl->setFont(font);
  m_denoise_template_lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
  denoisevalue_layout->addWidget(m_denoise_template_lbl, row++, 1);

  m_denoise_search_slider = new QwtSlider(Qt::Vertical, this);
  m_denoise_search_slider->setScalePosition(QwtSlider::LeadingScale);
  m_denoise_search_slider->setTrough(true);
  m_denoise_search_slider->setGroove(true);
  connect(m_denoise_search_slider, &QwtSlider::valueChanged, this, &OcrEditor::setDenoiseSearchLabel);
  denoisevalue_layout->addWidget(m_denoise_search_slider, row, 0, Qt::AlignHCenter);

  m_denoise_search_lbl = new QLabel(QString::number(m_denoise_search_slider->value()), this);
  m_denoise_search_lbl->setFont(font);
  m_denoise_search_lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
  denoisevalue_layout->addWidget(m_denoise_search_lbl, row++, 1);

  auto* apply_value_btn = new QPushButton(tr("Apply"), this);
  connect(apply_value_btn, &QPushButton::clicked, this, &OcrEditor::applyValue);
  denoisevalue_layout->addWidget(apply_value_btn, row++, 0, 1, 2);

  auto* accept_value_btn = new QPushButton(tr("Accept Changes"), this);
  connect(accept_value_btn, &QPushButton::clicked, this, &OcrEditor::acceptChanges);
  denoisevalue_layout->addWidget(accept_value_btn, row++, 0, 1, 2);

  auto* cancel_change_btn = new QPushButton(tr("Cancel Changes"), this);
  connect(cancel_change_btn,
          &QPushButton::clicked,
          m_image_display,
          &OcrImage::cancelChanges);
  denoisevalue_layout->addWidget(cancel_change_btn, row++, 0, 1, 2);

  return denoisevalue_frame;
}

//QFrame* OcrFrame::initOcrBtns()
//{
//  QFrame* frame = new QFrame(this);
//  frame->setFrameStyle(QFrame::StyledPanel);
//  QVBoxLayout* layout = new QVBoxLayout;
//  frame->setLayout(layout);

//  m_ocr_btn = new QPushButton(tr("Run OCR"), this);
//  m_ocr_btn->setToolTip(tr("Re run the OCR on tweaked image."));
//  connect(m_ocr_btn, &QPushButton::clicked, this, &OcrFrame::requestOcr);
//  layout->addWidget(m_ocr_btn);

////  m_ocr_sel_btn = new QPushButton(tr("Run OCR on Selection"), this);
////  m_ocr_sel_btn->setEnabled(false);
////  m_ocr_sel_btn->setToolTip(tr("Re run the OCR on part of the tweaked image."));
////  connect(m_ocr_sel_btn, &QPushButton::clicked, this, &OcrFrame::requestOcrOnSelection);
////  layout->addWidget(m_ocr_sel_btn);

//  return frame;
//}

QFrame* OcrEditor::initMoveBtns()
{
  QFrame* frame = new QFrame(this);
  frame->setFrameStyle(QFrame::StyledPanel);
  QVBoxLayout* layout = new QVBoxLayout;
  frame->setLayout(layout);

  m_move_sel_to_doc_btn = new QPushButton(tr("Move to Document"), this);
  m_move_sel_to_doc_btn->setEnabled(false);
  m_move_sel_to_doc_btn->setToolTip(tr("Set as an internal Document Image,\n"
                                       "rather than an OCR image."));
  connect(m_move_sel_to_doc_btn, &QPushButton::clicked, this, &OcrEditor::moveToDocument);
  layout->addWidget(m_move_sel_to_doc_btn);

  m_rem_selection_btn = new QPushButton(tr("Remove from Document"), this);
  m_rem_selection_btn->setEnabled(false);
  m_rem_selection_btn->setToolTip(tr("Remove Image/Text from Document."));
  connect(m_rem_selection_btn, &QPushButton::clicked, this, &OcrEditor::removeItemFromDocument);
  layout->addWidget(m_rem_selection_btn);

  return frame;
}

QFrame* OcrEditor::initModifyBtns()
{
  QFrame* frame = new QFrame(this);
  frame->setFrameStyle(QFrame::StyledPanel);
  QVBoxLayout* layout = new QVBoxLayout;
  frame->setLayout(layout);

  m_clr_to_back_btn = new QPushButton(tr("Clear to background"), this);
  m_clr_to_back_btn->setToolTip(tr("Remove section from the image image."));
  m_clr_to_back_btn->setEnabled(false);
  connect(m_clr_to_back_btn, &QPushButton::clicked, m_image_display, &OcrImage::clearToBackground);
  layout->addWidget(m_clr_to_back_btn);

  m_crop_btn = new QPushButton(tr("Crop"), this);
  m_crop_btn->setToolTip(tr("Crop the image to remove excess image."));
  m_crop_btn->setEnabled(false);
  connect(m_crop_btn, &QPushButton::clicked, m_image_display, &OcrImage::cropToSelection);
  layout->addWidget(m_crop_btn);

  m_greyscale_btn = new QPushButton(tr("Convert to Greyscale"), this);
  m_greyscale_btn->setToolTip(tr("Converts a colour image to a greyscale image.\n"
                                 "OCR text is generally better on greyscale images\n"
                                 "than colour images."));
  connect(m_greyscale_btn, &QPushButton::clicked, this, &OcrEditor::greyscale);
  layout->addWidget(m_greyscale_btn);

  m_monochrome_btn = new QPushButton(tr("Grey to Monochrome"), this);
  m_monochrome_btn->setToolTip(tr("Convert a greyscale image to monochrome.\n"
                                  "OCR text is generally better on black and white images\n"
                                  "than greyscale images."));
  connect(m_monochrome_btn, &QPushButton::clicked, this, &OcrEditor::monochrome);
  layout->addWidget(m_monochrome_btn);

  m_invert_btn = new QPushButton(tr("Invert Image"), this);
  m_invert_btn->setEnabled(false);
  m_invert_btn->setToolTip(tr("Converts from black text on white to white text on black and back."));
  connect(m_invert_btn, &QPushButton::clicked, this, &OcrEditor::invert);
  layout->addWidget(m_invert_btn);

  m_denoise_btn = new QPushButton(tr("De-noise"), this);
  m_denoise_btn->setToolTip(tr("Noise removal attempts to clean up the image."));
  connect(m_denoise_btn, &QPushButton::clicked, this, &OcrEditor::denoise);
  layout->addWidget(m_denoise_btn);

  m_dewarp_btn = new QPushButton(tr("De-warp"), this);
  m_dewarp_btn->setToolTip(tr("Straigntens lines that are curved."));
  connect(m_dewarp_btn, &QPushButton::clicked, this, &OcrEditor::dewarp);
  layout->addWidget(m_dewarp_btn);

  auto* rotate_cw_btn = new QPushButton(tr("Rotate CW by 90°"), this);
  rotate_cw_btn->setToolTip(tr("Rotate image clockwise by 90°."));
  connect(rotate_cw_btn, &QPushButton::clicked, this, &OcrEditor::rotateCW);
  layout->addWidget(rotate_cw_btn);

  auto* rotate_ccw_btn = new QPushButton(tr("Rotate CCW by 90°"), this);
  rotate_ccw_btn->setToolTip(tr("Rotate image counter-clockwise by 90°"));
  connect(rotate_ccw_btn, &QPushButton::clicked, this, &OcrEditor::rotateCCW);
  layout->addWidget(rotate_ccw_btn);

  auto* rotate_by_180_btn = new QPushButton(tr("Rotate by 180°"), this);
  rotate_by_180_btn->setToolTip(tr("Rotate image by 180°."));
  connect(rotate_by_180_btn, &QPushButton::clicked, this, &OcrEditor::rotate180);
  layout->addWidget(rotate_by_180_btn);

  m_deskew_btn = new QPushButton(tr("De-skew"), this);
  m_deskew_btn->setToolTip(tr("Allows rotation non-horizontal lines.\n"
                              "To use click deskew button, select a start point\n"
                              "using your mouse, then drag a line parallel\n"
                              "to the skewed text, holding down the left mouse button.\n"
                              "The text will automatically be rotated by the\n"
                              "correct amount."));
  connect(m_deskew_btn, &QPushButton::clicked, this, &OcrEditor::deskew);
  layout->addWidget(m_deskew_btn);

  auto* rescale_btn = new QPushButton(tr("Rescale"), this);
  rescale_btn->setToolTip(tr("Increase the DPI of the image."));
  connect(rescale_btn, &QPushButton::clicked, this, &OcrEditor::rescale);
  layout->addWidget(rescale_btn);

  auto* undo_btn = new QPushButton(tr("Undo last operation"), this);
  undo_btn->setToolTip(tr("Undo changes and revert to previous image."));
  connect(undo_btn, &QPushButton::clicked, this, &OcrEditor::undoChanges);
  layout->addWidget(undo_btn);

  return frame;
}

QFrame* OcrEditor::initFinaliseBtns()
{
  QFrame* frame = new QFrame(this);
  frame->setFrameStyle(QFrame::StyledPanel);
  QVBoxLayout* layout = new QVBoxLayout;
  frame->setLayout(layout);

  m_save_txt_btn = new QPushButton(tr("Save Data"), this);
  m_save_txt_btn->setEnabled(false);
  m_save_txt_btn->setToolTip(tr("Save the text and document images."));
  connect(m_save_txt_btn, &QPushButton::clicked, this, &OcrEditor::saveData);
  layout->addWidget(m_save_txt_btn);

  m_save_img_btn = new QPushButton(tr("Save OCR Image"), this);
  m_save_img_btn->setToolTip(tr("Save the OCR image for later."));
  connect(m_save_img_btn, &QPushButton::clicked, this, &OcrEditor::saveCurrentStateImage);
  layout->addWidget(m_save_img_btn);

  m_close_and_save_btn = new QPushButton(tr("Close and Save"), this);
  m_close_and_save_btn->setToolTip(tr("Closes the dialog, saving any changes."));
  connect(m_close_and_save_btn, &QPushButton::clicked, this, &OcrEditor::saveData);
  connect(m_close_and_save_btn, &QPushButton::clicked, this, &OcrEditor::saveCurrentStateImage);
  connect(m_close_and_save_btn, &QPushButton::clicked, this, &OcrEditor::close);
  layout->addWidget(m_close_and_save_btn);

  auto* close_btn = new QPushButton(tr("Close without save"), this);
  close_btn->setToolTip(tr("Closes the modification page but doesn't save data."));
  connect(close_btn, &QPushButton::clicked, this, &OcrEditor::close);
  layout->addWidget(close_btn);

  auto* completed_btn = new QPushButton(tr("OCR is complete"), this);
  completed_btn->setToolTip(tr("You have finished work on the OCR image\n"
                               "and you can delete the image."));
  connect(completed_btn, &QPushButton::clicked, this, &OcrEditor::completeOperation);
  layout->addWidget(completed_btn);

  return frame;
}

void OcrEditor::initGui()
{
  auto* layout = new QGridLayout;
  setLayout(layout);

  m_scan_list = new ScanList(this);
  connect(m_scan_list, &ScanList::clicked, this, &OcrEditor::scanListWasClicked);
  connect(m_scan_list, &ScanList::doubleClicked, this, &OcrEditor::scanListWasDoubleClicked);
  connect(m_scan_list, &ScanList::dataHasChanged, this, &OcrEditor::scanListDataChanged);
  connect(m_scan_list, &ScanList::imageHasChanged, this, &OcrEditor::scanListDataChanged);
  layout->addWidget(m_scan_list, 0, 0);

  auto* stacked_frame = new QFrame(this);
  m_ctl_stack = new QStackedLayout;
  stacked_frame->setLayout(m_ctl_stack);
  layout->addWidget(stacked_frame, 0, 1);

  m_image_name = tr("Page %1 : %2");
  m_image_size = tr("Size : %1 x %2");
  m_resolution = tr("Resolution X : %1dpi , Y: %2dpi");

  m_status_bar = new QStatusBar(this);

  m_message_lbl = new QLabel(this);
  m_message_lbl->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  m_message_lbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_status_bar->addPermanentWidget(m_message_lbl, 2);

  m_image_name_lbl = new QLabel(m_image_name.arg(0).arg(tr("No File")), this);
  m_image_name_lbl->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  m_image_name_lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_status_bar->addPermanentWidget(m_image_name_lbl);

  m_image_size_lbl = new QLabel(m_image_size.arg(0).arg(0), this);
  m_image_size_lbl->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  m_image_size_lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_status_bar->addPermanentWidget(m_image_size_lbl);

  m_res_lbl = new QLabel(m_resolution.arg(0).arg(0), this);
  m_res_lbl->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  m_res_lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_status_bar->addPermanentWidget(m_res_lbl);

  layout->addWidget(m_status_bar, 1, 0, 1, 3);

  // This needs to be done before threshold/btns
  // so m_image_display exists for them to connect to.
  m_image_display = new OcrImage(this);
  connect(m_image_display, &OcrImage::selected, this, &OcrEditor::setSelected);
  connect(m_image_display, &OcrImage::unselected, this, &OcrEditor::setUnselected);
  connect(m_image_display, &OcrImage::imageSizeChanged, this, &OcrEditor::imageSizeHasChanged);
  layout->addWidget(m_image_display, 0, 2);

  auto* intvalue_frame = initIntSliderFrame();
  m_intvalue_stack = m_ctl_stack->addWidget(intvalue_frame);

  QFrame* dblvalue_frame = initDoubleSliderFrame();
  m_dblvalue_stack = m_ctl_stack->addWidget(dblvalue_frame);

  QFrame* denoisevalue_frame = initDenoiseSliderFrame();
  m_denoise_stack = m_ctl_stack->addWidget(denoisevalue_frame);

  auto* btn_box = new QFrame(this);
  auto* btn_layout = new QVBoxLayout;
  btn_box->setLayout(btn_layout);
  m_btn_stack = m_ctl_stack->addWidget(btn_box);
  {
    // initialise button box
    m_ocr_btn = new QPushButton(tr("Run OCR"), this);
    m_ocr_btn->setToolTip(tr("Re run the OCR on tweaked image."));
    connect(m_ocr_btn, &QPushButton::clicked, this, &OcrEditor::requestOcr);
    btn_layout->addWidget(m_ocr_btn);

    //    btn_layout->addStretch(1);
    btn_layout->addWidget(initMoveBtns());
    //    btn_layout->addStretch(1);
    btn_layout->addWidget(initModifyBtns());
    //    btn_layout->addStretch(1);
    btn_layout->addWidget(initFinaliseBtns());
    //    btn_layout->addStretch(1);

    auto* help_btn = new QPushButton(tr("Help"), this);
    help_btn->setToolTip(tr("Help."));
    connect(help_btn, &QPushButton::clicked, this, &OcrEditor::help);
    btn_layout->addWidget(help_btn);

    enableCleanImageBtns(false);
  }

  m_ctl_stack->setCurrentIndex(m_btn_stack);

  layout->setColumnStretch(0, 3);
  layout->setColumnStretch(1, 1);
  layout->setColumnStretch(2, 3);
}

void OcrEditor::requestOcr()
{
  if (m_image_display->hasSelection()) {
    requestOcrOnSelection();

  } else {
    QImage image = m_image_display->currentImage();
    emit sendOcrRequest(m_page_no, image);
  }
}

void OcrEditor::requestOcrOnSelection()
{
  QRect copy_rect = m_image_display->selection();
  QImage image = m_image_display->currentImage();
  QImage cropped = image.copy(copy_rect);
  QFileInfo info("rect_image.png");
  cropped.save(info.filePath(), "PNG");

  if (!copy_rect.isNull()) {
    emit sendOcrRequest(m_page_no, image, copy_rect);
  }
}

void OcrEditor::help()
{
  // TODO help
}

void OcrEditor::setSelected()
{
  m_crop_btn->setEnabled(true);
  m_clr_to_back_btn->setEnabled(true);
  //  m_ocr_sel_btn->setEnabled(true);
  m_move_sel_to_doc_btn->setEnabled(true);
}

void OcrEditor::setUnselected()
{
  m_crop_btn->setEnabled(false);
  m_clr_to_back_btn->setEnabled(false);
  //  m_ocr_sel_btn->setEnabled(false);
  m_move_sel_to_doc_btn->setEnabled(false);
}

void OcrEditor::crop()
{
  m_image_display->cropToSelection();
}

void OcrEditor::greyscale()
{
  m_change_type = Greyscale;
  setCurrentChangeState();
  m_image_display->greyscale();
}

void OcrEditor::monochrome()
{
  m_change_type = Monochrome;
  setCurrentChangeState();
}

void OcrEditor::invert()
{
  m_image_display->invert();
  enableCleanImageBtns(true);
  m_doc_data->setInverted(m_image_display->isInverted());
}

void OcrEditor::denoise()
{
  m_change_type = Denoise;
  setCurrentChangeState();
  m_image_display->denoise();
}

void OcrEditor::dewarp()
{
  // TODO dewarp
}

void OcrEditor::deskew()
{
  m_image_display->deskew();
}

void OcrEditor::rotate180()
{
  m_image_display->rotate180();
}

void OcrEditor::rotateCW()
{
  m_image_display->rotateCW();
}

void OcrEditor::rotateCCW()
{
  m_image_display->rotateCCW();
}

void OcrEditor::rescale()
{
  m_change_type = Rescale;
  m_image_display->rescale();
  setCurrentChangeState();
}

void OcrEditor::moveToDocument()
{
  QImage image = m_image_display->copySelection();
  int image_index = m_scan_list->appendImage(image);

  emit saveSelectedDocumentImage(m_page_no, image_index, image, m_doc_data);
}

void OcrEditor::scanListWasClicked(const QModelIndex& /*index*/)
{
  if (!m_scan_list->selectionModel()->selection().empty()) {
    m_rem_selection_btn->setEnabled(true);

  } else {
    m_rem_selection_btn->setEnabled(false);
  }
}

void OcrEditor::scanListWasDoubleClicked(const QModelIndex& index)
{
  QVariant value = index.data();
  int row = index.row();

  if (value.type() == QVariant::String) {
    auto* dlg = new TextEditDialog(this);
    dlg->setText(value.toString());

    if (dlg->exec() == QDialog::Accepted) {
      StyledString text = dlg->text();
      m_scan_list->replaceText(index.row(), text);
    }

  } else if (value.type() == QVariant::Image) {
    auto* dlg = new ImageEditDialog(this);
    dlg->setImage(value.value<QImage>());

    if (dlg->exec() == QDialog::Accepted) {
      QImage image = dlg->image();
      m_scan_list->replaceImage(row, image);
    }
  }
}

void OcrEditor::completeOperation()
{
  if (m_scan_list->hasDataChanges()) {
    QMessageBox dlg(QMessageBox::Warning,
                    tr("Complete OCR Operation"),
                    tr("You are marking this image as complete. Any outstanding\n"
                       "changes you have made since the last save are not saved.\n"
                       "to save the data press the 'Leave with save' button, to leave without\n"
                       "save press the 'Leave no save' button, to continue editing press the"
                       "'Cancel' button."),
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Save,
                    this);
    dlg.setButtonText(QMessageBox::Yes, tr("Leave no save"));
    dlg.setButtonText(QMessageBox::No, tr("Cancel"));
    dlg.setButtonText(QMessageBox::Save, tr("Leave with save"));
    int result = dlg.exec();

    if (result == QMessageBox::Yes) {
      emit makeCompleted(m_doc_data);

    } else if (result == QMessageBox::Save) {
      saveCurrentStateImage();
      saveData();
      m_doc_data->setInverted(m_image_display->isInverted());
      emit makeCompleted(m_doc_data);
    }
  }
}

void OcrEditor::removeItemFromDocument()
{
  auto answer = QMessageBox::warning(this,
                                     tr("Remove Item From Document"),
                                     tr("You are about to remove an item from the Document\n"
                                        "This cannot be undone!"
                                        "Ary you sure?"),
                                     QMessageBox::Yes | QMessageBox::No,
                                     QMessageBox::No);

  if (answer == QMessageBox::Yes) {
    auto index = m_scan_list->selectionModel()->currentIndex();

    if (index.isValid()) {
      m_scan_list->removeDataRow(index);
    }
  }
}

void OcrEditor::saveData()
{
  emit saveModifiedData(m_doc_data);
}

void OcrEditor::saveCurrentStateImage()
{
  //  emit saveModifiedImage(m_page_no, m_image_display->currentImage());
  m_image_display->setImageChanged(false);
  //  emit reject();
}

//void OcrFrame::discard()
//{
//  if (m_image_display->hasImageChanges() || m_scan_list->hasDataChanges()) {
//    int result = QMessageBox::warning(this,
//                                      tr("Discaring Changes"),
//                                      tr("You are about to discard any changes you have made\n"
//                                         "and quit the dialog.\n"
//                                         "This cannot be undone\n"
//                                         "Are you sure?"),
//                                      QMessageBox::Yes | QMessageBox::No,
//                                      QMessageBox::No);

//    if (result == QMessageBox::Yes) {
//      m_image_display->setImageChanged(false);
//      m_scan_list->dumpData();
//      emit reject();
//    }

//  } else {
//    m_image_display->setImageChanged(false);
//    m_scan_list->dumpData();
//    emit reject();
//  }
//}

void OcrEditor::acceptChanges()
{
  m_image_display->acceptChanges();
  m_ctl_stack->setCurrentIndex(m_btn_stack);
  emit makeCompleted(m_doc_data);
}

void OcrEditor::undoChanges()
{
  int result = QMessageBox::warning(this,
                                    tr("Discarding Changes"),
                                    tr("You are about to discard any changes you have made\n"
                                       "This cannot be undone\n"
                                       "Are you sure?"),
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No);

  if (result == QMessageBox::Yes) {
    m_image_display->undoAllChanges();
    emit rejectChanges();
  }
}

void OcrEditor::close()
{
  if (m_scan_list->hasDataChanges()) {
    int result = QMessageBox::warning(this,
                                      tr("Close the modification page"),
                                      tr("You are about to leave the modification page.\n"
                                         "THERE ARE UNSAVED CHANGES. THIS CANNOT BE UNDONE.\n"
                                         "Are you sure?"),
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No);

    if (result == QMessageBox::Yes) {
      //      saveCurrentStateImage();
      //      saveData();
      //      m_doc_data->setInverted(m_image_display->isInverted());
      emit rejectChanges();
      return;
    }
  }

  emit rejectChanges();
}

void OcrEditor::scanListDataChanged()
{
  if (m_scan_list->hasDataChanges()) {
    m_save_txt_btn->setEnabled(true);
    m_close_and_save_btn->setEnabled(true);

  } else {
    m_save_txt_btn->setEnabled(false);

    if (!m_image_display->hasImageChanges()) {
      m_close_and_save_btn->setEnabled(false);
    }
  }
}

void OcrEditor::ocrImageChanged(bool changed)
{
  m_save_img_btn->setEnabled(changed);

  if (!m_scan_list->hasDataChanges()) {
    m_close_and_save_btn->setEnabled(changed);
  }
}

void OcrEditor::applyValue()
{
  switch (m_change_type) {
  case Monochrome: {
    int value = qRound(m_intvalue_slider->value());
    m_image_display->applyThreshold(value);
    break;
  }

  case Rescale: {
    qreal value = Util::round(m_dblvalue_slider->value(), 2);
    m_image_display->applyRescale(value);
    break;
  }

  case Denoise: {
    int filter_value = int(m_denoise_filter_slider->value());
    int template_value = Util::makeOdd(m_denoise_template_slider->value());
    int search_value = Util::makeOdd(m_denoise_search_slider->value());
    m_image_display->applyDenoise(filter_value, template_value, search_value);

  }

  default:
    break;
  }
}
void OcrEditor::setCurrentChangeState()
{
  QList<double> major_ticks, medium_ticks, minor_ticks;

  switch (m_change_type) {
  case Greyscale:
    setMessageLabel(tr("Binarising image"));
    break;

  case Monochrome:
    for (qreal i = 0; i < 250; i += 50) {
      major_ticks << i;
    }

    major_ticks << 255;

    for (qreal i = 0; i < 255; i += 10) {
      medium_ticks << i;
    }

    for (qreal i = 0; i <= 255; i += 5) {
      minor_ticks << i;
    }

    m_intvalue_slider->setScaleEngine(new QwtLinearScaleEngine());
    m_intvalue_slider->setScale(QwtScaleDiv(0, 255, minor_ticks, medium_ticks, major_ticks));
    m_intvalue_slider->setValue(OcrImage::BASE_THRESHOLD);
    m_ctl_stack->setCurrentIndex(m_intvalue_stack);
    setMessageLabel(tr("Convert image to Monochrome"));
    break;

  case Rescale:

    for (qreal i = 0.2; i < 1.0; i += 0.1) {
      major_ticks << i;
    }

    medium_ticks.clear();

    for (qreal i = 1.0; i <= 5.0; i += 1.0) {
      major_ticks << i;
    }

    for (qreal i = 0.2; i < 1.0; i += 0.01) {
      minor_ticks << i;
    }

    for (qreal i = 1.0; i < 5.0; i += 0.1) {
      minor_ticks << i;
    }

    m_dblvalue_slider->setScaleEngine(new QwtLogScaleEngine());
    m_dblvalue_slider->setScale(QwtScaleDiv(0.2, 5.0, minor_ticks, medium_ticks, major_ticks));
    m_dblvalue_slider->setValue(OcrImage::BASE_RESCALE);
    m_ctl_stack->setCurrentIndex(m_dblvalue_stack);
    setMessageLabel(tr("Rescaling image"));
    break;

  case Denoise:

    m_denoise_filter_slider->setScaleEngine(new QwtLinearScaleEngine());
    //    m_denoise_template_slider->setScale(QwtScaleDiv(0, 255, minor_ticks, medium_ticks, major_ticks));
    m_denoise_filter_slider->setValue(OcrImage::BASE_FILTER);
    m_denoise_filter_slider->setScale(1, 29);
    m_denoise_filter_slider->setSingleSteps(1);
    m_denoise_filter_slider->setPageSteps(1);

    m_denoise_template_slider->setValue(OcrImage::BASE_TEMPLATE);
    m_denoise_template_slider->setSingleSteps(2);
    m_denoise_template_slider->setPageSteps(10);

    m_denoise_search_slider->setValue(OcrImage::BASE_SEARCH);
    m_denoise_search_slider->setSingleSteps(2);
    m_denoise_search_slider->setPageSteps(10);

    m_ctl_stack->setCurrentIndex(m_denoise_stack);
    setMessageLabel(tr("Denoising image"));
    break;
  }
}

void OcrEditor::setIntValueLabel(qreal value)
{
  m_intvalue_lbl->setText(QString::number(int(value)));
}

void OcrEditor::setDoubleValueLabel(qreal value)
{
  m_dblvalue_lbl->setText(tr("%1").arg(value, 0, 'g', 2));
}

void OcrEditor::setDenoiseTemplateLabel(qreal value)
{
  m_denoise_template_lbl->setText(QString::number(Util::makeOdd(value)));
}

void OcrEditor::setDenoiseSearchLabel(qreal value)
{
  m_denoise_search_lbl->setText(QString::number(Util::makeOdd(value)));
}

void OcrEditor::setDenoiseFilterLabel(qreal value)
{
  m_denoise_filter_lbl->setText(QString::number(int(value)));
}

void OcrEditor::valueAccepted()
{
  m_ctl_stack->setCurrentIndex(m_btn_stack);
}

void OcrEditor::disableBinarise()
{
  m_greyscale_btn->setEnabled(false);
  enableCleanImageBtns(true);
}

void OcrEditor::enableBinarise()
{
  m_greyscale_btn->setEnabled(true);
}

void OcrEditor::enableCleanImageBtns(bool enable)
{
  m_invert_btn->setEnabled(enable);
  m_denoise_btn->setEnabled(enable);
  m_dewarp_btn->setEnabled(enable);
  m_deskew_btn->setEnabled(enable);
}

void OcrEditor::imageSizeHasChanged(int width, int height, int xres, int yres)
{
  setSizeLabel(width, height);
  setResLabel(xres, yres);
}

void OcrEditor::resizeEvent(QResizeEvent* event)
{
  QSize parent_size = qobject_cast<QWidget*>(parent())->frameSize();
  auto size = parent_size;
  size -= QSize(100, 100);

  auto* e = new QResizeEvent(size, event->oldSize());
  QFrame::resizeEvent(e);
}

void OcrEditor::setMessageLabel(const QString& text)
{
  m_message_lbl->setText(text);
}

void OcrEditor::setSizeLabel(int width, int height)
{
  m_image_size_lbl->setText(m_image_size.arg(width).arg(height));
}

void OcrEditor::setNameLabel(int page, QString name)
{
  m_image_name_lbl->setText(m_image_name.arg(page).arg(name));
}

void OcrEditor::setResLabel(int xres, int yres)
{
  m_res_lbl->setText(m_resolution.arg(xres).arg(yres));
}
