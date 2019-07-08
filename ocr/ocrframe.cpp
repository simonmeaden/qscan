#include "ocrframe.h"

#include "imageeditdialog.h"
#include "ocrimage.h"
#include "scanlist.h"
#include "texteditdialog.h"

OcrFrame::OcrFrame(QWidget* parent)
  : QFrame(parent)
  , m_scan_list(nullptr)
  , m_image_display(nullptr)
  , m_change_type(None)
{
  initGui();
}

QImage OcrFrame::image()
{
  return m_image_display->image();
}

void OcrFrame::setResolution(const QImage& image)
{
  int x_dpi = Util::dpmToDpi(image.dotsPerMeterX());
  int y_dpi = Util::dpmToDpi(image.dotsPerMeterY());
  setResLabel(x_dpi, y_dpi);
}

void OcrFrame::setData(int page_no, const QImage& image, const DocumentData& doc_data)
{
  m_page_no = page_no;
  m_doc_data = doc_data;
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

void OcrFrame::setOcrImage(int page_no, const QImage& image)
{
  m_page_no = page_no, m_image_display->setImage(image);

  if (image.format() == QImage::Format_Grayscale8) {
    m_greyscale_btn->setEnabled(false);
  }
}

void OcrFrame::setOcrText(int page_no, const QString& text)
{
  if (page_no == m_page_no) {
    m_scan_list->setText(text);
    m_doc_data->appendText(text);
  }
}

void OcrFrame::appendOcrText(int page_no, const QString& text)
{
  if (page_no == m_page_no) {
    m_scan_list->appendText(text);
    m_doc_data->appendText(text);
    m_image_display->clearSelection();
  }
}

QList<StyledString> OcrFrame::texts()
{
  return m_scan_list->texts();
}

bool OcrFrame::imageChanged() const
{
  return m_image_display->imageChanged();
}

bool OcrFrame::hasText()
{
  return (m_scan_list->size() > 0);
}

QImage OcrFrame::modifiedImage()
{
  return m_image_display->currentImage();
}

int OcrFrame::pageNumber()
{
  return m_page_no;
}

DocumentData OcrFrame::documentData() const
{
  return m_doc_data;
}

QFrame* OcrFrame::initIntSliderFrame()
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
  connect(m_intvalue_slider, &QwtSlider::valueChanged, this, &OcrFrame::setIntValueLabel);
  intvalue_layout->addWidget(m_intvalue_slider, row, 0, Qt::AlignHCenter);

  m_intvalue_lbl = new QLabel(QString::number(m_intvalue_slider->value()), this);
  QFont font;
  font.setWeight(QFont::Bold);
  font.setPixelSize(40);
  m_intvalue_lbl->setFont(font);
  m_intvalue_lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
  intvalue_layout->addWidget(m_intvalue_lbl, row++, 1);

  auto* apply_value_btn = new QPushButton(tr("Apply"), this);
  connect(apply_value_btn, &QPushButton::clicked, this, &OcrFrame::applyValue);
  intvalue_layout->addWidget(apply_value_btn, row++, 0, 1, 2);

  auto* accept_value_btn = new QPushButton(tr("Accept Changes"), this);
  connect(accept_value_btn, &QPushButton::clicked, this, &OcrFrame::acceptChanges);
  intvalue_layout->addWidget(accept_value_btn, row++, 0, 1, 2);

  auto* cancel_change_btn = new QPushButton(tr("Cancel Changes"), this);
  connect(cancel_change_btn,
          &QPushButton::clicked,
          m_image_display,
          &OcrImage::cancelChanges);
  connect(cancel_change_btn, &QPushButton::clicked, this, &OcrFrame::rejectChanges);
  intvalue_layout->addWidget(cancel_change_btn, row++, 0, 1, 2);

  return intvalue_frame;
}

QFrame* OcrFrame::initDoubleSliderFrame()
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
  connect(m_dblvalue_slider, &QwtSlider::valueChanged, this, &OcrFrame::setDoubleValueLabel);
  dblvalue_layout->addWidget(m_dblvalue_slider, row, 0, Qt::AlignHCenter);

  m_dblvalue_lbl = new QLabel(QString::number(m_intvalue_slider->value()), this);
  QFont font;
  font.setWeight(QFont::Bold);
  font.setPixelSize(40);
  m_dblvalue_lbl->setFont(font);
  m_dblvalue_lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
  dblvalue_layout->addWidget(m_dblvalue_lbl, row++, 1);

  auto* apply_value_btn = new QPushButton(tr("Apply"), this);
  connect(apply_value_btn, &QPushButton::clicked, this, &OcrFrame::applyValue);
  dblvalue_layout->addWidget(apply_value_btn, row++, 0, 1, 2);

  auto* accept_value_btn = new QPushButton(tr("Accept Changes"), this);
  connect(accept_value_btn, &QPushButton::clicked, this, &OcrFrame::acceptChanges);
  dblvalue_layout->addWidget(accept_value_btn, row++, 0, 1, 2);

  auto* cancel_change_btn = new QPushButton(tr("Cancel Changes"), this);
  connect(cancel_change_btn,
          &QPushButton::clicked,
          m_image_display,
          &OcrImage::cancelChanges);
  dblvalue_layout->addWidget(cancel_change_btn, row++, 0, 1, 2);

  return dblvalue_frame;
}

QFrame* OcrFrame::initDenoiseSliderFrame()
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
  connect(m_denoise_filter_slider, &QwtSlider::valueChanged, this, &OcrFrame::setDenoiseFilterLabel);
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
  connect(m_denoise_template_slider, &QwtSlider::valueChanged, this, &OcrFrame::setDenoiseTemplateLabel);
  denoisevalue_layout->addWidget(m_denoise_template_slider, row, 0, Qt::AlignHCenter);

  m_denoise_template_lbl = new QLabel(QString::number(m_denoise_template_slider->value()), this);
  m_denoise_template_lbl->setFont(font);
  m_denoise_template_lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
  denoisevalue_layout->addWidget(m_denoise_template_lbl, row++, 1);

  m_denoise_search_slider = new QwtSlider(Qt::Vertical, this);
  m_denoise_search_slider->setScalePosition(QwtSlider::LeadingScale);
  m_denoise_search_slider->setTrough(true);
  m_denoise_search_slider->setGroove(true);
  connect(m_denoise_search_slider, &QwtSlider::valueChanged, this, &OcrFrame::setDenoiseSearchLabel);
  denoisevalue_layout->addWidget(m_denoise_search_slider, row, 0, Qt::AlignHCenter);

  m_denoise_search_lbl = new QLabel(QString::number(m_denoise_search_slider->value()), this);
  m_denoise_search_lbl->setFont(font);
  m_denoise_search_lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
  denoisevalue_layout->addWidget(m_denoise_search_lbl, row++, 1);

  auto* apply_value_btn = new QPushButton(tr("Apply"), this);
  connect(apply_value_btn, &QPushButton::clicked, this, &OcrFrame::applyValue);
  denoisevalue_layout->addWidget(apply_value_btn, row++, 0, 1, 2);

  auto* accept_value_btn = new QPushButton(tr("Accept Changes"), this);
  connect(accept_value_btn, &QPushButton::clicked, this, &OcrFrame::acceptChanges);
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

QFrame* OcrFrame::initMoveBtns()
{
  QFrame* frame = new QFrame(this);
  frame->setFrameStyle(QFrame::StyledPanel);
  QVBoxLayout* layout = new QVBoxLayout;
  frame->setLayout(layout);

  m_move_sel_to_doc_btn = new QPushButton(tr("Move to Document"), this);
  m_move_sel_to_doc_btn->setEnabled(false);
  m_move_sel_to_doc_btn->setToolTip(tr("Set as an internal Document Image,\n"
                                       "rather than an OCR image."));
  connect(m_move_sel_to_doc_btn, &QPushButton::clicked, this, &OcrFrame::moveToDocument);
  layout->addWidget(m_move_sel_to_doc_btn);

  m_rem_selection_btn = new QPushButton(tr("Remove from Document"), this);
  m_rem_selection_btn->setEnabled(false);
  m_rem_selection_btn->setToolTip(tr("Remove Image/Text from Document."));
  connect(m_rem_selection_btn, &QPushButton::clicked, this, &OcrFrame::removeItemFromDocument);
  layout->addWidget(m_rem_selection_btn);

  return frame;
}

QFrame* OcrFrame::initModifyBtns()
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
  connect(m_greyscale_btn, &QPushButton::clicked, this, &OcrFrame::greyscale);
  layout->addWidget(m_greyscale_btn);

  m_monochrome_btn = new QPushButton(tr("Grey to Monochrome"), this);
  m_monochrome_btn->setToolTip(tr("Convert a greyscale image to monochrome.\n"
                                  "OCR text is generally better on black and white images\n"
                                  "than greyscale images."));
  connect(m_monochrome_btn, &QPushButton::clicked, this, &OcrFrame::monochrome);
  layout->addWidget(m_monochrome_btn);

  m_invert_btn = new QPushButton(tr("Invert Image"), this);
  m_invert_btn->setEnabled(false);
  m_invert_btn->setToolTip(tr("Converts from black text on white to white text on black and back."));
  connect(m_invert_btn, &QPushButton::clicked, this, &OcrFrame::invert);
  layout->addWidget(m_invert_btn);

  m_denoise_btn = new QPushButton(tr("De-noise"), this);
  m_denoise_btn->setToolTip(tr("Noise removal attempts to clean up the image."));
  connect(m_denoise_btn, &QPushButton::clicked, this, &OcrFrame::denoise);
  layout->addWidget(m_denoise_btn);

  m_dewarp_btn = new QPushButton(tr("De-warp"), this);
  m_dewarp_btn->setToolTip(tr("Straigntens lines that are curved."));
  connect(m_dewarp_btn, &QPushButton::clicked, this, &OcrFrame::dewarp);
  layout->addWidget(m_dewarp_btn);

  auto* rotate_cw_btn = new QPushButton(tr("Rotate CW by 90°"), this);
  rotate_cw_btn->setToolTip(tr("Rotate image clockwise by 90°."));
  connect(rotate_cw_btn, &QPushButton::clicked, this, &OcrFrame::rotateCW);
  layout->addWidget(rotate_cw_btn);

  auto* rotate_ccw_btn = new QPushButton(tr("Rotate CCW by 90°"), this);
  rotate_ccw_btn->setToolTip(tr("Rotate image counter-clockwise by 90°"));
  connect(rotate_ccw_btn, &QPushButton::clicked, this, &OcrFrame::rotateCCW);
  layout->addWidget(rotate_ccw_btn);

  auto* rotate_by_180_btn = new QPushButton(tr("Rotate by 180°"), this);
  rotate_by_180_btn->setToolTip(tr("Rotate image by 180°."));
  connect(rotate_by_180_btn, &QPushButton::clicked, this, &OcrFrame::rotate180);
  layout->addWidget(rotate_by_180_btn);

  m_deskew_btn = new QPushButton(tr("De-skew"), this);
  m_deskew_btn->setToolTip(tr("Allows rotation non-horizontal lines.\n"
                              "To use click deskew button, select a start point\n"
                              "using your mouse, then drag a line parallel\n"
                              "to the skewed text, holding down the left mouse button.\n"
                              "The text will automatically be rotated by the\n"
                              "correct amount."));
  connect(m_deskew_btn, &QPushButton::clicked, this, &OcrFrame::deskew);
  layout->addWidget(m_deskew_btn);

  auto* rescale_btn = new QPushButton(tr("Rescale"), this);
  rescale_btn->setToolTip(tr("Increase the DPI of the image."));
  connect(rescale_btn, &QPushButton::clicked, this, &OcrFrame::rescale);
  layout->addWidget(rescale_btn);

  auto* undo_btn = new QPushButton(tr("Undo last operation"), this);
  undo_btn->setToolTip(tr("Undo changes and revert to previous image."));
  connect(undo_btn, &QPushButton::clicked, this, &OcrFrame::undoChanges);
  layout->addWidget(undo_btn);

  return frame;
}

QFrame* OcrFrame::initFinaliseBtns()
{
  QFrame* frame = new QFrame(this);
  frame->setFrameStyle(QFrame::StyledPanel);
  QVBoxLayout* layout = new QVBoxLayout;
  frame->setLayout(layout);

  m_save_txt_btn = new QPushButton(tr("Save Data"), this);
  m_save_txt_btn->setEnabled(false);
  m_save_txt_btn->setToolTip(tr("Save the text and document images."));
  connect(m_save_txt_btn, &QPushButton::clicked, this, &OcrFrame::saveData);
  layout->addWidget(m_save_txt_btn);

  m_save_img_btn = new QPushButton(tr("Save OCR Image"), this);
  m_save_img_btn->setToolTip(tr("Save the OCR image for later."));
  connect(m_save_img_btn, &QPushButton::clicked, this, &OcrFrame::saveCurrentStateImage);
  layout->addWidget(m_save_img_btn);

  m_close_and_save_btn = new QPushButton(tr("Close and Save"), this);
  m_close_and_save_btn->setToolTip(tr("Closes the dialog, saving any changes."));
  connect(m_close_and_save_btn, &QPushButton::clicked, this, &OcrFrame::saveData);
  connect(m_close_and_save_btn, &QPushButton::clicked, this, &OcrFrame::saveCurrentStateImage);
  connect(m_close_and_save_btn, &QPushButton::clicked, this, &OcrFrame::close);
  layout->addWidget(m_close_and_save_btn);

  auto* close_btn = new QPushButton(tr("Close without save"), this);
  close_btn->setToolTip(tr("Closes the modification page but doesn't save data."));
  connect(close_btn, &QPushButton::clicked, this, &OcrFrame::close);
  layout->addWidget(close_btn);

  auto* completed_btn = new QPushButton(tr("OCR is complete"), this);
  completed_btn->setToolTip(tr("You have finished work on the OCR image\n"
                               "and you can delete the image."));
  connect(completed_btn, &QPushButton::clicked, this, &OcrFrame::completeOperation);
  layout->addWidget(completed_btn);

  return frame;
}

void OcrFrame::initGui()
{
  auto* layout = new QGridLayout;
  setLayout(layout);

  m_scan_list = new ScanList(this);
  connect(m_scan_list, &ScanList::clicked, this, &OcrFrame::scanListWasClicked);
  connect(m_scan_list, &ScanList::doubleClicked, this, &OcrFrame::scanListWasDoubleClicked);
  connect(m_scan_list, &ScanList::dataHasChanged, this, &OcrFrame::scanListDataChanged);
  connect(m_scan_list, &ScanList::imageHasChanged, this, &OcrFrame::scanListDataChanged);
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
  connect(m_image_display, &OcrImage::selected, this, &OcrFrame::setSelected);
  connect(m_image_display, &OcrImage::unselected, this, &OcrFrame::setUnselected);
  connect(m_image_display, &OcrImage::imageSizeChanged, this, &OcrFrame::imageSizeHasChanged);
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
    connect(m_ocr_btn, &QPushButton::clicked, this, &OcrFrame::requestOcr);
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
    connect(help_btn, &QPushButton::clicked, this, &OcrFrame::help);
    btn_layout->addWidget(help_btn);

    enableCleanImageBtns(false);
  }

  m_ctl_stack->setCurrentIndex(m_btn_stack);

  layout->setColumnStretch(0, 3);
  layout->setColumnStretch(1, 1);
  layout->setColumnStretch(2, 3);
}

void OcrFrame::requestOcr()
{
  if (m_image_display->hasSelection()) {
    requestOcrOnSelection();

  } else {
    QImage image = m_image_display->currentImage();
    emit sendOcrRequest(m_page_no, image);
  }
}

void OcrFrame::requestOcrOnSelection()
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

void OcrFrame::help()
{
  // TODO help
}

void OcrFrame::setSelected()
{
  m_crop_btn->setEnabled(true);
  m_clr_to_back_btn->setEnabled(true);
  //  m_ocr_sel_btn->setEnabled(true);
  m_move_sel_to_doc_btn->setEnabled(true);
}

void OcrFrame::setUnselected()
{
  m_crop_btn->setEnabled(false);
  m_clr_to_back_btn->setEnabled(false);
  //  m_ocr_sel_btn->setEnabled(false);
  m_move_sel_to_doc_btn->setEnabled(false);
}

void OcrFrame::crop()
{
  m_image_display->cropToSelection();
}

void OcrFrame::greyscale()
{
  m_change_type = Greyscale;
  setCurrentChangeState();
  m_image_display->greyscale();
}

void OcrFrame::monochrome()
{
  m_change_type = Monochrome;
  setCurrentChangeState();
}

void OcrFrame::invert()
{
  m_image_display->invert();
  enableCleanImageBtns(true);
  m_doc_data->setInverted(m_image_display->isInverted());
}

void OcrFrame::denoise()
{
  m_change_type = Denoise;
  setCurrentChangeState();
  m_image_display->denoise();
}

void OcrFrame::dewarp()
{
  // TODO dewarp
}

void OcrFrame::deskew()
{
  m_image_display->deskew();
}

void OcrFrame::rotate180()
{
  m_image_display->rotate180();
}

void OcrFrame::rotateCW()
{
  m_image_display->rotateCW();
}

void OcrFrame::rotateCCW()
{
  m_image_display->rotateCCW();
}

void OcrFrame::rescale()
{
  m_change_type = Rescale;
  m_image_display->rescale();
  setCurrentChangeState();
}

void OcrFrame::moveToDocument()
{
  QImage image = m_image_display->copySelection();
  int image_index = m_scan_list->appendImage(image);

  emit saveSelectedDocumentImage(m_page_no, image_index, image, m_doc_data);
}

void OcrFrame::scanListWasClicked(const QModelIndex& /*index*/)
{
  if (!m_scan_list->selectionModel()->selection().empty()) {
    m_rem_selection_btn->setEnabled(true);

  } else {
    m_rem_selection_btn->setEnabled(false);
  }
}

void OcrFrame::scanListWasDoubleClicked(const QModelIndex& index)
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

void OcrFrame::completeOperation()
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

void OcrFrame::removeItemFromDocument()
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

void OcrFrame::saveData()
{
  emit saveModifiedData(m_doc_data);
}

void OcrFrame::saveCurrentStateImage()
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

void OcrFrame::acceptChanges()
{
  m_image_display->acceptChanges();
  m_ctl_stack->setCurrentIndex(m_btn_stack);
  emit makeCompleted(m_doc_data);
}

void OcrFrame::undoChanges()
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

void OcrFrame::close()
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

void OcrFrame::scanListDataChanged()
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

void OcrFrame::ocrImageChanged(bool changed)
{
  m_save_img_btn->setEnabled(changed);

  if (!m_scan_list->hasDataChanges()) {
    m_close_and_save_btn->setEnabled(changed);
  }
}

void OcrFrame::applyValue()
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
void OcrFrame::setCurrentChangeState()
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

void OcrFrame::setIntValueLabel(qreal value)
{
  m_intvalue_lbl->setText(QString::number(int(value)));
}

void OcrFrame::setDoubleValueLabel(qreal value)
{
  m_dblvalue_lbl->setText(tr("%1").arg(value, 0, 'g', 2));
}

void OcrFrame::setDenoiseTemplateLabel(qreal value)
{
  m_denoise_template_lbl->setText(QString::number(Util::makeOdd(value)));
}

void OcrFrame::setDenoiseSearchLabel(qreal value)
{
  m_denoise_search_lbl->setText(QString::number(Util::makeOdd(value)));
}

void OcrFrame::setDenoiseFilterLabel(qreal value)
{
  m_denoise_filter_lbl->setText(QString::number(int(value)));
}

void OcrFrame::valueAccepted()
{
  m_ctl_stack->setCurrentIndex(m_btn_stack);
}

void OcrFrame::disableBinarise()
{
  m_greyscale_btn->setEnabled(false);
  enableCleanImageBtns(true);
}

void OcrFrame::enableBinarise()
{
  m_greyscale_btn->setEnabled(true);
}

void OcrFrame::enableCleanImageBtns(bool enable)
{
  m_invert_btn->setEnabled(enable);
  m_denoise_btn->setEnabled(enable);
  m_dewarp_btn->setEnabled(enable);
  m_deskew_btn->setEnabled(enable);
}

void OcrFrame::imageSizeHasChanged(int width, int height, int xres, int yres)
{
  setSizeLabel(width, height);
  setResLabel(xres, yres);
}

void OcrFrame::resizeEvent(QResizeEvent* event)
{
  QSize parent_size = qobject_cast<QWidget*>(parent())->frameSize();
  auto size = parent_size;
  size -= QSize(100, 100);

  auto* e = new QResizeEvent(size, event->oldSize());
  QFrame::resizeEvent(e);
}

void OcrFrame::setMessageLabel(const QString& text)
{
  m_message_lbl->setText(text);
}

void OcrFrame::setSizeLabel(int width, int height)
{
  m_image_size_lbl->setText(m_image_size.arg(width).arg(height));
}

void OcrFrame::setNameLabel(int page, QString name)
{
  m_image_name_lbl->setText(m_image_name.arg(page).arg(name));
}

void OcrFrame::setResLabel(int xres, int yres)
{
  m_res_lbl->setText(m_resolution.arg(xres).arg(yres));
}
