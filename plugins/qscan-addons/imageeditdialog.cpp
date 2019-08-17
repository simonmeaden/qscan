#include "imageeditdialog.h"

namespace QScanner {

ImageEditDialog::ImageEditDialog(QWidget* parent)
  : QDialog(parent)
{
}

ImageEditDialog::~ImageEditDialog() {}

void
ImageEditDialog::setImage(const QImage& image)
{
  m_image = image;
}

QImage
ImageEditDialog::image()
{
  //  const QPixmap *pixmap = ui->image_lbl->pixmap();
  //  QImage image = pixmap->toImage();
  //  return image;
  return m_image;
}

} // end of namespace QScanner
