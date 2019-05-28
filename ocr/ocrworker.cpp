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

void OcrWorker::convertPage(const DocumentData& page)
{
  emit log(LogLevel::INFO, (tr("Converting page in OcrWorker.")));
  m_pages.append(page);
}

void OcrWorker::convertImage(int page_no, const QImage& image)
{
  emit log(LogLevel::INFO, (tr("Converting image in OcrWorker.")));
  m_images.append(image);
  m_page_nos.append(page_no);
}

void OcrWorker::process()
{
  auto* m_api = new TessTools(m_datapath, m_lang, this);
  connect(m_api, &TessTools::log, this, &OcrWorker::log);

  while (m_running) {

    if (!m_images.isEmpty()) {
      QImage image = m_images.takeFirst();
      int page_no = m_page_nos.takeFirst();
      QString text = m_api->getStringFromImage(image);
      emit imageConverted(page_no, text);

    } else if (!m_pages.isEmpty()) {
      DocumentData page = m_pages.takeFirst();
      m_api->getStringFromPage(page);
      emit pageConverted(page);
    }

    //    }

    QThread::msleep(1000);
  }

  disconnect(m_api, &TessTools::log, this, &OcrWorker::log);
  delete m_api;
}

void OcrWorker::stopRunning()
{
  m_running = false;
}
