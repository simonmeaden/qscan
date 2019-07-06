#include "imagedelegate.h"

ImageDelegate::ImageDelegate(QObject* parent)
  : QStyledItemDelegate(parent)
{}

ImageDelegate::~ImageDelegate() = default;

void ImageDelegate::paint(QPainter* painter,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const
{
  QVariant v = index.data(Qt::DecorationRole);
  QString text;
  QImage image;
  bool is_image = false, is_text = false;

  if (v.canConvert<QImage>()) {
    image = v.value<QImage>();
    is_image = true;

  } else if (v.canConvert<QString>()) {
    text = v.toString();
    is_text = true;
  }

  if (is_image || is_text) {
    painter->save();
    auto image = v.value<QImage>();
    QModelIndex i = index.model()->index(index.row(), 1, index);
    bool is_completed = index.model()->data(i, Qt::DecorationRole).toBool();

    if (option.state & QStyle::State_Selected) {
      painter->fillRect(option.rect, option.palette.highlight());
    }

    QRect subject_rect;
    QRect border_rect;

    QPen pen;

    if (is_completed) {
      pen.setColor(QColor("lime"));

    } else  {
      pen.setColor(QColor("blue"));

    }

    border_rect.setX(option.rect.x() + 5);
    border_rect.setY(option.rect.y() + 5);
    subject_rect.setX(option.rect.x() + 10);

    if (is_text) {
      QFontMetrics metrics(option.font);
      int width = metrics.width(text);
      int height = metrics.height();

      subject_rect.setY(option.rect.y() + 5 + height);

      border_rect.setHeight(height + 10);
      border_rect.setWidth(width + 10);

      subject_rect.setWidth(width);
      subject_rect.setHeight(height);

      pen.setWidth(4);
      pen.setJoinStyle(Qt::MiterJoin);
      painter->setPen(pen);
      painter->drawRect(border_rect);
      pen.setColor(QColor("black"));
      painter->setPen(pen);
      painter->drawText(subject_rect.topLeft(), text);

    } else {
      border_rect.setWidth(image.rect().width() + 10);
      border_rect.setHeight(image.rect().height() + 10);

      subject_rect.setY(option.rect.y() + 10);
      subject_rect.setWidth(image.rect().width());
      subject_rect.setHeight(image.rect().height());

      pen.setWidth(4);
      pen.setJoinStyle(Qt::MiterJoin);
      painter->setPen(pen);
      painter->drawImage(subject_rect, image);
      painter->drawRect(border_rect);

      //      QBrush overlay(QColor(255, 0, 0, 100));
      painter->setPen(QPen());
      //      painter->setBrush(overlay);
      painter->drawRect(subject_rect);
    }

    painter->restore();

  } else {
    QStyledItemDelegate::paint(painter, option, index);
  }
}

QSize ImageDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  QVariant v = index.data(Qt::DecorationRole);

  if (v.canConvert<QImage>()) {
    auto image = v.value<QImage>();
    auto size = image.size();
    size += QSize(10, 20);
    return size;

  } else if (v.canConvert<QString>()) {
    QString text = tr("Page %1 : OCR completed.").arg(index.row());
    QFontMetrics metrics(option.font);
    int height = metrics.height() + 20;
    int width = metrics.width(text) + 20;
    QSize size(width, height);
    return size;
  }

  return QStyledItemDelegate::sizeHint(option, index);
}
