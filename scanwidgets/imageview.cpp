#include "imageview.h"

/* ImageView
  ===============================================================================*/

const QString ImageListModel::MIMETYPE = QStringLiteral("application/x-imagelistdata");

ImageView::ImageView(QWidget* parent)
  : QListView(parent)
{
  m_model = new ImageListModel(this);
  setModel(m_model);
  setFlow(QListView::TopToBottom);
  setItemDelegate(new ImageDelegate(this));
  setSelectionMode(QListView::SingleSelection);

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

void ImageView::appendThumbnail(const QImage& image, bool has_text)
{
  // TODO
  m_model->appendThumbnail(image, has_text);
}

void ImageView::insertThumbnail(int row, const QImage& image, bool has_text)
{
  // TODO
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
{
  // TODO
}

void ImageView::setHasText(int index, bool has_text)
{
  m_model->setHasText(index, has_text);
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

/* ImageListModel
  ===============================================================================*/

ImageListModel::ImageListModel(QObject* parent)
  : QAbstractListModel(parent)
{
  m_logger = Log4Qt::Logger::logger(tr("ImageListModel"));
  m_headers << tr("Pages");
  appendThumbnail(QImage(), false); // this will be the cover
}

void ImageListModel::setCover(const QImage& image)
{
  m_images.replace(0, image);
}

bool ImageListModel::appendThumbnail(const QImage& image, bool has_text, bool  internal_image)
{
  int row = rowCount();

  if (insertRows(row, 1)) {
    m_images.replace(row, image);
    m_has_text.replace(row, has_text);
    m_intsernal_image.replace(row, internal_image);
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

QStringList ImageListModel::mimeTypes() const
{
  QStringList types;
  types << MIMETYPE;
  return types;
}

bool ImageListModel::moveThumbnail(int source, int destination)
{
  return moveRows(index(source, 0), source, 1, index(destination, 0), destination);
}

bool ImageListModel::removeRows(int row, int count, const QModelIndex& parent)
{
  if (row == 0) {
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

bool ImageListModel::moveRows(const QModelIndex& sourceParent,
                              int source,
                              int count,
                              const QModelIndex& destinationParent,
                              int destination)
{
  if (source == -1) {
    source = sourceParent.row();
  }

  if (destination == -1) {
    destination = destinationParent.row();
  }

  // 0 is the cover so this cannot be moved.
  if (source == 0 || destination == 0) {
    return false;
  }

  if (source == destination) {
    return false;
  }

  if (destination > source && destination < source + count) {
    return false;
  }

  int dest_row = (source < destination ? destination + 1 : destination);
  bool good_move =
    beginMoveRows(sourceParent, source, source + count - 1, destinationParent, dest_row);

  if (good_move) {
    if (source < destination) {
      for (int s = 0; s < count; s++) {
        QImage src_image = m_images.takeAt(source + s);
        bool src_has_text = m_has_text.takeAt(source + s);
        m_images.insert(destination, src_image);
        m_has_text.insert(destination, src_has_text);
      }

    } else {
      for (int s = 0; s < count; s++) {
        QImage src_image = m_images.takeAt(source + s);
        bool src_has_text = m_has_text.takeAt(source + s);
        m_images.insert(destination + s, src_image);
        m_has_text.insert(destination + s, src_has_text);
      }
    }

  } else {
    return false;
  }

  endMoveRows();
  return true;
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
        return m_images.at(index.row());

      case 1:
        return m_has_text.at(index.row());
      }
    }
  }

  return QVariant();
}

bool ImageListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  QImage img = value.value<QImage>();

  if (index.isValid()) {
    if (role == Qt::DecorationRole) {
      switch (index.column()) {
      case 0:
        m_images.replace(index.row(), img);
        return true;

      case 1:
        m_has_text.replace(index.row(), false);
        return true;
      }
    }
  }

  return false;
}

Qt::DropActions ImageListModel::supportedDropActions() const
{
  return Qt::MoveAction;
}

Qt::DropActions ImageListModel::supportedDragActions() const
{
  return Qt::MoveAction;
}

QMimeData* ImageListModel::mimeData(const QModelIndexList& indexes) const
{
  auto* mime_data = new QMimeData();
  QByteArray encoded_data;

  for (const QModelIndex& index : indexes) {
    if (index.isValid()) {
      int row = index.row();
      QDataStream data_stream(&encoded_data, QIODevice::WriteOnly);
      data_stream << row << m_images.at(row) << m_has_text.at(row);
    }
  }

  mime_data->setData(MIMETYPE, encoded_data);
  return mime_data;
}

bool ImageListModel::dropMimeData(const QMimeData* data,
                                  Qt::DropAction action,
                                  int row,
                                  int,
                                  const QModelIndex& parent)
{
  // only move action is supported
  if (data->hasFormat(MIMETYPE) && action == Qt::MoveAction) {
    action = Qt::CopyAction;
    QByteArray encoded_data = data->data(MIMETYPE);
    QDataStream data_stream(&encoded_data, QIODevice::ReadOnly);

    QImage image;
    bool has_text;
    int source = 0, destination = 0;

    if (row == -1) {
      destination = parent.row();

    } else {
      destination = row;
    }

    while (!data_stream.atEnd()) {
      data_stream >> source >> image >> has_text;

      auto success = moveRows(parent, source, 1, parent, row);

      //      bool good_insert = insertRows(source, 0, parent);
      //      m_images.replace(source, image);
      //      m_has_text.replace(source, has_text);
      //            source++;

      //      endInsertRows();
    }

    return true;
  }

  return false;
}

bool ImageListModel::canDropMimeData(const QMimeData* data,
                                     Qt::DropAction,
                                     int row,
                                     int column,
                                     const QModelIndex& parent) const
{
  QStringList formats = data->formats();
  QString format;

  for (QString f : formats) {
    format += f + ", ";
  }

  m_logger->debug(tr("Row : %1, Col : %2 Formats : %3").arg(row, column).arg(format));

  return (data->hasFormat(MIMETYPE) && (column == 0 || (column == -1 && parent.column() == 0)));
}

Qt::ItemFlags ImageListModel::flags(const QModelIndex& index) const
{
  if (index.isValid() && index.model() == this && index.row() > 0) {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
  }

  return QAbstractListModel::flags(index);
  ;
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
  QVariant v = index.data(Qt::DecorationRole);

  if (v.canConvert<QImage>()) {
    painter->save();
    auto image = v.value<QImage>();
    QModelIndex i = index.model()->index(index.row(), 1, index);
    bool has_text = index.model()->data(i, Qt::DecorationRole).toBool();

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

    if (has_text) {
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
