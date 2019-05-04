#ifndef OCRWORKER_H
#define OCRWORKER_H

#include <QImage>
#include <QObject>
#include <QString>
#include <QThread>

#include "logger.h"
#include "scanpage.h"

class TessTools;

class OcrWorker : public QObject
{
  Q_OBJECT
public:
  explicit OcrWorker(const QString& datapath,
                     const QString& lang,
                     QObject* parent = nullptr);

  void convertToString(const Page& page);
  void process();

signals:
  void converted(Page page);
  void log(LogLevel, const QString&);

protected:
  TessTools* m_tesstools;
  bool m_available;
  QList<Page> m_images;
};

#endif // OCRWORKER_H
