#include "imagelistmodel.h"

namespace QScanner {

const QString ImageListModel::MIMETYPE = QStringLiteral("application/x-imagelistdata");

ImageListModel::ImageListModel(QObject* parent)
  : QAbstractListModel(parent)
{
  m_headers << tr("Pages");
  appendThumbnail(QImage());  // this will be used for the cover
}

void
ImageListModel::setCover(const QImage& image)
{
  m_images.replace(0, image);
  m_is_completed.replace(0, !image.isNull());
}

int
ImageListModel::appendThumbnail(const QImage& image)
{
  int row = rowCount();

  if (insertRows(row, 1)) {
    m_images.replace(row, image);
    m_is_completed.replace(row, false);
    //    m_internal_image.replace(row, internal_image);
    return row;
  }

  return -1;
}

int
ImageListModel::appendCompleted(const bool completed)
{
  int row = rowCount();

  if (insertRows(row, 1)) {
    m_images.replace(row, QImage());
    m_is_completed.replace(row, completed);
    return row;
  }

  return -1;
}

bool
ImageListModel::insertThumbnail(int row, const QImage& image)
{
  if (row > 0) {
    if (insertRows(row, 1)) {
      m_images.replace(row, image);
      m_is_completed.replace(row, false);
      //      m_internal_image.replace(row, is_internal_image);
      return true;
    }
  }

  return false;
}

bool
ImageListModel::removeThumbnail(int row)
{
  if (row > 0) {
    return removeRows(row, 1);
  }

  return false;
}

void
ImageListModel::replaceThumbnail(int row, const QImage& image)
{
  if (row > 0) {
    m_images.replace(row, image);
    m_is_completed.replace(row, false);
    //    m_internal_image.replace(row, is_internal_image);
  }
}

QImage
ImageListModel::thumbnail(int row)
{
  if (row > 0 && row < m_images.size()) {
    QImage image = m_images.at(row);
    return image;
  }

  return QImage();
}

void
ImageListModel::setCompleted(int row, bool has_text)
{
  if (row >= 0 && row < m_is_completed.size()) {
    m_is_completed.replace(row, has_text);
  }
}

bool
ImageListModel::isCompleted(int row)
{
  return m_is_completed.at(row);
}

bool
ImageListModel::isEmpty()
{
  return m_images.isEmpty();
}

// bool ImageListModel::isInternalImage(int row)
//{
//  return m_internal_image.at(row);
//}

// void ImageListModel::setInternalImage(int row, bool value)
//{
//  if (row >= 0 && row < m_internal_image.size()) {
//    m_internal_image.replace(row, value);
//  }
//}

QString
ImageListModel::tooltip(int row)
{
  QString text;

  if (row == 0) {
    text = tr("Page 0 : Cover image");

  } /* else if (isInternalImage(row)) {

    text = tr("Page %1 : Internal non-OCR image").arg(row);

  }*/
  else if (isCompleted(row)) {
    text = tr("Page %1 : Text exists for this image").arg(row);

  }

  else {
    text = tr("Page %1 : No text exists for this image").arg(row);
  }

  return text;
}

QStringList
ImageListModel::mimeTypes() const
{
  QStringList types;
  types << MIMETYPE;
  return types;
}

bool
ImageListModel::moveThumbnail(int source, int destination)
{
  return moveRows(index(source, 0), source, 1, index(destination, 0), destination);
}

bool
ImageListModel::removeRows(int row, int count, const QModelIndex& parent)
{
  if (row == 0) {
    return false;
  }

  if (row >= m_images.size() || row + count <= 0) {
    return false;
  }

  beginRemoveRows(parent, row, row + count - 1);

  for (int r = row; r < row + count; r++) {
    m_images.removeAt(r);
    m_is_completed.removeAt(r);
    //    m_internal_image.removeAt(r);
  }

  endRemoveRows();
  return true;
}

bool
ImageListModel::moveRows(
  const QModelIndex& sourceParent, int source, int count, const QModelIndex& destinationParent, int destination)
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
  bool good_move = beginMoveRows(sourceParent, source, source + count - 1, destinationParent, dest_row);

  if (good_move) {
    if (source < destination) {
      for (int s = 0; s < count; s++) {
        QImage src_image = m_images.takeAt(source + s);
        bool src_is_completed = m_is_completed.takeAt(source + s);
        m_images.insert(destination, src_image);
        m_is_completed.insert(destination, src_is_completed);
      }

    } else {
      for (int s = 0; s < count; s++) {
        QImage src_image = m_images.takeAt(source + s);
        bool src_is_completed = m_is_completed.takeAt(source + s);
        m_images.insert(destination + s, src_image);
        m_is_completed.insert(destination + s, src_is_completed);
      }
    }

  } else {
    return false;
  }

  endMoveRows();
  return true;
}

bool
ImageListModel::insertRows(int row, int count, const QModelIndex& parent)
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
    m_is_completed.insert(index, false);
    //    m_internal_image.insert(index, false);
  }

  endInsertRows();
  return true;
}

void
ImageListModel::setFont(const QFont& font)
{
  m_font = font;
}

int
ImageListModel::rowCount(const QModelIndex& /*parent*/) const
{
  return m_images.size();
}

int
ImageListModel::columnCount(const QModelIndex& /*parent*/) const
{
  return 3;  // the second column doesn't display.
}

QVariant
ImageListModel::data(const QModelIndex& index, int role) const
{
  if (index.isValid()) {

    switch (role) {
    case Qt::DecorationRole:
      switch (index.column()) {
      case 0:
        if (index.row() > 0 && m_is_completed.at(index.row())) {
          QString text = tr("Page %1 : OCR completed").arg(index.row());
          return text;
        }

        return m_images.at(index.row());

      case 1:
        return m_is_completed.at(index.row());
      }

      break;
    }
  }

  return QVariant();
}

bool
ImageListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  QImage img = value.value<QImage>();

  if (index.isValid()) {
    if (role == Qt::DecorationRole) {
      switch (index.column()) {
      case 0:
        m_images.replace(index.row(), img);
        return true;

      case 1:
        m_is_completed.replace(index.row(), false);
        return true;
      }
    }
  }

  return false;
}

Qt::DropActions
ImageListModel::supportedDropActions() const
{
  return Qt::MoveAction;
}

Qt::DropActions
ImageListModel::supportedDragActions() const
{
  return Qt::MoveAction;
}

QMimeData*
ImageListModel::mimeData(const QModelIndexList& indexes) const
{
  auto* mime_data = new QMimeData();
  QByteArray encoded_data;

  for (const QModelIndex& index : indexes) {
    if (index.isValid()) {
      int row = index.row();
      QDataStream data_stream(&encoded_data, QIODevice::WriteOnly);
      data_stream << row << m_images.at(row) << m_is_completed.at(row);
    }
  }

  mime_data->setData(MIMETYPE, encoded_data);
  return mime_data;
}

bool
ImageListModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int, const QModelIndex& parent)
{
  // only move action is supported
  if (data->hasFormat(MIMETYPE) && action == Qt::MoveAction) {
    action = Qt::CopyAction;
    QByteArray encoded_data = data->data(MIMETYPE);
    QDataStream data_stream(&encoded_data, QIODevice::ReadOnly);

    QImage image;
    bool has_text;
    int source = 0 /*, destination = 0*/;

    //    if (row == -1) {
    //      destination = parent.row();

    //    } else {
    //      destination = row;
    //    }

    while (!data_stream.atEnd()) {
      data_stream >> source >> image >> has_text;

      /*auto success =*/moveRows(parent, source, 1, parent, row);
    }

    return true;
  }

  return false;
}

bool
ImageListModel::canDropMimeData(
  const QMimeData* data, Qt::DropAction, int row, int column, const QModelIndex& parent) const
{
  QStringList formats = data->formats();
  QString format;

  for (const QString& f : formats) {
    format += f + ", ";
  }

  qCDebug(LogQScanWidgets) << tr("Row : %1, Col : %2 Formats : %3").arg(row, column).arg(format);

  return (data->hasFormat(MIMETYPE) && (column == 0 || (column == -1 && parent.column() == 0)));
}

Qt::ItemFlags
ImageListModel::flags(const QModelIndex& index) const
{
  if (index.isValid() && index.model() == this && index.row() > 0) {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
  }

  return QAbstractListModel::flags(index);
}

} // end of namespace QScanner
