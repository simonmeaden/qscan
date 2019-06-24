#include "imageeditdialog.h"
#include "ui_imageeditdialog.h"

ImageEditDialog::ImageEditDialog(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::ImageEditDialog)
{
  ui->setupUi(this);
}

ImageEditDialog::~ImageEditDialog()
{
  delete ui;
}

void ImageEditDialog::setImage(const QImage &image)
{
  ui->image_lbl->setPixmap(QPixmap::fromImage(image));
}

QImage ImageEditDialog::image()
{
  const QPixmap *pixmap = ui->image_lbl->pixmap();
  QImage image = pixmap->toImage();
  return image;
}
