#include "ocrdialog.h"

#include "ocrimage.h"
#include "scanedit.h"

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

void OcrDialog::setData(int index, const QImage& image, const DocumentData&  page)
{
  m_page_no = index;
  m_page = page;
  m_page_no = index,
  m_image_display->setImage(image);
  m_text_edit->setText(page->textList());
}

void OcrDialog::setOcrImage(int index, const QImage& image)
{
  m_page_no = index,
  m_image_display->setImage(image);
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

DocumentData OcrDialog::page() const
{
  return m_page;
}

void OcrDialog::open()
{
  QDialog::open();
  m_image_display->fitByType();
}

void OcrDialog::initGui()
{
  auto* layout = new QGridLayout;
  setLayout(layout);

  m_text_edit = new ScanEdit(this);
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
  connect(m_image_display, &OcrImage::thresholdAccepted, this, &OcrDialog::thresholdAccepted);
  layout->addWidget(m_image_display, 0, 2);

  auto* threshold_frame = new QFrame();
  auto* threshold_layout = new QGridLayout;
  threshold_frame->setLayout(threshold_layout);
  m_threshold_stack = m_ctl_stack->addWidget(threshold_frame);
  {
    // initialise threshold frame
    auto* threshold_slider = new QSlider(Qt::Vertical, this);
    threshold_slider->setMinimum(0);
    threshold_slider->setMaximum(255);
    threshold_slider->setValue(200); // 200 is a reasonable start point for threshold.
    connect(threshold_slider, &QSlider::valueChanged, m_image_display, &OcrImage::setThreshold);
    connect(threshold_slider, &QSlider::valueChanged, this, &OcrDialog::setThreshold);
    threshold_layout->addWidget(threshold_slider, 0, 0, Qt::AlignHCenter);

    threshold_lbl = new QLabel("200", this);
    QFont font;
    font.setWeight(QFont::Bold);
    font.setPixelSize(40);
    threshold_lbl->setFont(font);
    threshold_lbl->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    threshold_layout->addWidget(threshold_lbl, 0, 1);

    auto* apply_thresh_btn = new QPushButton(tr("Apply Threshold"), this);
    connect(apply_thresh_btn, &QPushButton::clicked, m_image_display, &OcrImage::applyThreshold);
    threshold_layout->addWidget(apply_thresh_btn, 2, 0, 1, 2);

    auto* accept_thresh_btn = new QPushButton(tr("Accept"), this);
    connect(accept_thresh_btn, &QPushButton::clicked, m_image_display, &OcrImage::acceptThreshold);
    threshold_layout->addWidget(accept_thresh_btn, 3, 0, 1, 2);

    auto* cancel_thresh_btn = new QPushButton(tr("Cancel"), this);
    connect(cancel_thresh_btn, &QPushButton::clicked, m_image_display, &OcrImage::cancelThreshold);
    threshold_layout->addWidget(cancel_thresh_btn, 4, 0, 1, 2);
  }

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

    m_cut_btn = new QPushButton(tr("Cut"), this);
    m_cut_btn->setToolTip(tr("Remove section from the image image."));
    m_cut_btn->setEnabled(false);
    connect(m_cut_btn, &QPushButton::clicked, m_image_display, &OcrImage::cutSelection);
    btn_layout->addWidget(m_cut_btn);

    m_crop_btn = new QPushButton(tr("Crop"), this);
    m_crop_btn->setToolTip(tr("Crop the image to remove excess image."));
    m_crop_btn->setEnabled(false);
    connect(m_crop_btn, &QPushButton::clicked, m_image_display, &OcrImage::cropToSelection);
    btn_layout->addWidget(m_crop_btn);

    m_binarise_btn = new QPushButton(tr("Binarise"), this);
    m_binarise_btn->setToolTip(tr("Binarisation turns the image to blackl and white.\n"
                                  "OCR text is generally better on black and white images."));
    connect(m_binarise_btn, &QPushButton::clicked, this, &OcrDialog::binarise);
    connect(m_image_display, &OcrImage::enableBinarise, this, &OcrDialog::enableBinarise);
    connect(m_image_display, &OcrImage::disableBinarise, this, &OcrDialog::disableBinarise);
    btn_layout->addWidget(m_binarise_btn);

    auto* invert_btn = new QPushButton(tr("Invert Image"), this);
    invert_btn->setToolTip(tr("Converts from black text on white to white on black and back."));
    connect(invert_btn, &QPushButton::clicked, this, &OcrDialog::invert);
    btn_layout->addWidget(invert_btn);

    auto* denoise_btn = new QPushButton(tr("De-noise"), this);
    denoise_btn->setToolTip(tr("Noise removal attempts to clean up the image."));
    connect(denoise_btn, &QPushButton::clicked, this, &OcrDialog::denoise);
    btn_layout->addWidget(denoise_btn);

    auto* dewarp_btn = new QPushButton(tr("De-warp"), this);
    dewarp_btn->setToolTip(tr("Straigntens lines that are curved."));
    connect(dewarp_btn, &QPushButton::clicked, this, &OcrDialog::dewarp);
    btn_layout->addWidget(dewarp_btn);

    auto* descew_btn = new QPushButton(tr("De-scew"), this);
    descew_btn->setToolTip(tr("Rotates non-horizontal up lines."));
    connect(descew_btn, &QPushButton::clicked, this, &OcrDialog::descew);
    btn_layout->addWidget(descew_btn);

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
    emit sendOcrRequest(m_page_no, copy) ;
  }
}

void OcrDialog::help()
{
  // TODO help
}

void OcrDialog::setSelected()
{
  m_crop_btn->setEnabled(true);
  m_cut_btn->setEnabled(true);
}

void OcrDialog::setUnselected()
{
  m_crop_btn->setEnabled(false);
  m_cut_btn->setEnabled(false);
}

void OcrDialog::binarise()
{
  m_ctl_stack->setCurrentIndex(m_threshold_stack);
  m_image_display->binarise();
}

void OcrDialog::invert()
{
  m_image_display->invert();
}

void OcrDialog::denoise()
{
  m_image_display->denoise();
}

void OcrDialog::dewarp()
{
  // TODO dewarp
}

void OcrDialog::descew()
{
  // TODO descew
}

void OcrDialog::rescale()
{
  // TODO rescale
}

void OcrDialog::saveText()
{
  emit saveModifiedText(m_page_no, m_text_edit->text());
}

void OcrDialog::saveImage()
{
  emit saveModifiedImage(m_page_no, m_image_display->modifiedImage());
}

void OcrDialog::discard()
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

void OcrDialog::undoChanges()
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

void OcrDialog::close()
{
  saveImage();
  saveText();
  QDialog::close();
}

void OcrDialog::setThreshold(int threshold)
{
  threshold_lbl->setText(QString::number(threshold));
}

void OcrDialog::thresholdAccepted()
{
  m_ctl_stack->setCurrentIndex(m_btn_stack);
}

void OcrDialog::disableBinarise()
{
  m_binarise_btn->setEnabled(false);
}

void OcrDialog::enableBinarise()
{
  m_binarise_btn->setEnabled(true);
}

void OcrDialog::resizeEvent(QResizeEvent* event)
{
  QSize parent_size = qobject_cast<QWidget*>(parent())->frameSize();
  auto size = parent_size;
  size -= QSize(100, 100);

  auto* e = new QResizeEvent(size, event->oldSize());
  QDialog::resizeEvent(e);

}
