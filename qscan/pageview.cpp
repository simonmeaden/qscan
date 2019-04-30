#include "pageview.h"

PageView::PageView(QWidget* parent)
  : QWidget(parent)
  , m_remove_page_act(new QAction(tr("Remove the selected page"), this))
  , m_move_page_up_act(new QAction(tr("Move page up"), this))
  , m_move_page_down_act(new QAction(tr("Move page down"), this))
{
  auto* layout = new QHBoxLayout;
  setLayout(layout);

  m_image_list = new QListWidget(this);
  m_image_list->setSelectionMode(QAbstractItemView::SingleSelection);
  m_image_list->setDragDropMode(QAbstractItemView::InternalMove);
  connect(m_image_list->model(),
          &QAbstractItemModel::rowsMoved,
          this,
          &PageView::rowsMoved);
  connect(m_remove_page_act, &QAction::triggered, this, &PageView::remove);
  connect(m_move_page_up_act, &QAction::triggered, this, &PageView::moveUp);
  connect(m_move_page_down_act, &QAction::triggered, this, &PageView::moveDown);
  layout->addWidget(m_image_list);
}

int
PageView::append(const QImage& thumbnail)
{
  QListWidgetItem* item =
    new QListWidgetItem(QIcon(QPixmap::fromImage(thumbnail)), "", m_image_list);
  m_image_list->addItem(item);
}

void
PageView::remove(int index)
{
  QListWidgetItem* item = m_image_list->takeItem(index);
  delete item;
}

void
PageView::insert(int row, const QImage& thumbnail)
{
  QListWidgetItem* item =
    new QListWidgetItem(QIcon(QPixmap::fromImage(thumbnail)), "", m_image_list);
  m_image_list->insertItem(row, item);
}

void
PageView::contextMenuEvent(QContextMenuEvent* event)
{
  auto* context_menu = new QMenu();

  context_menu->addAction(m_remove_page_act);
  context_menu->addSeparator();
  context_menu->addAction(m_move_page_up_act);
  context_menu->addAction(m_move_page_down_act);

  context_menu->popup(event->globalPos());
}

QSize
PageView::minimumSizeHint() const
{
  return { 100, 150 };
}

QSize
PageView::sizeHint() const
{
  return { 120, 200 };
}

void
PageView::rowsMoved(const QModelIndex& /*parent*/,
                    int start,
                    int /*end*/,
                    const QModelIndex& /*destination*/,
                    int row)
{
  emit pageMoved(start, row);
}

void
PageView::moveUp()
{
  // TODO move up
}

void
PageView::moveDown()
{
  // TODO move down
}
