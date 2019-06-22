#include "scanlist.h"

/* ScanEdit
 **************************************************************************************/
ScanList::ScanList(QWidget *parent)
  : QListView(parent)
{
  m_model = new ScanListModel(this);
  //  setItemDelegate(new ScanItemDelegate(this));
  setModel(m_model);
}

void ScanList::appendText(const QString &text)
{
  int row = m_model->rowCount();

  if (m_model->insertRow(row)) {
    QModelIndex index = m_model->index(row, 0, QModelIndex());
    m_model->setData(index, QVariant::fromValue<QString>(text), Qt::EditRole);
  }
}

int ScanList::appendImage(const QImage &image)
{
  int row = m_model->rowCount();

  if (m_model->insertRow(row)) {
    QModelIndex index = m_model->index(row, 0);
    m_model->setData(index, image, Qt::EditRole);
    return row;
  }

  return -1;
}

void ScanList::setData(const QMap<int, QVariant> &data)
{
  m_model->clearData();

  for (auto it = data.keyBegin(), end = data.keyEnd(); it != end; ++it) {
    int row = *it;

    if (m_model->insertRow(row)) {
      QModelIndex index = m_model->index(row, 0);
      m_model->setData(index, data.value(row), Qt::EditRole);
    }
  }
}

void ScanList::setText(const QString &text)
{
  m_model->clearData();
  m_model->insertRow(0);
  const QModelIndex index = m_model->index(0, 0);
  m_model->setData(index, text, Qt::EditRole);
}

void ScanList::setText(const QStringList &list)
{
  m_model->clearData();
  m_model->insertRows(0, list.size());

  for (int row = 0; row < list.size(); row++) {
    const QModelIndex index = m_model->index(row, 0);
    m_model->setData(index, list.at(row), Qt::EditRole);
  }
}

QStringList ScanList::texts()
{
  return m_model->textList();
}

QList<QImage> ScanList::images()
{
  return m_model->imageList();
}

void ScanList::contextMenuEvent(QContextMenuEvent * /*event*/)
{
  //  auto *context_menu = new QMenu();
}

/* ScanItemDelegate
 **************************************************************************************/
ScanItemDelegate::ScanItemDelegate(QObject* parent)
  : QStyledItemDelegate(parent)
{}

QWidget* ScanItemDelegate::createEditor(QWidget* parent,
                                        const QStyleOptionViewItem& /*option*/,
                                        const QModelIndex& /*index*/) const
{
  auto *editor = new QTextEdit(parent);
  editor->setFrameStyle(QFrame::NoFrame);

  return editor;
}

void ScanItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
  QVariant value = index.model()->data(index, Qt::EditRole);

  auto *text_edit = dynamic_cast<QTextEdit *>(editor);

  if (value.type() == QVariant::String) {
    text_edit->setPlainText(value.toString());

  } else if (value.type() == QVariant::Image) {
    QImage image = value.value<QImage>();
    QTextDocument *document = text_edit->document();
    document->addResource(QTextDocument::ImageResource, QUrl("mydata://image.png"), QVariant(image));
    QTextCursor cursor(document);
    QTextImageFormat imageFormat;
    imageFormat.setName("mydata://image.png");
    cursor.insertImage(imageFormat);
  }
}

void ScanItemDelegate::setModelData(QWidget* editor,
                                    QAbstractItemModel* model,
                                    const QModelIndex& index) const
{
  auto *text_edit = dynamic_cast<QTextEdit *>(editor);
  QVariant value = index.model()->data(index, Qt::EditRole);
  model->setData(index, value, Qt::EditRole);
}

void ScanItemDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex & /*index*/) const
{
  editor->setGeometry(option.rect);
}

/* ScanListModel
 **************************************************************************************/
ScanListModel::ScanListModel(QObject *parent)
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

QModelIndex ScanListModel::index(int row, int column, const QModelIndex &parent) const
{
  return createIndex(row, column);
}

QModelIndex ScanListModel::parent(const QModelIndex &index) const
{
  return {};
}

QStringList ScanListModel::textList()
{
  QStringList list;

  for (auto key : m_strings) {
    list << m_data_list.at(key).toString();
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

int ScanListModel::rowCount(const QModelIndex & /*parent*/) const
{
  return m_data_list.size();
}

int ScanListModel::columnCount(const QModelIndex & /*parent*/) const
{
  return 1;
}

QVariant ScanListModel::data(const QModelIndex &index, int role) const
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

bool ScanListModel::setData(const QModelIndex &index, const QVariant &value, int /*role*/)
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
  }
}

bool ScanListModel::removeRows(int row, int count, const QModelIndex &parent)
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

bool ScanListModel::moveRows(const QModelIndex &sourceParent,
                             int source,
                             int count,
                             const QModelIndex &destinationParent,
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

bool ScanListModel::insertRows(int row, int count, const QModelIndex &parent)
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
