#ifndef SCANEDIT_H
#define SCANEDIT_H

#include <QAbstractListModel>
#include <QAction>
#include <QList>
#include <QListView>
#include <QMenu>
#include <QStyledItemDelegate>
#include <QTextEdit>
#include <QVariant>

/* ScanItemDelegate
 **************************************************************************************/
class ScanItemDelegate : public QStyledItemDelegate
{
  Q_OBJECT

  public:
  ScanItemDelegate(QObject *parent = nullptr);

  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const override;

  void setEditorData(QWidget* editor, const QModelIndex& index) const override;
  void setModelData(QWidget* editor, QAbstractItemModel* model,
                    const QModelIndex& index) const override;

  void updateEditorGeometry(QWidget* editor,
                            const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

/* ScanListModel
 **************************************************************************************/
class ScanListModel : public QAbstractListModel
{
  public:
  ScanListModel(QObject *parent);

  void clearData();
  QStringList textList();
  QList<QImage> imageList();

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role) override;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index = QModelIndex()) const override;

  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
  bool moveRows(const QModelIndex &sourceParent,
                int sourceRow,
                int count,
                const QModelIndex &destinationParent,
                int destinationChild) override;
  bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

  int currentImage() const;

  protected:
  QList<QVariant> m_data_list;
  QList<int> m_strings;
  QList<int> m_images;
  int m_current_image;
};

/* ScanEdit
 **************************************************************************************/
class ScanList : public QListView
{
  Q_OBJECT
public:
  explicit ScanList(QWidget* parent = nullptr);

  void appendText(const QString &texts);
  int appendImage(const QImage &image);
  void setData(const QMap<int, QVariant> &data);
  void setText(const QString &texts);
  void setText(const QStringList &list);

  QStringList texts();
  QList<QImage> images();

  protected:
  ScanListModel *m_model;
  void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // SCANEDIT_H
