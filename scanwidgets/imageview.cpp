#include "imageview.h"

/* ImageView
  ===============================================================================*/

ImageView::ImageView(QWidget *parent)
    : QListView(parent)
    , m_drag_state(DragStopped)
    , m_drag_start_pos(-1)
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

  m_scroll_bar = verticalScrollBar();

  //  setStyle(new DropIndicatorProxyStyle);
}

void ImageView::setCover(const QImage &image)
{
  m_model->setCover(image);
}

int ImageView::appendThumbnail(const QImage &image, bool has_text, bool is_internal_image)
{
    return m_model->appendThumbnail(image, has_text, is_internal_image);
}

void ImageView::insertThumbnail(int row, const QImage &image, bool has_text, bool is_internal_image)
{
  m_model->insertThumbnail(row, image, has_text, is_internal_image);
}

void ImageView::removeThumbnail(int row)
{
  m_model->removeThumbnail(row);
}

bool ImageView::moveThumbnail(int source, int destination)
{
    return m_model->moveThumbnail(source, destination);
}

void ImageView::replaceThumbnail(int row, const QImage &image, bool has_text, bool is_internal_image)
{
    m_model->replaceThumbnail(row, image, has_text, is_internal_image);
}

void ImageView::setHasText(int index, bool has_text)
{
  m_model->setHasText(index, has_text);
}

void ImageView::setInternalImage(int index, bool internal_image)
{
    m_model->setInternalImage(index, internal_image);
}

void ImageView::setCurrentRow(int row)
{
    QModelIndex index = m_model->index(row, 0, QModelIndex());
    m_selection_model->clearSelection();
    m_selection_model->setCurrentIndex(index, QItemSelectionModel::Select);
}

void ImageView::setSelectionModel(QItemSelectionModel *selectionModel)
{
    m_selection_model = selectionModel;
    QListView::setSelectionModel(selectionModel);
}

void ImageView::dropEvent(QDropEvent *event)
{
    // This is needed because of a bug in QListView which removes the
    // destination image when moved.
    if (event->dropAction() == Qt::MoveAction) {
        event->setDropAction(Qt::CopyAction);
        QListView::dropEvent(event);
    }
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_drag_state = DragStarted;
        m_drag_start_pos = event->pos().y();

    } else {
        QListView::mousePressEvent(event);
    }
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_drag_state) {
        m_drag_state = DragStopped;
        m_drag_start_pos = -1;
        return;
    }

    QListView::mouseReleaseEvent(event);
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_drag_state != DragStopped) {
        const int itemSize = sizeHintForRow(0) / 2;
        const int distance = qAbs(m_drag_start_pos - event->pos().y());

        if (distance > 10) {
            m_drag_state = Dragged;
        }

        if (distance > itemSize) {
            int stepCount = (distance / itemSize);

            if (m_drag_start_pos < event->pos().y()) {
                stepCount = -stepCount; // scrolling up
            }

            m_scroll_bar->setValue(m_scroll_bar->value() + (stepCount * m_scroll_bar->singleStep()));
            m_drag_start_pos = event->y();
        }

        return;
    }

    QListView::mouseMoveEvent(event);
}

bool ImageView::isIndexHidden(const QModelIndex &index) const
{
  int column = index.column();

  if (column > 0) {
    return true;
  }

  return QListView::isIndexHidden(index);
}

bool ImageView::event(QEvent *event)
{
  if (event->type() == QEvent::ToolTip) {
    auto* helpEvent = static_cast<QHelpEvent*>(event);
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
                                            const QStyleOption *option,
                                            QPainter *painter,
                                            const QWidget *widget) const
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
