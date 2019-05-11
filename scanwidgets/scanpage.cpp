#include "scanpage.h"

ScanPage::ScanPage(QObject* parent)
  : QObject(parent)
{}

QString ScanPage::imagePath() const
{
  return m_image;
}

void ScanPage::setImage(const QString& image)
{
  m_image = image;
}

QString ScanPage::text() const
{
  return m_text;
}

void ScanPage::setText(const QString& text)
{
  m_text = text;
}

cv::Mat ScanPage::matImage() const
{
  return m_mat_image;
}

void ScanPage::setMatImage(const cv::Mat& mat_image)
{
  m_mat_image = mat_image;
}

//QImage ScanPage::thumbnail() const
//{
//  if (!m_image.isNull()) {
//    int w = m_image.width();
//    qreal factor = w / 100.0;
//    w = 100;
//    int h = int(m_image.height() / factor);
//    QSize size(w, h);
//    QImage thumbnail = m_image.scaled(w, h, Qt::KeepAspectRatio);
//    return thumbnail;
//  }

//  return  QImage();
//}
