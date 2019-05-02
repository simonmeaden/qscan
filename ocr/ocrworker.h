#ifndef OCRWORKER_H
#define OCRWORKER_H

#include <QImage>
#include <QObject>
#include <QString>
#include <QThread>

#include "logger.h"

class TessTools;

class OcrWorker : public QObject
{
  Q_OBJECT
public:
  explicit OcrWorker(const QString& datapath,
                     const QString& lang,
                     QObject* parent = nullptr);

  void convertToString(int page, const QImage& image);
  void process();

signals:
  void converted(int page, const QString&);
  void log(LogLevel, const QString&);

protected:
  TessTools* m_tesstools;
  bool m_available;
  QList<int> m_pages;
  QList<QImage> m_images;
};

#endif // OCRWORKER_H
