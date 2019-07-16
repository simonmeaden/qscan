#ifndef OCRWORKER_H
#define OCRWORKER_H

#include <QImage>
#include <QObject>
#include <QString>
#include <QThread>

#include "qscanlogging.h"
#include "documentdata.h"
#include "util.h"

class OcrWorker : public QObject
{
  Q_OBJECT
public:
  explicit OcrWorker(QString  datapath, QString  lang);

  void process();
  void convertPage(const DocumentData& doc_data);
  void convertImage(int page_no, const QImage& image, const QRect& rect);

  void stopRunning();

signals:
  void pageConverted(const DocumentData& doc_data);
  void imageConverted(int page_no, const QString& text);
  void imageConvertedRect(int page_no, const QString& text);

protected:
  QString m_datapath, m_lang;
  bool m_available;
  QList<DocumentData> m_doc_data{};
  QList<QImage> m_images;
  QList<int> m_page_nos;
  QList<QRect> m_rects;
  bool m_running;
};

#endif // OCRWORKER_H
