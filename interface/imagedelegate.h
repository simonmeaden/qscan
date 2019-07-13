#ifndef IMAGEDELEGATE_H
#define IMAGEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QPen>
#include <QImage>

class ImageDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  explicit ImageDelegate(QObject* parent = nullptr);
  ~ImageDelegate() override;;

public:
  void paint(QPainter* painter,
             const QStyleOptionViewItem& option,
             const QModelIndex& index) const override;
  QSize sizeHint(const QStyleOptionViewItem& option,
                 const QModelIndex& index) const override;
};

#endif // IMAGEDELEGATE_H
