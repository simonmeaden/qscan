#ifndef OCRTOOLS_H
#define OCRTOOLS_H

#include <QObject>
#include <QThread>

#include "logger.h"

#include "ocr_global.h"
#include "scanpage.h"
#include "logger.h"

class TessTools;
class OcrWorker;

class OCRSHARED_EXPORT OcrTools : public QObject
{
  Q_OBJECT
public:
  explicit OcrTools(const QString& datapath, const QString& lang, QObject* parent = nullptr);
  ~OcrTools() override;
  OcrTools(const OcrTools&) = delete;
  OcrTools& operator=(const OcrTools&) = delete;
  OcrTools(OcrTools&&) = delete;
  OcrTools& operator=(OcrTools&&) = delete;
  void convertImageToText(const Page& page);

signals:
  void finished();
  //  void startConverting(Page page);
  void converted(const Page& page);
  void convertImage(const Page& page);


public slots:

protected:
  OcrWorker* m_ocr_worker;
  Log4Qt::Logger* m_logger;

  void log(LogLevel, const QString&);
  //  void converted(const Page& page);

};

#endif // OCRTOOLS_H
