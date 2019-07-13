#include "imageeditdialog.h"

ImageEditDialog::ImageEditDialog(QWidget* parent)
  : QDialog(parent)
{
}

ImageEditDialog::~ImageEditDialog()
{
}

void ImageEditDialog::setImage(const QImage& image)
{
}

QImage ImageEditDialog::image()
{
  //  const QPixmap *pixmap = ui->image_lbl->pixmap();
  //  QImage image = pixmap->toImage();
  //  return image;
}
