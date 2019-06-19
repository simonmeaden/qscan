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

void OcrWorker::convertImage(int page_no, const QImage &image, const QRect &rect)
{
  emit log(LogLevel::INFO, (tr("Converting image in OcrWorker.")));
  m_images.append(image);
  m_page_nos.append(page_no);
  m_rects.append(rect);
}

void OcrWorker::process()
{
  QString text;

  while (m_running) {

    if (!m_images.isEmpty()) {
      QImage image = m_images.takeFirst();
      int page_no = m_page_nos.takeFirst();
      QRect rect = m_rects.takeFirst();
      text = TessTools::getStringFromImage(m_datapath, m_lang, image, rect);

      if (rect.isNull()) {
        emit imageConverted(page_no, text);

      } else {
        emit imageConvertedRect(page_no, text);
      }

    } else if (!m_pages.isEmpty()) {
      DocumentData page = m_pages.takeFirst();
      TessTools::getStringFromPage(m_datapath, m_lang, page);
      emit pageConverted(page);
    }

    //    }

    QThread::msleep(1000);
  }
}

void OcrWorker::stopRunning()
{
  m_running = false;
}
