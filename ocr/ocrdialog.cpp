#include "ocrdialog.h"

#include "ocrimage.h"

OCRDialog::OCRDialog(QWidget* parent)
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

QImage OCRDialog::image()
{
  return m_image_display->image();
}

void OCRDialog::setData(int index, const QImage& image, const Page&  page)
{
  m_page_no = index;
  m_page = page;
  setOcrImage(image);
  setOcrText(page->text());
}

void OCRDialog::setOcrImage(const QImage& image)
{
  m_image_display->setImage(image);
}

void OCRDialog::setOcrText(const QString& text)
{
  m_text_edit->setText(text);
}

QString OCRDialog::text()
{
  return m_text_edit->toPlainText();
}

bool OCRDialog::imageChanged() const
{
  return m_image_changed;
}

QSize OCRDialog::sizeHint() const
{
  QWidget* widget = qobject_cast<QWidget*>(parent());
  QSize size = widget->sizeHint();
  //  QPoint pos = widget->pos();
  size -= QSize(100, 100);
  //  pos -= QPoint(50,50);
  return { size };
}

void OCRDialog::initGui()
{
  auto* layout = new QGridLayout;
  setLayout(layout);

  m_text_edit = new QTextEdit(this);
  layout->addWidget(m_text_edit, 0, 0);

  auto* stacked_frame = new QFrame(this);
  m_ctl_stack = new QStackedLayout;
  stacked_frame->setLayout(m_ctl_stack);

  auto* threshold_frame = new QFrame();
  auto* threshold_layout = new QGridLayout;
  threshold_frame->setLayout(threshold_layout);
  m_threshold_stack = m_ctl_stack->addWidget(threshold_frame);
  {
    auto* threshold_slider = new QSlider(Qt::Vertical, this);
    threshold_slider->setMinimum(0);
    threshold_slider->setMaximum(255);
    threshold_slider->setValue(255);
    threshold_layout->addWidget(threshold_slider, 0, 0);
    connect(threshold_slider, &QSlider::valueChanged, m_image_display, &OcrImage::setThreshold);

    auto* accept_thresh_btn = new QPushButton(tr("Accept"), this);
    connect(accept_thresh_btn, &QPushButton::clicked, m_image_display, &OcrImage::acceptThreshold);
    threshold_layout->addWidget(accept_thresh_btn, 1, 0);

    auto* cancel_thresh_btn = new QPushButton(tr("Cancel"), this);
    connect(cancel_thresh_btn, &QPushButton::clicked, m_image_display, &OcrImage::cancelThreshold);
    threshold_layout->addWidget(cancel_thresh_btn, 2, 0);
  }

  auto* btn_box = new QFrame(this);
  auto* btn_layout = new QVBoxLayout;
  btn_box->setLayout(btn_layout);
  m_btn_stack = m_ctl_stack->addWidget(btn_box);
  {
    // initialise button box

    auto* ocr_btn = new QPushButton(tr("Run OCR"), this);
    ocr_btn->setToolTip(tr("Re run the OCR on tweaked image."));
    connect(ocr_btn, &QPushButton::clicked, this, &OCRDialog::applyOcr);
    btn_layout->addWidget(ocr_btn);

    btn_layout->addStretch(1);

    crop_btn = new QPushButton(tr("Crop"), this);
    crop_btn->setToolTip(tr("Crop the image to remove excess image."));
    connect(crop_btn, &QPushButton::clicked, m_image_display, &BaseScanImage::cropToSelection);
    btn_layout->addWidget(crop_btn);

    auto* binarise_btn = new QPushButton(tr("Binarise"), this);
    binarise_btn->setToolTip(tr("Binarisation turns the image to blackl and white."));
    connect(binarise_btn, &QPushButton::clicked, this, &OCRDialog::binarise);
    btn_layout->addWidget(binarise_btn);

    auto* denoise_btn = new QPushButton(tr("De-noise"), this);
    denoise_btn->setToolTip(tr("Noise removal attempts to clean up the image."));
    connect(denoise_btn, &QPushButton::clicked, this, &OCRDialog::denoise);
    btn_layout->addWidget(denoise_btn);

    auto* dewarp_btn = new QPushButton(tr("De-warp"), this);
    dewarp_btn->setToolTip(tr("Straigntens lines that are curved."));
    connect(dewarp_btn, &QPushButton::clicked, this, &OCRDialog::dewarp);
    btn_layout->addWidget(dewarp_btn);

    auto* descew_btn = new QPushButton(tr("De-scew"), this);
    descew_btn->setToolTip(tr("Rotates non-horizontal up lines."));
    connect(descew_btn, &QPushButton::clicked, this, &OCRDialog::descew);
    btn_layout->addWidget(descew_btn);

    auto* rescale_btn = new QPushButton(tr("Rescale"), this);
    rescale_btn->setToolTip(tr("Increase the DPI of the image."));
    connect(rescale_btn, &QPushButton::clicked, this, &OCRDialog::rescale);
    btn_layout->addWidget(rescale_btn);

    auto* undo_btn = new QPushButton(tr("Undo last operation"), this);
    undo_btn->setToolTip(tr("Undo changes and revert to previous image."));
    connect(undo_btn, &QPushButton::clicked, this, &OCRDialog::undoChanges);
    btn_layout->addWidget(undo_btn);

    btn_layout->addStretch(1);

    auto* save_txt_btn = new QPushButton(tr("Save Text"), this);
    save_txt_btn->setToolTip(tr("Save the text."));
    connect(save_txt_btn, &QPushButton::clicked, this, &OCRDialog::saveText);
    btn_layout->addWidget(save_txt_btn);

    auto* save_img_btn = new QPushButton(tr("Save Image"), this);
    save_img_btn->setToolTip(tr("Save the image."));
    connect(save_img_btn, &QPushButton::clicked, this, &OCRDialog::saveImage);
    btn_layout->addWidget(save_img_btn);

    auto* discard_btn = new QPushButton(tr("Discard changes"), this);
    discard_btn->setToolTip(tr("Closes the dialog, discarding any changes."));
    connect(discard_btn, &QPushButton::clicked, this, &OCRDialog::discard);
    btn_layout->addWidget(discard_btn);

    btn_layout->addStretch(1);

    auto* help_btn = new QPushButton(tr("Help"), this);
    help_btn->setToolTip(tr("Help."));
    connect(help_btn, &QPushButton::clicked, this, &OCRDialog::help);
    btn_layout->addWidget(help_btn);
  }

  m_ctl_stack->setCurrentIndex(m_btn_stack);

  m_image_display = new OcrImage(this);
  connect(m_image_display, &OcrImage::selected, this, &OCRDialog::setSelected);
  connect(m_image_display, &OcrImage::unselected, this, &OCRDialog::setUnselected);
  layout->addWidget(m_image_display, 0, 2);

  layout->setColumnStretch(0, 3);
  layout->setColumnStretch(1, 1);
  layout->setColumnStretch(2, 3);

}

void OCRDialog::applyOcr()
{
  emit sendOcrImage(m_image_display->image());
}

void OCRDialog::help()
{
  // TODO help
}

void OCRDialog::setSelected()
{
  crop_btn->setEnabled(true);
}

void OCRDialog::setUnselected()
{
  crop_btn->setEnabled(false);
}

void OCRDialog::binarise()
{
  m_ctl_stack.setCurrentIndex(m_threshold_stack);
  m_image_display->binarise();
}

void OCRDialog::denoise()
{
  // TODO denoise
}

void OCRDialog::dewarp()
{
  // TODO dewarp
}

void OCRDialog::descew()
{
  // TODO descew
}

void OCRDialog::rescale()
{
  // TODO rescale
}

void OCRDialog::saveText()
{
  emit saveModifiedText(m_page_no, m_text_edit->toPlainText());
}

void OCRDialog::saveImage()
{
  emit saveModifiedImage(m_page_no, m_image_display->modifiedImage());
}

void OCRDialog::discard()
{
  int result = QMessageBox::warning(
                 this,
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

void OCRDialog::undoChanges()
{
  int result = QMessageBox::warning(
                 this,
                 tr("Discaring Changes"),
                 tr("You are about to discard any changes you have made\n"
                    "This cannot be undone\n"
                    "Are you sure?"),
                 QMessageBox::Yes | QMessageBox::No,
                 QMessageBox::No);

  if (result == QMessageBox::Yes) {
    m_image_display->undoAllChanges();
  }
}

void OCRDialog::resizeEvent(QResizeEvent* event)
{
  QSize parent_size = qobject_cast<QWidget*>(parent())->frameSize();
  auto size = parent_size;
  size -= QSize(100, 100);

  auto* e = new QResizeEvent(size, event->oldSize());
  QDialog::resizeEvent(e);

}