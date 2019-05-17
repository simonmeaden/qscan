#include "scanedit.h"

/* ScanEdit
 **************************************************************************************/
ScanEdit::ScanEdit(QWidget* parent)
  : QListView(parent)
{
  m_model = new QStringListModel(this);
  setModel(m_model);

  QStringList list;
  list << "";
  m_model->setStringList(list);
}

void ScanEdit::setText(const QString& text)
{
  QStringList list;
  list << text;
  m_model->setStringList(list);
}

void ScanEdit::setText(const QStringList& list)
{
  m_model->setStringList(list);
}

QStringList ScanEdit::text()
{
  return m_model->stringList();
}

void ScanEdit::contextMenuEvent(QContextMenuEvent* event)
{
  auto* context_menu = new QMenu();
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
  auto* editor = new QPlainTextEdit(parent);
  editor->setFrameStyle(QFrame::NoFrame);

  return editor;
}

void ScanItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
  QString value = index.model()->data(index, Qt::EditRole).toString();

  auto* plain_text_edit = dynamic_cast<QPlainTextEdit*>(editor);
  plain_text_edit->setPlainText(value);
}

void ScanItemDelegate::setModelData(QWidget* editor,
                                    QAbstractItemModel* model,
                                    const QModelIndex& index) const
{
  auto* plain_text_edit = dynamic_cast<QPlainTextEdit*>(editor);
  QString text = plain_text_edit->toPlainText();

  model->setData(index, text, Qt::EditRole);
}

void ScanItemDelegate::updateEditorGeometry(QWidget* editor,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
  editor->setGeometry(option.rect);
}
