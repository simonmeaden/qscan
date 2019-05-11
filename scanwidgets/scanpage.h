#ifndef SCANPAGE_H
#define SCANPAGE_H

#include <QImage>
#include <QObject>

#include <opencv2/opencv.hpp>

class ScanPage : public QObject
{
  Q_OBJECT
public:
  explicit ScanPage(QObject* parent = nullptr);

  QString imagePath() const;
  void setImage(const QString& imagePath);

  QString text() const;
  void setText(const QString& text);

  //  QImage thumbnail() const;

  cv::Mat matImage() const;
  void setMatImage(const cv::Mat& mat_image);

signals:

protected:
  //  QImage m_image;
  cv::Mat m_mat_image;
  QString m_image;
  QString m_text;
};
using Page = QSharedPointer<ScanPage>;
using PageList = QList<Page>;

#endif // SCANPAGE_H
