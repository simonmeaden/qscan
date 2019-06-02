#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QAbstractListModel>
#include <QListView>
#include <QVariant>
#include <QWidget>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPixmap>
#include <QMimeData>
#include <QDataStream>
#include <QProxyStyle>
#include <QToolTip>

#include "scanwidgets_global.h"
#include "logger.h"
#include "imagelistmodel.h"
#include "imagedelegate.h"


class DropIndicatorProxyStyle : public QProxyStyle
{
public:
  void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
                     const QWidget* widget) const override;
};

class SCANWIDGETSSHARED_EXPORT ImageView : public QListView
{
public:
  explicit ImageView(QWidget* parent = nullptr);

  bool event(QEvent* event) override;

  void setCover(const QImage& image);
  int appendThumbnail(const QImage &image, bool has_text = false);
  void insertThumbnail(int row, const QImage& image, bool has_text = false, bool is_internal_image = false);
  void removeThumbnail(int row);
  void moveThumbnail(int source, int destination);
  void replaceThumbnail(int row, const QImage& image, bool has_text);
  void setHasText(int index, bool has_text);
  void setInternalImage(int index, bool internal);

protected:
  ImageListModel* m_model;

  void dropEvent(QDropEvent* event) override;
  bool isIndexHidden(const QModelIndex& index) const override;

};

#endif // IMAGEVIEW_H
