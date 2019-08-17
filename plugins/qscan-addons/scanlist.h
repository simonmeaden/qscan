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

#include "styledstring.h"

namespace QScanner {

/* ScanItemDelegate
 **************************************************************************************/
//class ScanItemDelegate : public QStyledItemDelegate
//{
//  Q_OBJECT

//  public:
//  ScanItemDelegate(QObject *parent = nullptr);

//  QWidget *createEditor(QWidget *parent,
//                        const QStyleOptionViewItem &option,
//                        const QModelIndex &index) const override;

//  void setEditorData(QWidget* editor, const QModelIndex& index) const override;
//  void setModelData(QWidget* editor, QAbstractItemModel* model,
//                    const QModelIndex& index) const override;

//  void updateEditorGeometry(QWidget* editor,
//                            const QStyleOptionViewItem& option, const QModelIndex& index) const override;
//};

/* ScanListModel
 **************************************************************************************/
class ScanListModel : public QAbstractListModel
{
public:
  ScanListModel(QObject* parent);

  void clearData();
  StyledStringList textList();
  QList<QImage> imageList();

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::DisplayRole) override;
  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& index = QModelIndex()) const override;

  bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
  bool moveRows(const QModelIndex& sourceParent,
                int sourceRow,
                int count,
                const QModelIndex& destinationParent,
                int destinationChild) override;
  bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

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

  void appendText(const QString& text);
  void appendText(const StyledString& text);
  int appendImage(const QImage& image);

  void replaceText(int row, const QString& text);
  void replaceText(int row, const StyledString& text);
  void replaceImage(int row, const QImage& image);

  void removeDataRow(const QModelIndex index);

  void setData(const QMap<int, QVariant>& data);
  void setText(const QString& texts);
  void setText(const StyledString& texts);
  void setText(const StyledStringList& list);
  void setText(const QStringList& list);
  int setImage(const QImage& image);

  void dumpData();

  bool hasDataChanges();

  StyledStringList texts();
  QList<QImage> images();

  int size();

signals:
  void dataHasChanged();
  void imageHasChanged();

protected:
  ScanListModel* m_model;
  bool m_changes;
  void contextMenuEvent(QContextMenuEvent* event) override;
};

} // end of namespace QScanner

#endif // SCANEDIT_H
