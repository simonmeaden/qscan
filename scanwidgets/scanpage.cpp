#include "scanpage.h"

ScanPage::ScanPage()
{}

QString ScanPage::imagePath() const
{
  return m_image_path;
}

void ScanPage::setImage(const QString& image)
{
  m_image_path = image;
}

QString ScanPage::text() const
{
  return m_text;
}

void ScanPage::setText(const QString& text)
{
  m_text = text;
}
