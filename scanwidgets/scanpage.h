#ifndef SCANPAGE_H
#define SCANPAGE_H

#include <QImage>
#include <QObject>
#include <QSharedPointer>
#include <QStringList>

#include <opencv2/opencv.hpp>

class ScanPage
{
public:
  explicit ScanPage();

  QString imagePath() const;
  void setImagePath(const QString& imagePath);

  QStringList text() const;
  QString text(int index) const;
  void setText(const QString& text);
  void setText(int row, const QString& text);
  void setTextList(const QStringList& text);

protected:
  QString m_image_path;
  QStringList m_text;
};
using Page = QSharedPointer<ScanPage>;
using PageList = QList<Page>;

Q_DECLARE_METATYPE(ScanPage)
Q_DECLARE_METATYPE(Page)

#endif // SCANPAGE_H
