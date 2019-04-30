#ifndef OCR_H
#define OCR_H

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QImage>
#include <QMessageBox>
#include <QObject>

#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>

#include <log4qt/logger.h>

#include "ocr_global.h"

class OCRSHARED_EXPORT Ocr : public QObject
{
  Q_OBJECT
public:
  Ocr(QObject* parent = nullptr);

  QString makeString(const QImage& qImage, int page);

  PIX* qImage2PIX(const QImage& qImage);
  QImage PIX2qImage(PIX* pixImage);
  QImage GetThresholded(const QImage& qImage);
  const char* qString2Char(QString string);
  QList<QString> getLanguages(const QString& datapath);

protected:
  Log4Qt::Logger* m_logger;

  //  tesseract::TessBaseAPI* api;
  QImage m_image;

  QString getDataPath();
  QString getLang();
  //  void msg(QString messageText);
  const char* kTrainedDataSuffix;
};

#endif // OCR_H
