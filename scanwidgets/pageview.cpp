#include "pageview.h"

PageView::PageView(QWidget* parent)
  : QWidget(parent)
  , m_remove_page_act(nullptr)
  , m_move_page_up_act(nullptr)
  , m_move_page_down_act(nullptr)
  , m_do_ocr_act(nullptr)
{
  auto* layout = new QHBoxLayout;
  setLayout(layout);

  m_image_list = new QListWidget(this);
  m_image_list->setSelectionMode(QAbstractItemView::SingleSelection);
  m_image_list->setViewMode(QListWidget::IconMode);
  m_image_list->setIconSize(QSize(200, 200));
  m_image_list->setResizeMode(QListWidget::Adjust);
  m_image_list->setDragDropMode(QAbstractItemView::InternalMove);
  // insert an empty image at zero position. This will be the cover.
  QListWidgetItem* item = new QListWidgetItem(QIcon(), "", m_image_list);
  m_image_list->addItem(item);

  connect(m_image_list->model(),
          &QAbstractItemModel::rowsMoved,
          this,
          &PageView::rowsMoved);

  m_remove_page_act = new QAction(tr("Remove the selected page"), this);
  m_move_page_up_act = new QAction(tr("Move page up"), this);
  m_move_page_down_act = new QAction(tr("Move page down"), this);
  m_do_ocr_act = new QAction(tr("Run OCR on selected image."), this);

  connect(m_remove_page_act, &QAction::triggered, this, &PageView::remove);
  connect(m_move_page_up_act, &QAction::triggered, this, &PageView::moveUp);
  connect(m_move_page_down_act, &QAction::triggered, this, &PageView::moveDown);
  connect(m_do_ocr_act, &QAction::triggered, this, &PageView::doOcr);
  layout->addWidget(m_image_list);
}

int
PageView::append(const QImage& thumbnail)
{
  // this list will always start at position 1.
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
PageView::setCover(const QImage& cover)
{
  m_cover = cover;
  auto* item = m_image_list->item(0);
  item->setIcon(QIcon(QPixmap::fromImage(cover)));
}

void
PageView::contextMenuEvent(QContextMenuEvent* event)
{
  auto* context_menu = new QMenu();

  context_menu->addAction(m_remove_page_act);
  context_menu->addSeparator();
  context_menu->addAction(m_move_page_up_act);
  context_menu->addAction(m_move_page_down_act);
  context_menu->addSeparator();
  context_menu->addAction(m_do_ocr_act);

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

void
PageView::doOcr()
{
  emit sendOcrPage(m_image_list->currentIndex().row());
}
