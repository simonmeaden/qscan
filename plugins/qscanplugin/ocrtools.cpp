#include <utility>

#include "ocrtools.h"
#include "ocrworker.h"


OcrTools::OcrTools(const QString& datapath,
                   const QString& lang, QObject* parent)
  : QObject(parent)
{

  auto* thread = new QThread;

  m_ocr_worker = new OcrWorker(datapath, lang);

  // cleanup
  connect(thread, &QThread::started, m_ocr_worker, &OcrWorker::process);
  connect(this, &OcrTools::finished, thread, &QThread::quit);
  connect(thread, &QThread::finished, m_ocr_worker, &OcrWorker::deleteLater);
  connect(thread, &QThread::finished, thread, &QThread::deleteLater);

  connect(m_ocr_worker, &OcrWorker::pageConverted, this, &OcrTools::convertedPage);
  connect(m_ocr_worker, &OcrWorker::imageConverted, this, &OcrTools::convertedImage);
  connect(m_ocr_worker, &OcrWorker::imageConvertedRect, this, &OcrTools::convertedImageRect);

  qCInfo(QscanOcr) << tr("Starting OCR Thread");

  m_ocr_worker->moveToThread(thread);
  thread->start();
}

OcrTools::~OcrTools()
{
  m_ocr_worker->stopRunning();
}

void OcrTools::convertImageToText(const DocumentData& doc_data)
{
  m_ocr_worker->convertPage(doc_data);
}

void OcrTools::convertImageToText(int page_no, const QImage& image, const QRect& rect)
{
  m_ocr_worker->convertImage(page_no, image, rect);
}


