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
  int appendThumbnail(const QImage &image, bool has_text, bool is_internal_image);
  void insertThumbnail(int row, const QImage &image, bool has_text, bool is_internal_image);
  void removeThumbnail(int row);
  bool moveThumbnail(int source, int destination);
  void replaceThumbnail(int row, const QImage &image, bool has_text, bool is_internal_image);
  void setHasText(int index, bool has_text);
  void setInternalImage(int index, bool internal);

  void setCurrentRow(int row);

  void setSelectionModel(QItemSelectionModel *selectionModel) override;

  protected:
  enum DragState { DragStopped, DragStarted, Dragged };

  ImageListModel* m_model;
  QItemSelectionModel *m_selection_model;
  QScrollBar *m_scroll_bar;
  quint8 m_drag_state;
  int m_drag_start_pos;

  bool isIndexHidden(const QModelIndex &index) const override;

  void dropEvent(QDropEvent* event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // IMAGEVIEW_H
