#include "ocrdialog.h"

#include "ocrimage.h"

OCRDialog::OCRDialog(QWidget* parent)
  : QDialog(parent)
  , m_text_edit(nullptr)
  , m_image_display(nullptr)
  , m_image_changed(false)
{
  initGui();
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
  return { 1000, 600 };
}

void OCRDialog::initGui()
{
  auto* layout = new QGridLayout;
  setLayout(layout);

  m_text_edit = new QTextEdit(this);
  layout->addWidget(m_text_edit, 0, 0);

  auto* btn_box = new QDialogButtonBox(Qt::Vertical, this);
  layout->addWidget(btn_box, 0, 1);

  m_image_display = new OcrImage(this);
  connect(m_image_display, &OcrImage::selected, this, &OCRDialog::setSelected);
  connect(m_image_display, &OcrImage::unselected, this, &OCRDialog::setUnselected);
  layout->addWidget(m_image_display, 0, 2);

  auto* ocr_btn = new QPushButton(tr("Run OCR"), this);
  ocr_btn->setToolTip(tr("Re run the OCR on tweaked image."));
  connect(ocr_btn, &QPushButton::clicked, this, &OCRDialog::applyOcr);
  btn_box->addButton(ocr_btn, QDialogButtonBox::ActionRole);

  crop_btn = new QPushButton(tr("Crop"), this);
  crop_btn->setToolTip(tr("Crop the image to remove excess image."));
  connect(crop_btn, &QPushButton::clicked, m_image_display, &OcrImage::cropToSelection);
  btn_box->addButton(crop_btn, QDialogButtonBox::ActionRole);

  auto* binarise_btn = new QPushButton(tr("Binarise"), this);
  binarise_btn->setToolTip(tr("Binarisation turns the image to blackl and white."));
  connect(binarise_btn, &QPushButton::clicked, this, &OCRDialog::binarise);
  btn_box->addButton(binarise_btn, QDialogButtonBox::ActionRole);

  auto* denoise_btn = new QPushButton(tr("De-noise"), this);
  denoise_btn->setToolTip(tr("Noise removal attempts to clean up the image."));
  connect(denoise_btn, &QPushButton::clicked, this, &OCRDialog::denoise);
  btn_box->addButton(denoise_btn, QDialogButtonBox::ActionRole);

  auto* dewarp_btn = new QPushButton(tr("De-warp"), this);
  dewarp_btn->setToolTip(tr("Straigntens lines that are curved."));
  connect(dewarp_btn, &QPushButton::clicked, this, &OCRDialog::dewarp);
  btn_box->addButton(dewarp_btn, QDialogButtonBox::ActionRole);

  auto* descew_btn = new QPushButton(tr("De-scew"), this);
  descew_btn->setToolTip(tr("Rotates non-horizontal up lines."));
  connect(descew_btn, &QPushButton::clicked, this, &OCRDialog::descew);
  btn_box->addButton(descew_btn, QDialogButtonBox::ActionRole);

  auto* rescale_btn = new QPushButton(tr("Rescale"), this);
  rescale_btn->setToolTip(tr("Increase the DPI of the image."));
  connect(rescale_btn, &QPushButton::clicked, this, &OCRDialog::rescale);
  btn_box->addButton(rescale_btn, QDialogButtonBox::ActionRole);

  auto* save_txt_btn = new QPushButton(tr("Save Text"), this);
  save_txt_btn->setToolTip(tr("Save the text."));
  connect(save_txt_btn, &QPushButton::clicked, this, &OCRDialog::saveText);
  btn_box->addButton(save_txt_btn, QDialogButtonBox::ActionRole);

  auto* save_img_btn = new QPushButton(tr("Save Image"), this);
  save_img_btn->setToolTip(tr("Save the image."));
  connect(save_img_btn, &QPushButton::clicked, this, &OCRDialog::saveImage);
  btn_box->addButton(save_img_btn, QDialogButtonBox::ActionRole);

  auto* undo_btn = new QPushButton(tr("Undo changes"), this);
  undo_btn->setToolTip(tr("Undo changes and revert to original image."));
  connect(undo_btn, &QPushButton::clicked, this, &OCRDialog::undoChanges);
  btn_box->addButton(undo_btn, QDialogButtonBox::ActionRole);

  auto* discard_btn = new QPushButton(tr("Discard changes"), this);
  discard_btn->setToolTip(tr("Closes the dialog, discarding any changes."));
  connect(discard_btn, &QPushButton::clicked, this, &OCRDialog::discard);
  btn_box->addButton(discard_btn, QDialogButtonBox::ActionRole);

  auto* help_btn = new QPushButton(tr("Help"), this);
  help_btn->setToolTip(tr("Help."));
  connect(help_btn, &QPushButton::clicked, this, &OCRDialog::help);
  btn_box->addButton(help_btn, QDialogButtonBox::ActionRole);
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
  // TODO binarise
  cv::Mat mat = m_page->matImage();
  cv::Mat gray;
  cv::cvtColor(mat, gray, CV_BGR2GRAY);
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
    m_image_display->undoChanges();
  }
}
