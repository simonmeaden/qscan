#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QAbstractListModel>
#include <QBrush>
#include <QDataStream>
#include <QListView>
#include <QMimeData>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QProxyStyle>
#include <QScrollBar>
#include <QStyledItemDelegate>
#include <QToolTip>
#include <QVariant>
#include <QWidget>
#include <QMouseEvent>

#include "interface_global.h"
#include "imagedelegate.h"
#include "imagelistmodel.h"

class DropIndicatorProxyStyle : public QProxyStyle
{
public:
  void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
                     const QWidget* widget) const override;
};

class INTERFACESHARED_EXPORT ImageView : public QListView
{
public:
  explicit ImageView(QWidget* parent = nullptr);

  bool event(QEvent* event) override;

  void setCover(const QImage& image);
  int appendThumbnail(const QImage& image);
  int appendCompleted(const bool completed);
  void insertThumbnail(int row, const QImage& image);
  QImage removeThumbnail(int row);
  bool moveThumbnail(int source, int destination);
  void replaceThumbnail(int row, const QImage& image);
  QImage thumbnail(int row);

  void setCurrentRow(int row);

  void  setFont(const QFont& font);

  void setSelectionModel(QItemSelectionModel* selectionModel) override;

protected:
  enum DragState
  {
    DragStopped,
    DragStarted,
    Dragged,
  };
  enum DragDirection
  {
    Drag_Up,
    Drag_Down,
    Drag_None,
  };

  ImageListModel* m_model;
  QItemSelectionModel* m_selection_model{};
  DragState m_drag_state;
  int m_drag_start_x;

  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;

  bool isIndexHidden(const QModelIndex& index) const override;

  void dropEvent(QDropEvent* event) override;

  friend class PageView;
};

#endif // IMAGEVIEW_H
