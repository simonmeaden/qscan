#include "ocrframe.h"

#include "ocrimage.h"
#include "scanlist.h"

OcrFrame::OcrFrame(QWidget *parent)
  : QFrame(parent)
  , m_data_edit(nullptr)
  , m_image_display(nullptr)
  , m_image_changed(false)
  , m_change_type(None)
{
  initGui();
}

QImage OcrFrame::image()
{
  return m_image_display->image();
}

void OcrFrame::setResolution(const QImage &image)
{
  int x_dpi = Util::dpmToDpi(image.dotsPerMeterX());
  int y_dpi = Util::dpmToDpi(image.dotsPerMeterY());
  setResLabel(x_dpi, y_dpi);
}

void OcrFrame::setData(int page_no, const QImage &image, const DocumentData &doc_data)
{
  m_page_no = page_no;
  m_doc_data = doc_data;
  m_image_display->setImage(image);
  m_image_display->setInverted(doc_data->inverted());
  QMap<int, QString> in_data = doc_data->textList();
  QMap<int, QVariant> data;
  m_item_count = 0;

  for (auto it = in_data.keyBegin(), end = in_data.keyEnd(); it != end; ++it) {
    int key = *it;
    data.insert(key, in_data.value(key));
    m_item_count = (m_item_count >= *it ? m_item_count : *it);
  }

  in_data = doc_data->imageList();

  for (auto it = in_data.keyBegin(), end = in_data.keyEnd(); it != end; ++it) {
    int key = *it;
    QString path = in_data.value(key);
    QImage image;
    image.load(path, "PNG");
    data.insert(key, image);
    m_item_count = (m_item_count >= *it ? m_item_count : *it);
  }

  if (!data.isEmpty()) {
    m_data_edit->setData(data);
  }

  QFileInfo info(m_doc_data->filename());
  setNameLabel(m_page_no, info.fileName());
  setSizeLabel(image.width(), image.height());

  setResolution(image);

  if (image.format() == QImage::Format_Mono || image.format() == QImage::Format_Grayscale8) {
    m_binarise_btn->setEnabled(false);
  }

  enableCleanImageBtns(doc_data->inverted());
}

void OcrFrame::setOcrImage(int page_no, const QImage &image)
{
  m_page_no = page_no, m_image_display->setImage(image);
}

void OcrFrame::setOcrText(int page_no, const QString &text)
{
  if (page_no == m_page_no) {
    m_data_edit->setText(text);
    m_doc_data->appendText(text);
  }
}

void OcrFrame::appendOcrText(int page_no, const QString &text)
{
  if (page_no == m_page_no) {
    m_data_edit->appendText(text);
    m_doc_data->appendText(text);
    m_image_display->clearSelection();
  }
}

QStringList OcrFrame::texts()
{
  return m_data_edit->texts();
}

bool OcrFrame::imageChanged() const
{
  return m_image_changed;
}

QImage OcrFrame::modifiedImage()
{
  return m_image_display->modifiedImage();
}

int OcrFrame::pageNumber()
{
  return m_page_no;
}

DocumentData OcrFrame::documentData() const
{
  return m_doc_data;
}

QFrame *OcrFrame::initIntSliderFrame()
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

  auto *accept_value_btn = new QPushButton(tr("Accept Changes"), this);
  connect(accept_value_btn, &QPushButton::clicked, this, &OcrFrame::acceptChanges);
  intvalue_layout->addWidget(accept_value_btn, row++, 0, 1, 2);

  auto *undo_last_btn = new QPushButton(tr("Undo Last Change"), this);
  connect(undo_last_btn, &QPushButton::clicked, m_image_display, &OcrImage::undoLastChange);
  intvalue_layout->addWidget(undo_last_btn, row++, 0, 1, 2);

  auto *cancel_change_btn = new QPushButton(tr("Cancel Current Changes"), this);
  connect(cancel_change_btn,
          &QPushButton::clicked,
          m_image_display,
          &OcrImage::cancelCurrentChanges);
  connect(cancel_change_btn, &QPushButton::clicked, this, &OcrFrame::reject);
  intvalue_layout->addWidget(cancel_change_btn, row++, 0, 1, 2);

  return intvalue_frame;
}

QFrame *OcrFrame::initDoubleSliderFrame()
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

  auto *accept_value_btn = new QPushButton(tr("Accept Changes"), this);
  connect(accept_value_btn, &QPushButton::clicked, this, &OcrFrame::acceptChanges);
  dblvalue_layout->addWidget(accept_value_btn, row++, 0, 1, 2);

  auto *undo_last_btn = new QPushButton(tr("Undo Last Change"), this);
  connect(undo_last_btn, &QPushButton::clicked, m_image_display, &OcrImage::undoLastChange);
  dblvalue_layout->addWidget(undo_last_btn, row++, 0, 1, 2);

  auto *cancel_change_btn = new QPushButton(tr("Cancel Current Changes"), this);
  connect(cancel_change_btn,
          &QPushButton::clicked,
          m_image_display,
          &OcrImage::cancelCurrentChanges);
  dblvalue_layout->addWidget(cancel_change_btn, row++, 0, 1, 2);

  return dblvalue_frame;
}

void OcrFrame::initGui()
{
  auto *layout = new QGridLayout;
  setLayout(layout);

  m_data_edit = new ScanList(this);
  layout->addWidget(m_data_edit, 0, 0);

  auto *stacked_frame = new QFrame(this);
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

    m_ocr_sel_btn = new QPushButton(tr("Run OCR on Selection"), this);
    m_ocr_sel_btn->setEnabled(false);
    m_ocr_sel_btn->setToolTip(tr("Re run the OCR on part of the tweaked image."));
    connect(m_ocr_sel_btn, &QPushButton::clicked, this, &OcrFrame::requestOcrOnSelection);
    btn_layout->addWidget(m_ocr_sel_btn);

    btn_layout->addStretch(1);

    m_clr_to_back_btn = new QPushButton(tr("Clear to background"), this);
    m_clr_to_back_btn->setToolTip(tr("Remove section from the image image."));
    m_clr_to_back_btn->setEnabled(false);
    connect(m_clr_to_back_btn, &QPushButton::clicked, m_image_display, &OcrImage::clearToBackground);
    btn_layout->addWidget(m_clr_to_back_btn);

    m_crop_btn = new QPushButton(tr("Crop"), this);
    m_crop_btn->setToolTip(tr("Crop the image to remove excess image."));
    m_crop_btn->setEnabled(false);
    connect(m_crop_btn, &QPushButton::clicked, m_image_display, &OcrImage::cropToSelection);
    btn_layout->addWidget(m_crop_btn);

    m_binarise_btn = new QPushButton(tr("Binarise"), this);
    m_binarise_btn->setToolTip(tr("Binarisation turns the image to blackl and white.\n"
                                  "OCR text is generally better on black and white images."));
    connect(m_binarise_btn, &QPushButton::clicked, this, &OcrFrame::binarise);
    btn_layout->addWidget(m_binarise_btn);

    auto* invert_btn = new QPushButton(tr("Invert Image"), this);
    invert_btn->setToolTip(tr("Converts from black text on white to white on black and back."));
    connect(invert_btn, &QPushButton::clicked, this, &OcrFrame::invert);
    btn_layout->addWidget(invert_btn);

    denoise_btn = new QPushButton(tr("De-noise"), this);
    denoise_btn->setToolTip(tr("Noise removal attempts to clean up the image."));
    connect(denoise_btn, &QPushButton::clicked, this, &OcrFrame::denoise);
    btn_layout->addWidget(denoise_btn);

    dewarp_btn = new QPushButton(tr("De-warp"), this);
    dewarp_btn->setToolTip(tr("Straigntens lines that are curved."));
    connect(dewarp_btn, &QPushButton::clicked, this, &OcrFrame::dewarp);
    btn_layout->addWidget(dewarp_btn);

    auto* rotate_cw_btn = new QPushButton(tr("Rotate CW by 90°"), this);
    rotate_cw_btn->setToolTip(tr("Rotate image clockwise by 90°."));
    connect(rotate_cw_btn, &QPushButton::clicked, this, &OcrFrame::rotateCW);
    btn_layout->addWidget(rotate_cw_btn);

    auto* rotate_ccw_btn = new QPushButton(tr("Rotate CCW by 90°"), this);
    rotate_ccw_btn->setToolTip(tr("Rotate image counter-clockwise by 90°"));
    connect(rotate_ccw_btn, &QPushButton::clicked, this, &OcrFrame::rotateCCW);
    btn_layout->addWidget(rotate_ccw_btn);

    auto* rotate_by_180_btn = new QPushButton(tr("Rotate by 180°"), this);
    rotate_by_180_btn->setToolTip(tr("Rotate image by 180°."));
    connect(rotate_by_180_btn, &QPushButton::clicked, this, &OcrFrame::rotate180);
    btn_layout->addWidget(rotate_by_180_btn);

    auto* deskew_btn = new QPushButton(tr("De-skew"), this);
    deskew_btn->setToolTip(tr("Allows rotation non-horizontal lines.\n"
                              "To use click deskew button, select a start point\n"
                              "using your mouse, then drag a line parallel\n"
                              "to the skewed text, holding down the left mouse button.\n"
                              "The text will automatically be rotated by the\n"
                              "correct amount."));
    connect(deskew_btn, &QPushButton::clicked, this, &OcrFrame::deskew);
    btn_layout->addWidget(deskew_btn);

    auto* rescale_btn = new QPushButton(tr("Rescale"), this);
    rescale_btn->setToolTip(tr("Increase the DPI of the image."));
    connect(rescale_btn, &QPushButton::clicked, this, &OcrFrame::rescale);
    btn_layout->addWidget(rescale_btn);

    auto* undo_btn = new QPushButton(tr("Undo last operation"), this);
    undo_btn->setToolTip(tr("Undo changes and revert to previous image."));
    connect(undo_btn, &QPushButton::clicked, this, &OcrFrame::undoChanges);
    btn_layout->addWidget(undo_btn);

    btn_layout->addStretch(1);

    move_sel_to_doc_btn = new QPushButton(tr("Set to Document"), this);
    move_sel_to_doc_btn->setEnabled(false);
    move_sel_to_doc_btn->setToolTip(tr("Set as an internal Document Image."));
    connect(move_sel_to_doc_btn, &QPushButton::clicked, this, &OcrFrame::moveToDocument);
    btn_layout->addWidget(move_sel_to_doc_btn);

    m_save_txt_btn = new QPushButton(tr("Save Text"), this);
    m_save_txt_btn->setEnabled(false);
    m_save_txt_btn->setToolTip(tr("Save the text."));
    connect(m_save_txt_btn, &QPushButton::clicked, this, &OcrFrame::saveData);
    btn_layout->addWidget(m_save_txt_btn);

    auto *save_img_btn = new QPushButton(tr("Save Image"), this);
    save_img_btn->setToolTip(tr("Save the image."));
    connect(save_img_btn, &QPushButton::clicked, this, &OcrFrame::saveCurrentStateImage);
    btn_layout->addWidget(save_img_btn);

    auto* discard_btn = new QPushButton(tr("Discard changes"), this);
    discard_btn->setToolTip(tr("Closes the dialog, discarding any changes."));
    connect(discard_btn, &QPushButton::clicked, this, &OcrFrame::discard);
    btn_layout->addWidget(discard_btn);

    auto* close_btn = new QPushButton(tr("Close and Save"), this);
    close_btn->setToolTip(tr("Closes the dialog, discarding any changes."));
    connect(close_btn, &QPushButton::clicked, this, &OcrFrame::close);
    btn_layout->addWidget(close_btn);

    btn_layout->addStretch(1);

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
  QImage image = m_image_display->modifiedImage();
  emit sendOcrRequest(m_page_no, image);
}

void OcrFrame::requestOcrOnSelection()
{
  QRect copy_rect = m_image_display->selection();
  QImage image = m_image_display->modifiedImage();
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
  m_save_txt_btn->setEnabled(true);
  m_ocr_sel_btn->setEnabled(true);
  move_sel_to_doc_btn->setEnabled(true);
}

void OcrFrame::setUnselected()
{
  m_crop_btn->setEnabled(false);
  m_clr_to_back_btn->setEnabled(false);
  m_save_txt_btn->setEnabled(false);
  m_ocr_sel_btn->setEnabled(false);
  move_sel_to_doc_btn->setEnabled(false);
}

void OcrFrame::crop()
{
  m_image_display->cropToSelection();
  m_image_changed = true;
}

void OcrFrame::binarise()
{
  m_change_type = Binarise;
  setCurrentChangeState();
  m_image_display->binarise();
}

void OcrFrame::invert()
{
  m_image_display->invert();
  m_image_changed = true;
  enableCleanImageBtns(true);
  m_doc_data->setInverted(m_image_display->isInverted());
}

void OcrFrame::denoise()
{
  m_image_display->denoise();
  m_image_changed = true;
}

void OcrFrame::dewarp()
{
  // TODO dewarp
}

void OcrFrame::deskew()
{
  m_image_display->deskew();
  m_image_changed = true;
}

void OcrFrame::rotate180()
{
  m_image_display->rotate180();
  m_image_changed = true;
}

void OcrFrame::rotateCW()
{
  m_image_display->rotateCW();
  m_image_changed = true;
}

void OcrFrame::rotateCCW()
{
  m_image_display->rotateCCW();
  m_image_changed = true;
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
  int image_index = m_data_edit->appendImage(image);

  emit saveSelectedDocumentImage(m_page_no, image_index, image, m_doc_data);
}

void OcrFrame::saveData()
{
  emit saveModifiedData(m_doc_data);
}

void OcrFrame::saveCurrentStateImage()
{
  emit saveModifiedImage(m_page_no, m_image_display->modifiedImage());
  m_image_changed = false;
}

void OcrFrame::discard()
{
  int result = QMessageBox::warning(this,
                                    tr("Discaring Changes"),
                                    tr("You are about to discard any changes you have made\n"
                                       "and quit the dialog.\n"
                                       "This cannot be undone\n"
                                       "Are you sure?"),
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No);

  if (result == QMessageBox::Yes) {
    m_image_display->dumpImageChanges();
    emit reject();
  }
}

void OcrFrame::acceptChanges()
{
  m_image_display->acceptChanges();
  emit accept();
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
    emit reject();
  }
}

void OcrFrame::close()
{
  int result = QMessageBox::warning(this,
                                    tr("Save and Update"),
                                    tr("You are about to save any changes you have made\n"
                                       "including modified images and text."
                                       "This cannot be undone\n"
                                       "Are you sure?"),
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No);

  if (result == QMessageBox::Yes) {
    saveCurrentStateImage();
    saveData();
    m_doc_data->setInverted(m_image_display->isInverted());
    emit accept();
  }
}

void OcrFrame::applyValue()
{
  switch (m_change_type) {
  case Binarise: {
    int value = qRound(m_intvalue_slider->value());
    m_image_display->applyThreshold(value);
    break;
  }

  case Rescale: {
    qreal value = Util::round(m_dblvalue_slider->value(), 2);
    m_image_display->applyRescale(value);
    break;
  }
  }
}
void OcrFrame::setCurrentChangeState()
{
  QList<double> major_ticks, medium_ticks, minor_ticks;

  switch (m_change_type) {
  case Binarise:
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
    setMessageLabel(tr("Binarising image"));
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
  }
}

void OcrFrame::setIntValueLabel(qreal value)
{
  m_intvalue_lbl->setText(QString::number(int(value)));
}

void OcrFrame::setDoubleValueLabel(qreal value)
{
  m_dblvalue_lbl->setText(QString("%1").arg(value, 0, 'g', 2));
}

void OcrFrame::valueAccepted()
{
  m_ctl_stack->setCurrentIndex(m_btn_stack);
}

void OcrFrame::disableBinarise()
{
  m_binarise_btn->setEnabled(false);
  enableCleanImageBtns(true);
}

void OcrFrame::enableBinarise()
{
  m_binarise_btn->setEnabled(true);
}

void OcrFrame::enableCleanImageBtns(bool enable)
{
  denoise_btn->setEnabled(enable);
  dewarp_btn->setEnabled(enable);
  //  deskew_btn->setEnabled(enable);
}

void OcrFrame::imageSizeHasChanged(int width, int height, int xres, int yres)
{
  setSizeLabel(width, height);
  setResLabel(xres, yres);
}

void OcrFrame::resizeEvent(QResizeEvent *event)
{
  QSize parent_size = qobject_cast<QWidget*>(parent())->frameSize();
  auto size = parent_size;
  size -= QSize(100, 100);

  auto* e = new QResizeEvent(size, event->oldSize());
  QFrame::resizeEvent(e);
}

void OcrFrame::setMessageLabel(const QString &text)
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

///*
//   =======================================*/
//OcrDialog::OcrDialog(QWidget *parent)
//  : QDialog(parent)
//{
//  auto parent_size = parent->frameSize();
//  auto size = parent_size;
//  size -= QSize(50, 50);
//  auto pos = parent->pos();
//  pos += QPoint(25, 25);
//  setGeometry(QRect(pos, size));
//}

//void OcrDialog::open()
//{
//  QDialog::open();
//  m_image_display->fitByType();
//}
