#include "ocrworker.h"

#include "tesstools.h"

OcrWorker::OcrWorker(const QString& datapath,
                     const QString& lang,
                     QObject* parent)
  : QObject(parent)
//  , m_tesstools(new TessTools(datapath, lang.this))
{
  m_tesstools = new TessTools(datapath, lang, this);
  connect(m_tesstools, &TessTools::log, this, &OcrWorker::log);
}

void
OcrWorker::convertToString(int page, const QImage& image)
{
  QString str = m_tesstools->makeBoxes(image, page);
  emit converted(page, str);
}
