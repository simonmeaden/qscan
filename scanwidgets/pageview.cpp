#include "pageview.h"

const QString PageView::HASTEXT = "border: 2px solid green; border-radius: 3px; margin: 2px; padding: 2px;";
const QString PageView::HASNOTEXT = "border: 2px solid red; border-radius: 3px;; margin: 2px; padding: 2px;";

PageView::PageView(QWidget* parent)
  : QWidget(parent)
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
  QListWidgetItem* item = new QListWidgetItem(QIcon(), "No Cover Set", m_image_list);
  m_image_list->addItem(item);
  connect(m_image_list->model(), &QAbstractItemModel::rowsMoved, this, &PageView::rowsMoved);
  m_remove_page_act = new QAction(tr("Remove the selected page"), this);
  m_move_page_up_act = new QAction(tr("Move page up"), this);
  m_move_page_down_act = new QAction(tr("Move page down"), this);
  m_load_text_act = new QAction(tr("Load text."), this);
  m_do_ocr_act = new QAction(tr("Run OCR on selected image."), this);
  m_do_all_ocr_act = new QAction(tr("Run OCR on all text-free images."), this);
  connect(m_remove_page_act, &QAction::triggered, this, &PageView::remove);
  connect(m_move_page_up_act, &QAction::triggered, this, &PageView::moveUp);
  connect(m_move_page_down_act, &QAction::triggered, this, &PageView::moveDown);
  connect(m_do_ocr_act, &QAction::triggered, this, &PageView::doOcr);
  connect(m_do_all_ocr_act, &QAction::triggered, this, &PageView::doAllOcr);
  connect(m_load_text_act, &QAction::triggered, this, &PageView::loadTextIntoEditor);
  layout->addWidget(m_image_list);
}

void PageView::append(const QImage& thumbnail)
{
  // this list will always start at position 1.
  QLabel* lbl = new QLabel(this);
  lbl->setPixmap(QPixmap::fromImage(thumbnail));
  lbl->setStyleSheet(HASNOTEXT);
  auto* item = new QListWidgetItem(m_image_list);
  item->setSizeHint(QSize(thumbnail.width(), thumbnail.height()));
  m_image_list->addItem(item);
  m_image_list->setItemWidget(item, lbl);
}

void PageView::remove(int index)
{
  QListWidgetItem* item = m_image_list->takeItem(index);
  delete item;
}

void PageView::insert(int row, const QImage& thumbnail, bool has_text)
{
  QLabel* lbl = new QLabel(this);
  lbl->setPixmap(QPixmap::fromImage(thumbnail));

  if (has_text) {
    lbl->setStyleSheet(HASTEXT);

  } else {
    lbl->setStyleSheet(HASNOTEXT);
  }

  auto* item = new QListWidgetItem(m_image_list);
  item->setSizeHint(QSize(thumbnail.width(), thumbnail.height()));
  m_image_list->insertItem(row, item);
  m_image_list->setItemWidget(item, lbl);
}

void PageView::setCover(const QImage& cover)
{
  QLabel* lbl = new QLabel(this);
  lbl->setPixmap(QPixmap::fromImage(cover));
  lbl->setStyleSheet(HASTEXT);
  auto* item = m_image_list->item(0);
  item->setText(QString());
  item->setSizeHint(QSize(cover.width(), cover.height()));
  m_image_list->setItemWidget(item, lbl);
}

void PageView::setHasText(int index, bool has_text)
{
  auto* item = m_image_list->item(index);
  auto* lbl = m_image_list->itemWidget(item);
  m_has_text.insert(index, has_text);

  if (has_text) {
    lbl->setStyleSheet(HASTEXT);

  } else {
    lbl->setStyleSheet(HASNOTEXT);
  }
}

bool PageView::hasText(int page_no)
{
  if (m_has_text.contains(page_no)) {
    return m_has_text.value(page_no);
  }

  return false;
}

void PageView::contextMenuEvent(QContextMenuEvent* event)
{
  auto* context_menu = new QMenu();
  context_menu->addAction(m_remove_page_act);
  context_menu->addSeparator();
  context_menu->addAction(m_move_page_up_act);
  context_menu->addAction(m_move_page_down_act);

  // index 0 is always the cover.
  if (m_image_list->indexAt(event->pos()).row() > 0) {
    context_menu->addSeparator();
    context_menu->addAction(m_do_ocr_act);
    context_menu->addAction(m_do_all_ocr_act);
    context_menu->addSeparator();
    context_menu->addAction(m_load_text_act);
  }

  context_menu->popup(event->globalPos());
}

QSize PageView::minimumSizeHint() const
{
  return { 100, 150 };
}

QSize PageView::sizeHint() const
{
  return { 120, 200 };
}

void PageView::rowsMoved(const QModelIndex& /*parent*/,
                         int start,
                         int /*end*/,
                         const QModelIndex& /*destination*/,
                         int row)
{
  emit pageMoved(start, row);
}

void PageView::moveUp()
{
  // TODO move up
}

void PageView::moveDown()
{
  // TODO move down
}

void PageView::doOcr()
{
  emit sendOcrPage(m_image_list->currentIndex().row());
}

void PageView::doAllOcr()
{
  emit clearSaveAllFlag();

  for (int row = 1; m_image_list->count(); row++) {
    emit sendOcrPage(row); // cover page is not included
  }

  emit clearSaveAllFlag();
}

void PageView::loadTextIntoEditor()
{
  emit loadText(m_image_list->currentIndex().row());
}
