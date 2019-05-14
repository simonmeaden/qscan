#ifndef SCANPAGE_H
#define SCANPAGE_H

#include <QImage>
#include <QObject>
#include <QSharedPointer>

#include <opencv2/opencv.hpp>

class ScanPage
{
public:
  explicit ScanPage();

  QString imagePath() const;
  void setImage(const QString& imagePath);

  QString text() const;
  void setText(const QString& text);

protected:
  QString m_image_path;
  QString m_text;
};
using Page = QSharedPointer<ScanPage>;
using PageList = QList<Page>;

Q_DECLARE_METATYPE(ScanPage)
Q_DECLARE_METATYPE(Page)

#endif // SCANPAGE_H
