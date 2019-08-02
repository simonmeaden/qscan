#include "scanlist.h"

/* ScanEdit
 **************************************************************************************/
ScanList::ScanList(QWidget* parent)
  : QListView(parent)
  , m_changes(false)
{
  m_model = new ScanListModel(this);
  setModel(m_model);
  setSelectionMode(QAbstractItemView::SingleSelection);
  //  setEditTriggers(QAbstractItemView::DoubleClicked);
  //  setItemDelegate(new ScanItemDelegate(this));
}

void ScanList::appendText(const QString& text)
{
  appendText(StyledString(text));
}


void ScanList::appendText(const StyledString& text)
{
  int row = m_model->rowCount();

  if (m_model->insertRow(row)) {
    QModelIndex index = m_model->index(row, 0, QModelIndex());
    m_model->setData(index, QVariant::fromValue<StyledString>(text), Qt::EditRole);
    m_changes = true;
    emit dataHasChanged();
  }
}

int ScanList::appendImage(const QImage& image)
{
  int row = m_model->rowCount();

  if (m_model->insertRow(row)) {
    QModelIndex index = m_model->index(row, 0);
    m_model->setData(index, image, Qt::EditRole);
    m_changes = true;
    emit dataHasChanged();
    return row;
  }

  return -1;
}

void ScanList::replaceText(int row, const QString& text)
{
  replaceText(row, StyledString(text));
}

void ScanList::replaceText(int row, const StyledString& text)
{
  QModelIndex index = m_model->index(row, 0);

  if (index.isValid()) {
    m_model->setData(index, QVariant::fromValue<StyledString>(text));
    m_changes = true;
    emit dataHasChanged();
  }
}

void ScanList::replaceImage(int row, const QImage& image)
{
  QModelIndex index = m_model->index(row, 0);

  if (index.isValid()) {
    m_model->setData(index, image);
    m_changes = true;
    emit dataHasChanged();
  }
}

void ScanList::removeDataRow(const QModelIndex index)
{
  if (index.isValid()) {
    m_model->removeRows(index.row(), 1);
    m_changes = true;
    emit dataHasChanged();
  }
}

void ScanList::setData(const QMap<int, QVariant>& data)
{
  m_model->clearData();

  for (auto it = data.keyBegin(), end = data.keyEnd(); it != end; ++it) {
    int row = *it;

    if (m_model->insertRow(row)) {
      QModelIndex index = m_model->index(row, 0);
      m_model->setData(index, data.value(row), Qt::EditRole);
      m_changes = false;
    }
  }
}

void ScanList::setText(const QString& text)
{
  setText(StyledString(text));
}

void ScanList::setText(const StyledString& text)
{
  m_model->clearData();
  m_model->insertRow(0);
  const QModelIndex index = m_model->index(0, 0);
  m_model->setData(index, QVariant::fromValue<StyledString>(text), Qt::EditRole);
  m_changes = false;
  emit dataHasChanged();
}

void ScanList::setText(const StyledStringList& list)
{
  m_model->clearData();
  m_model->insertRows(0, list.size());

  for (int row = 0; row < list.size(); row++) {
    const QModelIndex index = m_model->index(row, 0);
    m_model->setData(index, QVariant::fromValue<StyledString>(list.at(row)), Qt::EditRole);
    m_changes = false;
    emit dataHasChanged();
  }
}

void ScanList::setText(const QStringList& list)
{
  m_model->clearData();
  m_model->insertRows(0, list.size());

  for (int row = 0; row < list.size(); row++) {
    const QModelIndex index = m_model->index(row, 0);
    m_model->setData(index, list.at(row), Qt::EditRole);
    m_changes = false;
    emit dataHasChanged();
  }
}

int ScanList::setImage(const QImage& image)
{
  m_model->clearData();
  m_model->insertRow(0);
  const QModelIndex index = m_model->index(0, 0);
  m_model->setData(index, image, Qt::EditRole);
  m_changes = false;
  emit dataHasChanged();
  return index.row();
}

void ScanList::dumpData()
{
  m_model->clearData();
  m_changes = false;
  emit dataHasChanged();
}

bool ScanList::hasDataChanges()
{
  return m_changes;
}

StyledStringList ScanList::texts()
{
  return m_model->textList();
}

QList<QImage> ScanList::images()
{
  return m_model->imageList();
}

int ScanList::size()
{
  return m_model->rowCount();
}

void ScanList::contextMenuEvent(QContextMenuEvent* /*event*/)
{
  //  auto *context_menu = new QMenu();
}

/* ScanListModel
 **************************************************************************************/
ScanListModel::ScanListModel(QObject* parent)
  : QAbstractListModel(parent)
  , m_current_image(-1)
{}

void ScanListModel::clearData()
{
  int rows = rowCount();

  if (rows > 0) {
    removeRows(0, rows);
  }

  m_strings.clear();
  m_images.clear();
  m_data_list.clear();
}

QModelIndex ScanListModel::index(int row, int column, const QModelIndex& /*parent*/) const
{
  return createIndex(row, column);
}

QModelIndex ScanListModel::parent(const QModelIndex& /*index*/) const
{
  return {};
}

StyledStringList ScanListModel::textList()
{
  StyledStringList list;

  for (auto key : m_strings) {
    list << m_data_list.at(key).value<StyledString>();
  }

  return list;
}

QList<QImage> ScanListModel::imageList()
{
  QList<QImage> list;

  for (auto key : m_images) {
    list << m_data_list.at(key).value<QImage>();
  }

  return list;
}

int ScanListModel::rowCount(const QModelIndex& /*parent*/) const
{
  return m_data_list.size();
}

int ScanListModel::columnCount(const QModelIndex& /*parent*/) const
{
  return 1;
}

QVariant ScanListModel::data(const QModelIndex& index, int role) const
{
  if (index.isValid()) {
    int row = index.row();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::DecorationRole:
      return m_data_list.at(row);
    }
  }

  return QVariant();
}

bool ScanListModel::setData(const QModelIndex& index, const QVariant& value, int /*role*/)
{
  if (index.isValid()) {
    int row = index.row();

    if (m_data_list.size() >= row) {
      m_data_list.replace(row, value);

    } else {
      m_data_list.insert(row, value);
    }

    if (value.type() == QVariant::String) {
      if (!m_strings.contains(row)) {
        if (m_images.contains(row)) {
          m_images.removeOne(row);
        }

        m_strings.append(row);
      } // else already a string

    } else if (value.type() == QVariant::Image) {
      if (!m_images.contains(row)) {
        if (m_strings.contains(row)) {
          m_strings.removeOne(row);
        }

        m_current_image = m_images.size();
        m_images.append(row);
      } // else already an image.
    }

    return true;
  }

  return false;
}

bool ScanListModel::removeRows(int row, int count, const QModelIndex& parent)
{
  if (row == 0) {
    return false;
  }

  if (row >= m_data_list.size() || row + count <= 0) {
    return false;
  }

  beginRemoveRows(parent, row, row + count - 1);

  for (int r = row; r < row + count; r++) {
    m_data_list.removeAt(r);
  }

  endRemoveRows();
  return true;
}

bool ScanListModel::moveRows(const QModelIndex& sourceParent,
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
  bool good_move = beginMoveRows(sourceParent,
                                 source,
                                 source + count - 1,
                                 destinationParent,
                                 dest_row);

  if (good_move) {
    if (source < destination) {
      for (int s = 0; s < count; s++) {
        QVariant value = m_data_list.takeAt(source + s);
        m_data_list.insert(destination, value);
      }

    } else {
      for (int s = 0; s < count; s++) {
        QVariant value = m_data_list.takeAt(source + s);
        m_data_list.insert(destination + s, value);
      }
    }

  } else {
    return false;
  }

  endMoveRows();
  return true;
}

bool ScanListModel::insertRows(int row, int count, const QModelIndex& parent)
{
  if (parent.isValid()) {
    return false;
  }

  if (row < 0 || row > m_data_list.size() || count <= 0) {
    return false;
  }

  beginInsertRows(QModelIndex(), row, row + count - 1);

  for (int index = row, end = row + count; index < end; ++index) {
    m_data_list.insert(index, QVariant());
  }

  endInsertRows();
  return true;
}

int ScanListModel::currentImage() const
{
  return m_current_image;
}
