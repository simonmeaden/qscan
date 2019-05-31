#include "imagedelegate.h"

ImageDelegate::ImageDelegate(QObject* parent)
  : QStyledItemDelegate(parent)
{}

ImageDelegate::~ImageDelegate() {}

void ImageDelegate::paint(QPainter* painter,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const
{
  QVariant v = index.data(Qt::DecorationRole);

  if (v.canConvert<QImage>()) {
    painter->save();
    auto image = v.value<QImage>();
    QModelIndex i = index.model()->index(index.row(), 1, index);
    bool has_text = index.model()->data(i, Qt::DecorationRole).toBool();
    i = index.model()->index(index.row(), 2, index);
    bool is_internal = index.model()->data(i, Qt::DecorationRole).toBool();

    if (option.state & QStyle::State_Selected) {
      painter->fillRect(option.rect, option.palette.highlight());
    }

    QRect image_rect;
    QRect border_rect;

    border_rect.setX(option.rect.x() + 5);
    border_rect.setY(option.rect.y() + 5);
    border_rect.setWidth(image.rect().width() + 10);
    border_rect.setHeight(image.rect().height() + 6);

    image_rect.setX(option.rect.x() + 10);
    image_rect.setY(option.rect.y() + 10);
    image_rect.setWidth(image.rect().width());
    image_rect.setHeight(image.rect().height());

    QPen pen;

    if (is_internal) {
      pen.setColor(QColor("yellow"));

    } else if (has_text) {
      pen.setColor(QColor("green"));

    } else {
      pen.setColor(QColor("red"));
    }

    pen.setWidth(4);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->drawImage(image_rect, image);
    painter->drawRect(border_rect);
    painter->restore();

  } else {
    QStyledItemDelegate::paint(painter, option, index);
  }
}

QSize ImageDelegate::sizeHint(const QStyleOptionViewItem& /*option*/, const QModelIndex& index) const
{
  QVariant v = index.data(Qt::DecorationRole);
  auto image = v.value<QImage>();
  auto size = image.size();
  size += QSize(10, 20);
  return size;
}
