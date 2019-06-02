#include "imageview.h"

/* ImageView
  ===============================================================================*/

ImageView::ImageView(QWidget* parent)
  : QListView(parent)
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

  //  setStyle(new DropIndicatorProxyStyle);
}

void ImageView::setCover(const QImage& image)
{
  m_model->setCover(image);
}

int ImageView::appendThumbnail(const QImage &image, bool has_text) {
  // TODO
  return m_model->appendThumbnail(image, has_text);
}

void ImageView::insertThumbnail(int row, const QImage& image, bool has_text, bool is_internal_image)
{
  // TODO
  m_model->insertThumbnail(row, image, has_text, is_internal_image);
}

void ImageView::removeThumbnail(int row)
{
  m_model->removeThumbnail(row);
}

void ImageView::moveThumbnail(int source, int destination)
{
  m_model->moveThumbnail(source, destination);
}

void ImageView::replaceThumbnail(int row, const QImage& image, bool has_text)
{
  // TODO
}

void ImageView::setHasText(int index, bool has_text)
{
  m_model->setHasText(index, has_text);
}

void ImageView::setInternalImage(int index, bool internal_image)
{
  m_model->setInternalImage(index, internal_image);
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
