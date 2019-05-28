#ifndef OCRWORKER_H
#define OCRWORKER_H

#include <QImage>
#include <QObject>
#include <QString>
#include <QThread>

#include "logger.h"
//#include "scanpage.h"
#include "documentdata.h"

class TessTools;

class OcrWorker : public QObject
{
  Q_OBJECT
public:
  explicit OcrWorker(QString  datapath, QString  lang);

  void process();
  void convertPage(const DocumentData& page);
  void convertImage(int page_no, const QImage& image);

  void stopRunning();

signals:
  void pageConverted(const DocumentData& page);
  void imageConverted(int page_no, const QString& text);
  void log(LogLevel, const QString&);

protected:
  QString m_datapath, m_lang;
  bool m_available;
  QList<DocumentData> m_pages{};
  QList<QImage> m_images;
  QList<int> m_page_nos;
  bool m_running;

};

#endif // OCRWORKER_H
