#ifndef SCANPAGE_H
#define SCANPAGE_H

#include <QImage>
#include <QObject>

class ScanPage : public QObject
{
  Q_OBJECT
public:
  explicit ScanPage(QObject* parent = nullptr);

  QImage image() const;
  void setImage(const QImage& image);

  QString text() const;
  void setText(const QString& text);

  QImage thumbnail() const;

signals:

protected:
  QImage m_image;
  QString m_text;
};
using Page = QSharedPointer<ScanPage>;
using PageList = QList<Page>;

#endif // SCANPAGE_H
