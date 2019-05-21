#include "imageview.h"

/* ImageView
  ===============================================================================*/

ImageView::ImageView(QWidget* parent)
  : QListView(parent)
{
  m_model = new ImageListModel(this);
  setModel(m_model);
  setFlow(QListView::TopToBottom);
  setDragEnabled(true);
  setDragDropOverwriteMode(false);
  setDragDropMode(QListView::InternalMove);
  setDefaultDropAction(Qt::MoveAction);
  //  setItemDelegate(new ImageDelegate(this));
}

void ImageView::setCover(const QImage& image)
{
  m_model->setCover(image);
}

void ImageView::addThumbnail(const QImage& image, bool has_text)
{
  m_model->addThumbnail(image, has_text);
}

void ImageView::insertThumbnail(int row, const QImage& image, bool has_text)
{
  m_model->insertThumbnail(row, image, has_text);
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
{}

void ImageView::setHasText(int index, bool has_text)
{
  m_model->setHasText(index, has_text);
}

/* ImageListModel
  ===============================================================================*/

ImageListModel::ImageListModel(QObject* parent)
  : QAbstractListModel(parent)
{
  m_headers << tr("Pages");
  addThumbnail(QImage(), false); // this will be the cover
}

void ImageListModel::setCover(const QImage& image)
{
  m_images.replace(0, image);
}

bool ImageListModel::addThumbnail(const QImage& image, bool has_text)
{
  int row = rowCount();

  if (insertRows(row, 1)) {
    m_images.replace(row, image);
    m_has_text.replace(row, has_text);
    return true;
  }

  return false;
}

bool ImageListModel::insertThumbnail(int row, const QImage& image, bool has_text)
{
  if (row > 0) {
    if (insertRows(row, 1)) {
      m_images.replace(row, image);
      m_has_text.replace(row, has_text);
      return true;
    }
  }

  return false;
}

bool ImageListModel::removeThumbnail(int row)
{
  if (row > 0) {
    return removeRows(row, 1);
  }

  return false;
}

void ImageListModel::replaceThumbnail(int row, const QImage& image, bool has_text)
{
  if (row > 0) {
    m_images.replace(row, image);
    m_has_text.replace(row, has_text);
  }
}

void ImageListModel::setHasText(int index, bool has_text)
{
  m_has_text.replace(index, has_text);
}

bool ImageListModel::hasText(int row)
{
  return m_has_text.at(row);
}

bool ImageListModel::moveThumbnail(int source, int destination)
{
  if (source == 0 || destination == 0) {
    return false;
  }

  return moveRows(QModelIndex(), source, 1, QModelIndex(), destination);
}

bool ImageListModel::removeRows(int row, int count, const QModelIndex& parent)
{
  if (parent.isValid() || row == 0) {
    return false;
  }

  if (row >= m_images.size() || row + count <= 0) {
    return false;
  }

  beginRemoveRows(parent, row, row + count - 1);

  for (int r = 0; r < count; r++) {
    m_images.removeAt(r);
    m_has_text.removeAt(r);
  }

  endRemoveRows();
  return true;
}

bool ImageListModel::moveRows(const QModelIndex& /*sourceParent*/,
                              int source,
                              int count,
                              const QModelIndex& /*destinationParent*/,
                              int destination)
{
  if (source == destination) {
    return false;
  }

  if (destination > source && destination < source + count) {
    return false;
  }

  beginMoveRows(
    QModelIndex(), source, source + count - 1, QModelIndex(), destination);

  if (source < destination) {
    for (int s = 0; s < source + count - 1; s++) {
      QImage src_image = m_images.takeAt(source);
      bool src_has_text = m_has_text.takeAt(source);
      m_images.insert(destination - 1, src_image);
      m_has_text.insert(destination - 1, src_has_text);
    }

  } else {
    for (int s = 0; s < source + count - 1; s++) {
      QImage src_image = m_images.takeAt(source + s);
      bool src_has_text = m_has_text.takeAt(source + s);
      m_images.insert(destination + s, src_image);
      m_has_text.insert(destination + s, src_has_text);
    }
  }

  endMoveRows();
}

bool ImageListModel::insertRows(int row, int count, const QModelIndex& parent)
{
  if (parent.isValid()) {
    return false;
  }

  if (row < 0 || row > m_images.size() || count <= 0) {
    return false;
  }

  beginInsertRows(QModelIndex(), row, row + count - 1);

  for (int index = row, end = row + count; index < end; ++index) {
    m_images.insert(index, QImage());
    m_has_text.insert(index, false);
  }

  endInsertRows();
  return true;
}

int ImageListModel::rowCount(const QModelIndex& /*parent*/) const
{
  return m_images.size();
}

int ImageListModel::columnCount(const QModelIndex& /*parent*/) const
{
  return 1; // the second column doesn't display.
}

QVariant ImageListModel::data(const QModelIndex& index, int role) const
{
  if (index.isValid()) {
    if (role == Qt::DecorationRole) {
      switch (index.column()) {
      case 0:
        return QIcon(QPixmap::fromImage(m_images.at(index.row())));

      case 1:
        return m_has_text.at(index.row());
      }
    }
  }

  return QVariant();
}

Qt::DropActions ImageListModel::supportedDropActions() const
{
  return Qt::MoveAction;
}

Qt::DropActions ImageListModel::supportedDragActions() const
{
  return Qt::MoveAction;
}

Qt::ItemFlags ImageListModel::flags(const QModelIndex& index) const
{
  if (index.isValid() && index.model() == this && index.row() > 0) {
    return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
  }

  return Qt::NoItemFlags;
}

/* ImageDelegate
  ===============================================================================*/
ImageDelegate::ImageDelegate(QObject* parent)
  : QStyledItemDelegate(parent)
{}

void ImageDelegate::paint(QPainter* painter,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const
{
  if (index.data().canConvert<QImage>()) {
    QImage image = qvariant_cast<QImage>(index.data());
    QModelIndex i = index.model()->index(index.row(), 1, index);
    bool has_text = index.model()->data(i, Qt::DisplayRole).toBool();

    if (option.state & QStyle::State_Selected) {
      painter->fillRect(option.rect, option.palette.highlight());
    }

    QRect rect = option.rect;
    QPen pen;
    QBrush brush;

    if (has_text) {
      pen.setColor(QColor("green"));
      brush.setColor(QColor("green"));

    } else {
      pen.setColor(QColor("red"));
      brush.setColor(QColor("red"));
    }

    pen.setWidth(4);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(rect);
    painter->drawImage(rect, image);

  } else {
    QStyledItemDelegate::paint(painter, option, index);
  }
}
