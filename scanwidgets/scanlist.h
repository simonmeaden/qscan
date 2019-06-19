#ifndef SCANEDIT_H
#define SCANEDIT_H

//#include <QFrame>
//#include <QVBoxLayout>
#include <QListView>
#include <QMenu>
#include <QAction>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <QPlainTextEdit>

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

/* ScanEdit
 **************************************************************************************/
class ScanList : public QListView
{
  Q_OBJECT
public:
  explicit ScanList(QWidget* parent = nullptr);

  void appendText(const QString &text);
  void setText(const QString &text);
  void setText(const QStringList &list);

  QStringList text();

protected:
  QStringListModel* m_model;
  void contextMenuEvent(QContextMenuEvent* event) override;

};

#endif // SCANEDIT_H
