#include <utility>

#include "ocrtools.h"
#include "ocrworker.h"

OcrTools::OcrTools(const QString& datapath,
                   const QString& lang, QObject* parent)
  : QObject(parent)
{

  m_logger = Log4Qt::Logger::logger(tr("OcrTools"));

  auto* thread = new QThread;
  m_ocr_worker = new OcrWorker(datapath, lang);

  // cleanup
  connect(thread, &QThread::started, m_ocr_worker, &OcrWorker::process);
  connect(this, &OcrTools::finished, thread, &QThread::quit);
  connect(thread, &QThread::finished, m_ocr_worker, &OcrWorker::deleteLater);
  connect(thread, &QThread::finished, thread, &QThread::deleteLater);

  // actual work.
  //  connect(this, &OcrTools::convertImage, m_ocr_worker, &OcrWorker::convertImage);
  //  connect(
  //    this, &OcrTools::startConverting, m_ocr_worker, &OcrWorker::convertImage);
  //  connect(m_ocr_worker, &OcrWorker::converted, this, &OcrTools::converted);
  connect(m_ocr_worker, &OcrWorker::converted, this, &OcrTools::converted);
  connect(m_ocr_worker, &OcrWorker::log, this, &OcrTools::log);

  m_ocr_worker->moveToThread(thread);
  thread->start();
}

OcrTools::~OcrTools()
{
  m_ocr_worker->stopRunning();
}

void OcrTools::convertImageToText(const Page& page)
{
  m_ocr_worker->convertImage(page);
}

void OcrTools::log(LogLevel level, const QString& msg)
{
  switch (level) {
  case TRACE:
    m_logger->trace(msg);
    break;

  case DEBUG:
    m_logger->debug(msg);
    break;

  case INFO:
    m_logger->info(msg);
    break;

  case WARN:
    m_logger->warn(msg);
    break;

  case ERROR:
    m_logger->error(msg);
    break;

  case FATAL:
    m_logger->fatal(msg);
    break;

  case OFF:
    break;
  }
}

//void OcrTools::converted(const Page& page)
//{
//  emit converted(page);
//}
