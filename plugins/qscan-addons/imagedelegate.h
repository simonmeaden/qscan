#ifndef IMAGEDELEGATE_H
#define IMAGEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QPen>
#include <QImage>

#include "scanoptions.h"

namespace QScanner {

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

} // end of namespace QScanner

#endif // IMAGEDELEGATE_H
