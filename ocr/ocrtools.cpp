#include <utility>

#include <utility>

#include <utility>

#include "ocrtools.h"
#include "ocrworker.h"

OcrTools::OcrTools(const QString& datapath,
                   const QString& lang,
                   QObject* parent)
  : QObject(parent)
{

  auto* thread = new QThread;
  auto* ocr_worker = new OcrWorker(datapath, lang);

  // cleanup
  connect(thread, &QThread::started, ocr_worker, &OcrWorker::process);
  connect(this, &OcrTools::finished, thread, &QThread::quit);
  connect(thread, &QThread::finished, ocr_worker, &OcrWorker::deleteLater);
  connect(thread, &QThread::finished, thread, &QThread::deleteLater);

  connect(
    this, &OcrTools::startConverting, ocr_worker, &OcrWorker::convertToString);
  connect(ocr_worker, &OcrWorker::converted, this, &OcrTools::converted);
  connect(ocr_worker, &OcrWorker::log, this, &OcrTools::log);

  thread->start();
}

OcrTools::~OcrTools()
{
  emit finished();
}

void OcrTools::convertImage(Page page)
{
  emit startConverting(std::move(page));
}
