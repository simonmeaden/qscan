#include <utility>

#include <utility>

#include "ocrtools.h"

#include "ocrworker.h"

OcrTools::OcrTools(QString datapath, QString lang, QObject* parent)
  : QObject(parent)
{

  auto* thread = new QThread;
  auto* ocr_worker = new OcrWorker(std::move(datapath), std::move(lang));

  // cleanup
  connect(this, &OcrTools::finished, thread, &QThread::quit);
  connect(thread, &QThread::finished, ocr_worker, &OcrWorker::deleteLater);
  connect(thread, &QThread::finished, thread, &QThread::deleteLater);

  connect(
    this, &OcrTools::startConverting, ocr_worker, &OcrWorker::convertToString);
  connect(ocr_worker, &OcrWorker::converted, this, &OcrTools::converted);
  connect(ocr_worker, &OcrWorker::log, this, &OcrTools::log);
}

OcrTools::~OcrTools()
{
  emit finished();
}

void
OcrTools::convertImage(int page, const QImage& image)
{
  emit startConverting(page, image);
}
