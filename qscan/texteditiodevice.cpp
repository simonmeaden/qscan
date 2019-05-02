#include "texteditiodevice.h"

/*TextEditIoDevice
  =============================================================================*/
TextEditIoDevice::TextEditIoDevice(QPlainTextEdit* text_edit, QObject* parent)
  : QIODevice(parent)
  , m_text_edit(text_edit)
{
  open(QIODevice::WriteOnly | QIODevice::Text);
}

void
TextEditIoDevice::setTextEdit(QPlainTextEdit* text_edit)
{
  m_text_edit = text_edit;

  if (!m_pre_data.isEmpty()) {
    m_text_edit->setPlainText(m_pre_data);
    m_pre_data.clear();
  }
}

qint64
TextEditIoDevice::readData(char*, qint64)
{
  return 0;
}

qint64
TextEditIoDevice::writeData(const char* data, qint64 maxSize)
{
  if (m_text_edit) {
    QString d(data);
    d = d.trimmed();

    if (d.endsWith("\n")) {
      d = d.left(d.length() - 1);
    }

    m_text_edit->appendPlainText(d);

  } else {
    m_pre_data = QString(data);
  }

  return maxSize;
}
