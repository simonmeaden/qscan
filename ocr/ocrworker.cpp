#include "ocrworker.h"

#include "tesstools.h"

OcrWorker::OcrWorker(const QString& datapath,
                     const QString& lang,
                     QObject* parent)
  : QObject(parent)
  , m_available(true)
{
  m_tesstools = new TessTools(datapath, lang, this);
  connect(m_tesstools, &TessTools::log, this, &OcrWorker::log);
}

void
OcrWorker::convertToString(int page, const QImage& image)
{
  m_pages.append(page);
  m_images.append(image);
}

void
OcrWorker::process()
{
  while (true) {
    if (!m_available) {
      if (!m_images.isEmpty()) {
        int page = m_pages.takeFirst();
        QImage image = m_images.takeFirst();
        QString str = m_tesstools->makeBoxes(image, page);
        emit converted(page, str);
        m_available = false;
      }
    }
    thread()->msleep(200);
  }
}
