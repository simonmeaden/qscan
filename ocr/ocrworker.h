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
  explicit OcrWorker(QString  datapath, QString  lang);

  void process();
  void convertImage(const Page& page);

  void stopRunning();

signals:
  void converted(const Page& page);
  void log(LogLevel, const QString&);

protected:
  QString m_datapath, m_lang;
  std::atomic<bool> m_available;
  QList<Page> m_images;
  std::atomic<bool> m_running;

};

#endif // OCRWORKER_H
