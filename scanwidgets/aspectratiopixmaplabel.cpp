#include "aspectratiopixmaplabel.h"

AspectRatioPixmapLabel::AspectRatioPixmapLabel(QWidget* parent)
  : QLabel(parent)
{
  setMinimumSize(1, 1);
  setScaledContents(false);
}

void AspectRatioPixmapLabel::setPixmap(const QPixmap& p)
{
  pix = p;
  QLabel::setPixmap(scaledPixmap());
}

int AspectRatioPixmapLabel::heightForWidth(int width) const
{
  return pix.isNull() ? height() : ((qreal)pix.height() * width) / pix.width();
}

QSize AspectRatioPixmapLabel::sizeHint() const
{
  int w = width();
  return {w, heightForWidth(w)};
}

QPixmap AspectRatioPixmapLabel::scaledPixmap() const
{
  return pix.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void AspectRatioPixmapLabel::resizeEvent(QResizeEvent* /*e*/)
{
  if (!pix.isNull()) {
    QLabel::setPixmap(scaledPixmap());
  }
}
