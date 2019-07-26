#include <utility>

#include <opencv2/opencv.hpp>

#include "ocrworker.h"
#include "tesstools.h"

using namespace cv;

OcrWorker::OcrWorker(QString datapath, QString lang)
  : m_datapath(std::move(datapath))
  , m_lang(std::move(lang))
  , m_available(true)
  , m_running(true)
{
}

void OcrWorker::convertPage(const DocumentData& doc_data)
{
  qCInfo(QscanOcr) << (tr("Converting page in OcrWorker."));
  m_doc_data.append(doc_data);
}

void OcrWorker::convertImage(int page_no, const QImage& image, const QRect& rect)
{
  qCInfo(QscanOcr) << (tr("Converting image in OcrWorker."));
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

      if (text.isEmpty()) {
        emit ocrFailed(page_no);

      } else {
        if (rect.isNull()) {
          emit imageConverted(page_no, Util::cleanText(text));

        } else {
          emit imageConvertedRect(page_no, Util::cleanText(text));
        }
      }

    } else if (!m_doc_data.isEmpty()) {
      DocumentData doc_data = m_doc_data.takeFirst();

      if (doc_data) {
        TessTools::getStringFromPage(m_datapath, m_lang, doc_data);
        emit pageConverted(doc_data);
      }
    }

    //    }

    QThread::msleep(1000);
  }
}

void OcrWorker::stopRunning()
{
  m_running = false;
}
