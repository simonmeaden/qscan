#include "ocrdialog.h"

#include "ocrimage.h"
#include "scanlist.h"

OcrDialog::OcrDialog(QWidget* parent)
  : QDialog(parent)
  , m_text_edit(nullptr)
  , m_image_display(nullptr)
  , m_image_changed(false)
{
  initGui();

  auto parent_size = parent->frameSize();
  auto size = parent_size;
  size -= QSize(50, 50);
  auto pos = parent->pos();
  pos += QPoint(25, 25);
  setGeometry(QRect(pos, size));
}

QImage OcrDialog::image()
{
  return m_image_display->image();
}

void OcrDialog::setData(int index, const QImage& image, const DocumentData& doc_data)
{
  m_page_no = index;
  m_doc_data = doc_data;
  m_image_display->setImage(image);
  m_image_display->setInverted(doc_data->inverted());
  m_text_edit->setText(doc_data->textList());

  if (image.format() == QImage::Format_Mono || image.format() == QImage::Format_Grayscale8) {
    m_binarise_btn->setEnabled(false);
  }

  enableCleanImageBtns(doc_data->inverted());
}

void OcrDialog::setOcrImage(int index, const QImage& image)
{
  m_page_no = index, m_image_display->setImage(image);
}

void OcrDialog::setOcrText(int page_no, const QString& text)
{
  if (page_no == m_page_no) {
    m_text_edit->setText(text);
  }
}

QStringList OcrDialog::text()
{
  return m_text_edit->text();
}

bool OcrDialog::imageChanged() const
{
  return m_image_changed;
}

QSize OcrDialog::sizeHint() const
{
  QWidget* widget = qobject_cast<QWidget*>(parent());
  QSize size = widget->sizeHint();
  //  QPoint pos = widget->pos();
  size -= QSize(100, 100);
  //  pos -= QPoint(50,50);
  return { size };
}

QImage OcrDialog::modifiedImage()
{
  return m_image_display->modifiedImage();
}

int OcrDialog::pageNumber()
{
  return m_page_no;
}

DocumentData OcrDialog::page() const
{
  return m_doc_data;
}

void OcrDialog::open()
{
  QDialog::open();
  m_image_display->fitByType();
}

QFrame* OcrDialog::initIntSliderFrame()
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
  connect(m_intvalue_slider, &QwtSlider::valueChanged, this, &OcrDialog::setIntValueLabel);
  intvalue_layout->addWidget(m_intvalue_slider, row, 0, Qt::AlignHCenter);

  m_intvalue_lbl = new QLabel(QString::number(m_intvalue_slider->value()), this);
  QFont font;
  font.setWeight(QFont::Bold);
  font.setPixelSize(40);
  m_intvalue_lbl->setFont(font);
  m_intvalue_lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
  intvalue_layout->addWidget(m_intvalue_lbl, row++, 1);

  auto* apply_value_btn = new QPushButton(tr("Apply"), this);
  connect(apply_value_btn, &QPushButton::clicked, this, &OcrDialog::applyValue);
  intvalue_layout->addWidget(apply_value_btn, row++, 0, 1, 2);

  auto* accept_value_btn = new QPushButton(tr("Accept"), this);
  connect(accept_value_btn, &QPushButton::clicked, m_image_display, &OcrImage::acceptChanges);
  intvalue_layout->addWidget(accept_value_btn, row++, 0, 1, 2);

  auto* undo_last_btn = new QPushButton(tr("Undo Last"), this);
  connect(undo_last_btn, &QPushButton::clicked, m_image_display, &OcrImage::undoLastChange);
  intvalue_layout->addWidget(undo_last_btn, row++, 0, 1, 2);

  auto* cancel_change_btn = new QPushButton(tr("Cancel"), this);
  connect(cancel_change_btn, &QPushButton::clicked, m_image_display, &OcrImage::cancelChanges);
  intvalue_layout->addWidget(cancel_change_btn, row++, 0, 1, 2);

  return intvalue_frame;
}

QFrame* OcrDialog::initDoubleSliderFrame()
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
  connect(m_dblvalue_slider, &QwtSlider::valueChanged, this, &OcrDialog::setDoubleValueLabel);
  dblvalue_layout->addWidget(m_dblvalue_slider, row, 0, Qt::AlignHCenter);

  m_dblvalue_lbl = new QLabel(QString::number(m_intvalue_slider->value()), this);
  QFont font;
  font.setWeight(QFont::Bold);
  font.setPixelSize(40);
  m_dblvalue_lbl->setFont(font);
  m_dblvalue_lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
  dblvalue_layout->addWidget(m_dblvalue_lbl, row++, 1);

  auto* apply_value_btn = new QPushButton(tr("Apply"), this);
  connect(apply_value_btn, &QPushButton::clicked, this, &OcrDialog::applyValue);
  dblvalue_layout->addWidget(apply_value_btn, row++, 0, 1, 2);

  auto* accept_value_btn = new QPushButton(tr("Accept"), this);
  connect(accept_value_btn, &QPushButton::clicked, m_image_display, &OcrImage::acceptChanges);
  dblvalue_layout->addWidget(accept_value_btn, row++, 0, 1, 2);

  auto* undo_last_btn = new QPushButton(tr("Undo Last"), this);
  connect(undo_last_btn, &QPushButton::clicked, m_image_display, &OcrImage::undoLastChange);
  dblvalue_layout->addWidget(undo_last_btn, row++, 0, 1, 2);

  auto* cancel_change_btn = new QPushButton(tr("Cancel"), this);
  connect(cancel_change_btn, &QPushButton::clicked, m_image_display, &OcrImage::cancelChanges);
  dblvalue_layout->addWidget(cancel_change_btn, row++, 0, 1, 2);

  return dblvalue_frame;
}

void OcrDialog::initGui()
{
  auto* layout = new QGridLayout;
  setLayout(layout);

  m_text_edit = new ScanList(this);
  layout->addWidget(m_text_edit, 0, 0);

  auto* stacked_frame = new QFrame(this);
  m_ctl_stack = new QStackedLayout;
  stacked_frame->setLayout(m_ctl_stack);
  layout->addWidget(stacked_frame, 0, 1);

  // This needs to be done before threshold/btns
  // so m_image_display exists for them to connect to.
  m_image_display = new OcrImage(this);
  connect(m_image_display, &OcrImage::selected, this, &OcrDialog::setSelected);
  connect(m_image_display, &OcrImage::unselected, this, &OcrDialog::setUnselected);
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
    connect(m_ocr_btn, &QPushButton::clicked, this, &OcrDialog::requestOcr);
    btn_layout->addWidget(m_ocr_btn);

    m_ocr_sel_btn = new QPushButton(tr("Run OCR on Selection"), this);
    m_ocr_sel_btn->setToolTip(tr("Re run the OCR on part of the tweaked image."));
    connect(m_ocr_sel_btn, &QPushButton::clicked, this, &OcrDialog::requestOcrOnSelection);
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
    connect(m_binarise_btn, &QPushButton::clicked, this, &OcrDialog::binarise);
    btn_layout->addWidget(m_binarise_btn);

    auto* invert_btn = new QPushButton(tr("Invert Image"), this);
    invert_btn->setToolTip(tr("Converts from black text on white to white on black and back."));
    connect(invert_btn, &QPushButton::clicked, this, &OcrDialog::invert);
    btn_layout->addWidget(invert_btn);

    denoise_btn = new QPushButton(tr("De-noise"), this);
    denoise_btn->setToolTip(tr("Noise removal attempts to clean up the image."));
    connect(denoise_btn, &QPushButton::clicked, this, &OcrDialog::denoise);
    btn_layout->addWidget(denoise_btn);

    dewarp_btn = new QPushButton(tr("De-warp"), this);
    dewarp_btn->setToolTip(tr("Straigntens lines that are curved."));
    connect(dewarp_btn, &QPushButton::clicked, this, &OcrDialog::dewarp);
    btn_layout->addWidget(dewarp_btn);

    auto* rotate_cw_btn = new QPushButton(tr("Rotate CW by 90°"), this);
    rotate_cw_btn->setToolTip(tr("Rotate image clockwise by 90°."));
    connect(rotate_cw_btn, &QPushButton::clicked, this, &OcrDialog::rotateCW);
    btn_layout->addWidget(rotate_cw_btn);

    auto* rotate_ccw_btn = new QPushButton(tr("Rotate CCW by 90°"), this);
    rotate_ccw_btn->setToolTip(tr("Rotate image counter-clockwise by 90°"));
    connect(rotate_ccw_btn, &QPushButton::clicked, this, &OcrDialog::rotateCCW);
    btn_layout->addWidget(rotate_ccw_btn);

    auto* rotate_by_180_btn = new QPushButton(tr("Rotate by 180°"), this);
    rotate_by_180_btn->setToolTip(tr("Rotate image by 180°."));
    connect(rotate_by_180_btn, &QPushButton::clicked, this, &OcrDialog::rotate180);
    btn_layout->addWidget(rotate_by_180_btn);

    auto* deskew_btn = new QPushButton(tr("De-skew"), this);
    deskew_btn->setToolTip(tr("Allows rotation non-horizontal lines.\n"
                              "To use click deskew button, select a start point\n"
                              "using your mouse, then drag a line parallel\n"
                              "to the skewed text, holding down the left mouse button.\n"
                              "The text will automatically be rotated by the\n"
                              "correct amount."));
    connect(deskew_btn, &QPushButton::clicked, this, &OcrDialog::deskew);
    btn_layout->addWidget(deskew_btn);

    auto* rescale_btn = new QPushButton(tr("Rescale"), this);
    rescale_btn->setToolTip(tr("Increase the DPI of the image."));
    connect(rescale_btn, &QPushButton::clicked, this, &OcrDialog::rescale);
    btn_layout->addWidget(rescale_btn);

    auto* undo_btn = new QPushButton(tr("Undo last operation"), this);
    undo_btn->setToolTip(tr("Undo changes and revert to previous image."));
    connect(undo_btn, &QPushButton::clicked, this, &OcrDialog::undoChanges);
    btn_layout->addWidget(undo_btn);

    btn_layout->addStretch(1);

    auto* save_txt_btn = new QPushButton(tr("Save Text"), this);
    save_txt_btn->setToolTip(tr("Save the text."));
    connect(save_txt_btn, &QPushButton::clicked, this, &OcrDialog::saveText);
    btn_layout->addWidget(save_txt_btn);

    auto* save_img_btn = new QPushButton(tr("Save Image"), this);
    save_img_btn->setToolTip(tr("Save the image."));
    connect(save_img_btn, &QPushButton::clicked, this, &OcrDialog::saveImage);
    btn_layout->addWidget(save_img_btn);

    auto* discard_btn = new QPushButton(tr("Discard changes"), this);
    discard_btn->setToolTip(tr("Closes the dialog, discarding any changes."));
    connect(discard_btn, &QPushButton::clicked, this, &OcrDialog::discard);
    btn_layout->addWidget(discard_btn);

    auto* close_btn = new QPushButton(tr("Close and Save"), this);
    close_btn->setToolTip(tr("Closes the dialog, discarding any changes."));
    connect(close_btn, &QPushButton::clicked, this, &OcrDialog::close);
    btn_layout->addWidget(close_btn);

    btn_layout->addStretch(1);

    auto* help_btn = new QPushButton(tr("Help"), this);
    help_btn->setToolTip(tr("Help."));
    connect(help_btn, &QPushButton::clicked, this, &OcrDialog::help);
    btn_layout->addWidget(help_btn);

    enableCleanImageBtns(false);
  }

  m_ctl_stack->setCurrentIndex(m_btn_stack);

  layout->setColumnStretch(0, 3);
  layout->setColumnStretch(1, 1);
  layout->setColumnStretch(2, 3);
}

void OcrDialog::requestOcr()
{
  emit sendOcrRequest(m_page_no, m_image_display->modifiedImage());
}

void OcrDialog::requestOcrOnSelection()
{
  QImage copy = m_image_display->selectedSubImage();

  if (!copy.isNull()) {
    emit sendOcrRequest(m_page_no, copy);
  }
}

void OcrDialog::help()
{
  // TODO help
}

void OcrDialog::setSelected()
{
  m_crop_btn->setEnabled(true);
  m_clr_to_back_btn->setEnabled(true);
}

void OcrDialog::setUnselected()
{
  m_crop_btn->setEnabled(false);
  m_clr_to_back_btn->setEnabled(false);
}

void OcrDialog::crop()
{
  m_image_display->cropToSelection();
  m_image_changed = true;
}

void OcrDialog::binarise()
{
  m_change_type = Binarise;
  setCurrentValueType();
  m_image_display->binarise();
  m_image_changed = true;
}

void OcrDialog::invert()
{
  m_image_display->invert();
  m_image_changed = true;
  enableCleanImageBtns(true);
  m_doc_data->setInverted(m_image_display->isInverted());
}

void OcrDialog::denoise()
{
  m_image_display->denoise();
  m_image_changed = true;
}

void OcrDialog::dewarp()
{
  // TODO dewarp
}

void OcrDialog::deskew()
{
  m_image_display->deskew();
  m_image_changed = true;
}

void OcrDialog::rotate180()
{
  m_image_display->rotate180();
  m_image_changed = true;
}

void OcrDialog::rotateCW()
{
  m_image_display->rotateCW();
  m_image_changed = true;
}

void OcrDialog::rotateCCW()
{
  m_image_display->rotateCCW();
  m_image_changed = true;
}

void OcrDialog::rescale()
{
  m_change_type = Rescale;
  setCurrentValueType();
  //  m_ctl_stack->setCurrentIndex(m_dblvalue_stack);
  m_image_changed = true;
}

void OcrDialog::saveText()
{
  emit saveModifiedText(m_page_no, m_text_edit->text());
}

void OcrDialog::saveImage()
{
  emit saveModifiedImage(m_page_no, m_image_display->modifiedImage());
  m_image_changed = false;
}

void OcrDialog::discard()
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
    reject();
  }
}

void OcrDialog::undoChanges()
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
  }
}

void OcrDialog::close()
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
    saveImage();
    saveText();
    m_doc_data->setInverted(m_image_display->isInverted());
    QDialog::accept();
  }
}

void OcrDialog::applyValue()
{

  switch (m_change_type) {
  case Binarise: {
    int value = m_intvalue_slider->value();
    m_image_display->applyThreshold(value);
    break;
  }

  case Rescale: {
    qreal value = m_dblvalue_slider->value();
    m_image_display->applyRescale(value);
    break;
  }
  }
}


void OcrDialog::setCurrentValueType()
{
  QList<double> major_ticks, medium_ticks, minor_ticks;

  switch (m_change_type) {
  case Binarise:
    for (qreal i = 0; i < 255; i += 50) {
      major_ticks << i;
    }

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
    break;

  case Rescale:

    for (qreal i = 0.1; i < 1.0; i += 0.1) {
      major_ticks << i;
    }

    medium_ticks.clear();

    for (qreal i = 1.0; i <= 10.0; i += 1.0) {
      major_ticks << i;
    }

    for (qreal i = 0.1; i < 1.0; i += 0.01) {
      minor_ticks << i;
    }

    for (qreal i = 1.0; i < 10.0; i += 0.1) {
      minor_ticks << i;
    }

    m_dblvalue_slider->setScaleEngine(new QwtLogScaleEngine());
    m_dblvalue_slider->setScale(QwtScaleDiv(0.1, 10.0, minor_ticks, medium_ticks, major_ticks));
    m_dblvalue_slider->setValue(OcrImage::BASE_RESCALE);
    m_ctl_stack->setCurrentIndex(m_dblvalue_stack);
    break;
  }
}

void OcrDialog::setIntValueLabel(qreal value)
{
  m_intvalue_lbl->setText(QString::number(int(value)));
}

void OcrDialog::setDoubleValueLabel(qreal value)
{
  m_dblvalue_lbl->setText(QString::number(value, 'g', 2));
}

void OcrDialog::valueAccepted()
{
  m_ctl_stack->setCurrentIndex(m_btn_stack);
}

void OcrDialog::disableBinarise()
{
  m_binarise_btn->setEnabled(false);
  enableCleanImageBtns(true);
}

void OcrDialog::enableBinarise()
{
  m_binarise_btn->setEnabled(true);
}

void OcrDialog::enableCleanImageBtns(bool enable)
{
  denoise_btn->setEnabled(enable);
  dewarp_btn->setEnabled(enable);
  //  deskew_btn->setEnabled(enable);
}

void OcrDialog::resizeEvent(QResizeEvent* event)
{
  QSize parent_size = qobject_cast<QWidget*>(parent())->frameSize();
  auto size = parent_size;
  size -= QSize(100, 100);

  auto* e = new QResizeEvent(size, event->oldSize());
  QDialog::resizeEvent(e);
}
