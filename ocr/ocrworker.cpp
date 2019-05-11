#include <utility>

#include <opencv2/opencv.hpp>

#include "ocrworker.h"
#include "tesstools.h"

using namespace cv;

OcrWorker::OcrWorker(QString datapath, QString lang)
  : m_available(true)
  , m_running(true)
  , m_datapath(std::move(datapath))
  , m_lang(std::move(lang))
{
}

void OcrWorker::convertImage(const Page& page)
{
  emit log(LogLevel::INFO, (tr("Converting image in OcrWorker.")));
  m_images.append(page);
}

void OcrWorker::process()
{
  auto* m_api = new TessTools(m_datapath, m_lang, this);
  connect(m_api, &TessTools::log, this, &OcrWorker::log);

  while (m_running) {

    if (m_available) {
      if (!m_images.isEmpty()) {
        Page page = m_images.takeFirst();
        QString image_path = page->imagePath();
        m_api->getStringFromImage(page);
        emit converted(page);
        m_available = false;
      }
    }

    QThread::msleep(1000);
  }

  disconnect(m_api, &TessTools::log, this, &OcrWorker::log);
  delete m_api;
}

void OcrWorker::stopRunning()
{
  m_running = false;
}
