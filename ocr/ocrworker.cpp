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

void OcrWorker::convertToString(const Page& page)
{
  m_images.append(page);
}

void OcrWorker::process()
{
  while (true) {
    if (!m_available) {
      if (!m_images.isEmpty()) {
        Page  page = m_images.takeFirst();
        QImage image = page->image();
        QString str = m_tesstools->makeBoxes(image, 0);
        page->setText(str);
        emit converted(page);
        m_available = false;
      }
    }

    QThread::msleep(1000);
  }
}
