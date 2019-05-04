#ifndef OCRTOOLS_H
#define OCRTOOLS_H

#include <QObject>
#include <QThread>

#include "logger.h"

#include "ocr_global.h"
#include "scanpage.h"

class TessTools;

class OCRSHARED_EXPORT OcrTools : public QObject
{
  Q_OBJECT
public:
  explicit OcrTools(const QString& datapath,
                    const QString& lang,
                    QObject* parent = nullptr);
  ~OcrTools() override;

  void convertImage(Page page);

signals:
  void finished();
  void startConverting(Page page);
  void converted(Page page);
  void log(LogLevel, const QString&);

public slots:

protected:
};

#endif // OCRTOOLS_H
