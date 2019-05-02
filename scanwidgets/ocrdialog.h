#ifndef OCRDIALOG_H
#define OCRDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QObject>
#include <QPlainTextEdit>
#include <QPushButton>

class OCRDialog : public QDialog
{
  Q_OBJECT
public:
  explicit OCRDialog(QWidget* parent = nullptr);

  QImage image();
  void setImage(const QImage& image);
  void receiveOcrText(const QString& text);
  QString text();

  bool imageChanged() const;

signals:
  void sendOcrImage(const QImage&);

protected:
  QPlainTextEdit* m_text_edit;
  QLabel* m_image_display;
  QImage m_image;
  bool m_image_changed;

  void initGui();
  void applyOcr();
  void applyTweaks();
  void help();
};

#endif // OCRDIALOG_H
