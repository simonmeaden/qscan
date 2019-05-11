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
  return m_image;
}

void OCRDialog::setImage(const QImage& image)
{
  m_image = image;
  QImage scaled_image = m_image.scaled(600, 600, Qt::KeepAspectRatio);
  m_image_display->setPixmap(QPixmap::fromImage(scaled_image));
  QSize size = m_image_display->frameSize();
  qreal scale_w = qreal(size.width()) / qreal(scaled_image.width());
  qreal scale_h = qreal(size.height()) / qreal(scaled_image.height());
  qreal factor = (scale_w < scale_h ? scale_w : scale_h);

  size = scaled_image.size();
  size *= factor;
  m_image_display->resize(size);
}

void OCRDialog::setOcrText(const QString& text)
{
  m_text_edit->setPlainText(text);
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

  m_text_edit = new QPlainTextEdit(this);
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

  auto* save_btn = new QPushButton(tr("Save Text"), this);
  save_btn->setToolTip(tr("Increase the DPI of the image."));
  connect(save_btn, &QPushButton::clicked, this, &OCRDialog::save);
  btn_box->addButton(save_btn, QDialogButtonBox::ActionRole);

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
  emit sendOcrImage(m_image);
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

void OCRDialog::save()
{
  accept();
}

void OCRDialog::discard()
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
    reject();
  }
}
