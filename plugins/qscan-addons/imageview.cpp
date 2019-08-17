#include "imageview.h"

namespace QScanner {

/* ImageView
  ===============================================================================*/

ImageView::ImageView(QWidget* parent)
  : QListView(parent)
  , m_drag_state(DragStopped)
  , m_drag_start_x(-1)
{
  m_model = new ImageListModel(this);
  setModel(m_model);
  setFlow(QListView::TopToBottom);
  setItemDelegate(new ImageDelegate(this));
  setSelectionMode(QListView::SingleSelection);
  setMouseTracking(true);

  setDragEnabled(true);
  setAcceptDrops(true);
  setDragDropMode(QListView::InternalMove);
  setDropIndicatorShown(true);
  setDragDropOverwriteMode(false);
  setDefaultDropAction(Qt::MoveAction);

  m_model->setFont(font());
}

void ImageView::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    m_drag_state = DragStarted;
    m_drag_start_x = int(event->localPos().x());

  } /*else {*/

  QListView::mousePressEvent(event);
  //  }
}

void ImageView::mouseReleaseEvent(QMouseEvent* event)
{
  if (m_drag_state) {
    m_drag_state = DragStopped;
    m_drag_start_x = 0;
    return;
  }

  QListView::mouseReleaseEvent(event);
}

void ImageView::mouseMoveEvent(QMouseEvent* event)
{
  QPoint pos = event->localPos().toPoint();

  if (m_drag_state != DragStopped) {
    const int mouse_position = int(pos.x());
    const int mouse_distance = qAbs(m_drag_start_x - mouse_position);
    DragDirection drag_direction = (mouse_distance < 10
                                    ? Drag_Up
                                    : (mouse_distance > 10 ? Drag_Down : Drag_None));

    if (drag_direction != Drag_None) { // mouse has moved significantly
      QScrollBar* scroll_bar = verticalScrollBar();
      QWidget* viewport = qobject_cast<QAbstractScrollArea*>(this)->viewport();

      const int view_x = viewport->x();
      const int view_height = viewport->height();

      if (drag_direction == Drag_Up && (mouse_position < view_x && mouse_position > view_x + 10)) {
        scroll_bar->setValue(scroll_bar->value() - mouse_distance);
        pos.setX(view_x + 5); // don't let the mouse above the widget while dragging

      } else if (drag_direction == Drag_Down
                 && (mouse_position > view_x + view_height
                     && mouse_position > view_x + view_height - 10)) {
        scroll_bar->setValue(scroll_bar->value() + mouse_distance);
        pos.setX(view_x + view_height - 5); // don't let the mouse below the widget while dragging
      }

      event->setLocalPos(pos);

      m_drag_state = Dragged;
    }
  }

  QListView::mouseMoveEvent(event);
}

void ImageView::setCover(const QImage& image)
{
  m_model->setCover(image);
}

int ImageView::appendThumbnail(const QImage& image)
{
  return m_model->appendThumbnail(image);
}

int ImageView::appendCompleted(const bool completed)
{
  return m_model->appendCompleted(completed);
}

void ImageView::insertThumbnail(int row, const QImage& image)
{
  m_model->insertThumbnail(row, image);
}

QImage ImageView::removeThumbnail(int row)
{
  QImage image = m_model->thumbnail(row);
  m_model->removeThumbnail(row);
  return image;
}

bool ImageView::moveThumbnail(int source, int destination)
{
  return m_model->moveThumbnail(source, destination);
}

void ImageView::replaceThumbnail(int row, const QImage& image)
{
  m_model->replaceThumbnail(row, image);
}

QImage ImageView::thumbnail(int row)
{
  return m_model->thumbnail(row);
}

void ImageView::setCurrentRow(int row)
{
  QModelIndex index = m_model->index(row, 0, QModelIndex());
  m_selection_model->clearSelection();
  m_selection_model->setCurrentIndex(index, QItemSelectionModel::Select);
}

void ImageView::setFont(const QFont& font)
{
  m_model->setFont(font);
  QListView::setFont(font);
}

void ImageView::setSelectionModel(QItemSelectionModel* selectionModel)
{
  m_selection_model = selectionModel;
  QListView::setSelectionModel(selectionModel);
}

void ImageView::dropEvent(QDropEvent* event)
{
  // This is needed because of a bug in QListView which removes the
  // destination image when moved.
  if (event->dropAction() == Qt::MoveAction) {
    event->setDropAction(Qt::CopyAction);
    QListView::dropEvent(event);
  }
}

bool ImageView::isIndexHidden(const QModelIndex& index) const
{
  int column = index.column();

  if (column > 0) {
    return true;
  }

  return QListView::isIndexHidden(index);
}

bool ImageView::event(QEvent* event)
{
  if (event->type() == QEvent::ToolTip) {
    auto* helpEvent = dynamic_cast<QHelpEvent*>(event);
    QModelIndex index = indexAt(helpEvent->pos());

    if (index.isValid()) {
      QToolTip::showText(helpEvent->globalPos(), m_model->tooltip(index.row()));

    } else {
      QToolTip::hideText();
      event->ignore();
    }

    return true;
  }

  return QListView::event(event);
}

/* DropIndicatorProxyStyle
  ===============================================================================*/
void DropIndicatorProxyStyle::drawPrimitive(QStyle::PrimitiveElement element,
    const QStyleOption* option,
    QPainter* painter,
    const QWidget* widget) const
{
  if (element == QStyle::PE_IndicatorItemViewItemDrop) {
    painter->setRenderHint(QPainter::Antialiasing, true);

    QColor c("blue");
    QPen pen(c);
    pen.setWidth(3);

    painter->setPen(pen);
    painter->drawRect(option->rect);

  } else {
    QProxyStyle::drawPrimitive(element, option, painter, widget);
  }
}

} // end of namespace QScanner
