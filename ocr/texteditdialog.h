#ifndef TEXTEDITDIALOG_H
#define TEXTEDITDIALOG_H

#include <QDialog>

namespace Ui {
class TextEditDialog;
}

class TextEditDialog : public QDialog
{
  Q_OBJECT

  public:
  explicit TextEditDialog(QWidget *parent = nullptr);
  ~TextEditDialog() override;

  void setText(const QString &text);
  QString text();

  private:
  Ui::TextEditDialog *ui;

  private slots:
  void accept();
  void reject();
};

#endif // TEXTEDITDIALOG_H
