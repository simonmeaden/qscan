#ifndef TEXTEDITIODEVICE_H
#define TEXTEDITIODEVICE_H

#include <QIODevice>
#include <QObject>
#include <QPlainTextEdit>

class TextEditIoDevice : public QIODevice
{
  Q_OBJECT

public:
  TextEditIoDevice(QPlainTextEdit* text_edit, QObject* parent = nullptr);

  void setTextEdit(QPlainTextEdit* m_text_edit);

protected:
  qint64 readData(char*, qint64) override;
  qint64 writeData(const char* data, qint64 maxSize) override;

private:
  QPlainTextEdit* m_text_edit;
  QString m_pre_data;
};

#endif // TEXTEDITIODEVICE_H
