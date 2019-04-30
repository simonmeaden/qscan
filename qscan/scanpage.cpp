#include "scanpage.h"

ScanPage::ScanPage(QObject* parent)
  : QObject(parent)
{}

QImage
ScanPage::image() const
{
  return m_image;
}

void
ScanPage::setImage(const QImage& image)
{
  m_image = image;
}

QString
ScanPage::text() const
{
  return m_text;
}

void
ScanPage::setText(const QString& text)
{
  m_text = text;
}

QImage
ScanPage::thumbnail() const
{
  int w = m_image.width();
  qreal factor = w / 100.0;
  int h = int(m_image.height() / factor);
  QSize size(w, h);
  QImage thumbnail = m_image.scaled(w, h, Qt::KeepAspectRatio);
  return thumbnail;
}
