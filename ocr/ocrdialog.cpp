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
  return {1000, 600};
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
  btn_box->addButton(ocr_btn, QDialogButtonBox::ActionRole);

  crop_btn = new QPushButton(tr("Crop"), this);
  crop_btn->setToolTip(tr("Crop the image to remove excess image."));
  connect(crop_btn, &QPushButton::clicked, m_image_display, &OcrImage::cropToSelection);
  btn_box->addButton(crop_btn, QDialogButtonBox::ActionRole);

  auto* binarise_btn = new QPushButton(tr("Binarise"), this);
  binarise_btn->setToolTip(tr("Binarisation turns the image to blackl and white."));
  btn_box->addButton(binarise_btn, QDialogButtonBox::ActionRole);

  auto* denoise_btn = new QPushButton(tr("De-noise"), this);
  denoise_btn->setToolTip(tr("Noise removal attempts to clean up the image."));
  btn_box->addButton(denoise_btn, QDialogButtonBox::ActionRole);

  auto* rescale_btn = new QPushButton(tr("Rescale"), this);
  btn_box->addButton(rescale_btn, QDialogButtonBox::ActionRole);

  btn_box->addButton(QDialogButtonBox::Save);
  btn_box->addButton(QDialogButtonBox::Discard);
  btn_box->addButton(QDialogButtonBox::Close);
  btn_box->addButton(QDialogButtonBox::Help);

  connect(ocr_btn, &QPushButton::clicked, this, &OCRDialog::applyOcr);
  connect(btn_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(btn_box, &QDialogButtonBox::helpRequested, this, &OCRDialog::help);
  connect(btn_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
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

void OCRDialog::rescale()
{
  // TODO rescale
}
