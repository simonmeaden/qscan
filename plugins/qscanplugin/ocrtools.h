#ifndef OCRTOOLS_H
#define OCRTOOLS_H

#include <QObject>
#include <QThread>
#include <QtDebug>

#include "qscanlogging.h"
#include "documentdata.h"

class OcrWorker;

class OcrTools : public QObject
{
  Q_OBJECT
public:
  explicit OcrTools(const QString& datapath, const QString& lang, QObject* parent = nullptr);
  ~OcrTools() override;
  OcrTools(const OcrTools&) = delete;
  OcrTools& operator=(const OcrTools&) = delete;
  OcrTools(OcrTools&&) = delete;
  OcrTools& operator=(OcrTools&&) = delete;
  void convertImageToText(const DocumentData& doc_data);
  void convertImageToText(int page_no, const QImage& image, const QRect& rect);

signals:
  void finished();
  //  void startConverting(Page page);
  void convertedPage(const DocumentData& doc_data);
  void convertedImage(int page_no, const QString& text);
  void convertedImageRect(int page_no, const QString& text);
  void ocrFailed(int page_no);

protected:
  OcrWorker* m_ocr_worker;

};

#endif // OCRTOOLS_H
