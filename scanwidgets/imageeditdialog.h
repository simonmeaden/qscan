#ifndef IMAGEEDITDIALOG_H
#define IMAGEEDITDIALOG_H

#include <QDialog>
#include <QImage>
#include <QPixmap>

namespace Ui {
class ImageEditDialog;
}

class ImageEditDialog : public QDialog
{
  Q_OBJECT

  public:
  explicit ImageEditDialog(QWidget *parent = nullptr);
  ~ImageEditDialog() override;

  void setImage(const QImage &image);
  QImage image();

  private:
  Ui::ImageEditDialog *ui;
};

#endif // IMAGEEDITDIALOG_H
