#include "ocrdialog.h"

OCRDialog::OCRDialog(QWidget* parent)
  : QDialog(parent)
  , m_text_edit(nullptr)
  , m_image_display(nullptr)
  , m_image_changed(false)
{
  initGui();
}

QImage
OCRDialog::image()
{
  return m_image;
}

void
OCRDialog::setImage(const QImage& image)
{
  m_image = image;
  m_image_display->setPixmap(QPixmap::fromImage(m_image));
  QSize size = m_image_display->frameSize();
  qreal scale_w = qreal(size.width()) / qreal(m_image.width());
  qreal scale_h = qreal(size.height()) / qreal(m_image.height());
  qreal factor = (scale_w < scale_h ? scale_w : scale_h);

  size = m_image.size();
  size *= factor;
  m_image_display->resize(size);
}

void
OCRDialog::receiveOcrText(const QString& text)
{
  m_text_edit->setPlainText(text);
}

QString
OCRDialog::text()
{
  return m_text_edit->toPlainText();
}

bool
OCRDialog::imageChanged() const
{
  return m_image_changed;
}

void
OCRDialog::initGui()
{
  auto* layout = new QGridLayout;
  setLayout(layout);

  auto* box_layout = new QHBoxLayout;
  layout->addLayout(box_layout, 0, 0);

  m_text_edit = new QPlainTextEdit(this);
  box_layout->addWidget(m_text_edit);

  m_image_display = new QLabel(this);
  box_layout->addWidget(m_image_display);

  auto* btn_box = new QDialogButtonBox(this);
  layout->addWidget(btn_box, 1, 0);

  auto* ocr_btn = new QPushButton("Do OCR", this);
  btn_box->addButton(ocr_btn, QDialogButtonBox::ActionRole);

  auto* tweaks_btn = new QPushButton("Apply Tweaks", this);
  btn_box->addButton(tweaks_btn, QDialogButtonBox::ActionRole);

  btn_box->addButton(QDialogButtonBox::Close);
  btn_box->addButton(QDialogButtonBox::Discard);
  btn_box->addButton(QDialogButtonBox::Help);

  connect(ocr_btn, &QPushButton::clicked, this, &OCRDialog::applyOcr);
  connect(tweaks_btn, &QPushButton::clicked, this, &OCRDialog::applyTweaks);
  connect(btn_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(btn_box, &QDialogButtonBox::helpRequested, this, &OCRDialog::help);
  connect(btn_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void
OCRDialog::applyOcr()
{
  emit sendOcrImage(m_image);
}

void
OCRDialog::applyTweaks()
{
  // TODO tweaks.
  // make some tweaks to the image then set the changed flag.
  // m_image_changed = true;
}

void
OCRDialog::help()
{
  // TODO help
}
