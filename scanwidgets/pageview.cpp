#include "pageview.h"

const QString PageView::HASTEXT
  = "border: 2px solid green; border-radius: 3px; margin: 2px; padding: 2px;";
const QString PageView::HASNOTEXT
  = "border: 2px solid red; border-radius: 3px;; margin: 2px; padding: 2px;";

PageView::PageView(QWidget *parent)
  : QWidget(parent)
{
  auto* layout = new QHBoxLayout;
  setLayout(layout);
  m_image_view = new ImageView(this);
  m_image_view->setSelectionMode(QAbstractItemView::SingleSelection);
  m_image_view->setDragDropMode(QAbstractItemView::InternalMove);
  connect(m_image_view->model(), &QAbstractItemModel::rowsMoved, this, &PageView::rowsMoved);
  connect(m_image_view->selectionModel(),
          &QItemSelectionModel::selectionChanged,
          this,
          &PageView::selectionChanged);
  connect(m_image_view, &QListView::doubleClicked, this, &PageView::itemDoubleClicked);
  connect(m_image_view, &QListView::clicked, this, &PageView::itemClicked);

  layout->addWidget(m_image_view);
}

int PageView::appendThumbnail(const QImage &thumbnail, bool has_text, bool is_internal_image)
{
  // this list will always start at position 1.
  return m_image_view->appendThumbnail(thumbnail, has_text, is_internal_image);
}

void PageView::removeImage()
{
  if (m_current_row == 0) {
    m_image_view->setCover(QImage());
    m_current_row = -1;

  } else {
    m_image_view->removeThumbnail(m_current_row);
    m_current_row = -1;
  }
}

//void PageView::removeText()
//{
//  if (m_current_row > 0) {
//    int answer = QMessageBox::warning(this,
//                                      tr("Remove Text"),
//                                      tr("You are about to remove the text\n"
//                                         "for this OCR image.\n"
//                                         "This cannot be undone.\n"
//                                         "Are you sure?"),
//                                      QMessageBox::Yes | QMessageBox::No,
//                                      QMessageBox::No);

//    if (answer == QMessageBox::Yes) {
//      emit removeCurrentText(m_current_row);
//    }
//  }
//}

int PageView::currentPageNumber() const
{
  return m_current_row;
}

void PageView::removeThumbnail(int index)
{
  m_image_view->removeThumbnail(index);
}

void PageView::replaceThumbnail(int index,
                                const QImage &image,
                                bool has_text,
                                bool is_internal_image)
{
  m_image_view->replaceThumbnail(index, image, has_text, is_internal_image);
}

void PageView::insertThumbnail(int row,
                               const QImage &thumbnail,
                               bool has_text,
                               bool is_internal_image)
{
  m_image_view->insertThumbnail(row, thumbnail, has_text, is_internal_image);
}

void PageView::setCover(const QImage &cover)
{
  m_image_view->setCover(cover);
}

void PageView::setHasText(int index, bool has_text)
{
  m_image_view->setHasText(index, has_text);
}

bool PageView::hasText(int page_no)
{
  if (m_has_text.contains(page_no)) {
    return m_has_text.value(page_no);
  }

  return false;
}

bool PageView::isInternalImage(int page_no)
{
  if (m_is_internal_image.contains(page_no)) {
    return m_is_internal_image.value(page_no);
  }

  return false;
}

void PageView::setIsInternal(int index, bool is_internal_image)
{
  m_image_view->setInternalImage(index, is_internal_image);
}

QMap<int, bool> PageView::hasTextMap() const
{
  return m_has_text;
}

void PageView::setHasTextMap(const QMap<int, bool> &has_text)
{
  m_has_text = has_text;
}

//void PageView::contextMenuEvent(QContextMenuEvent *event)
//{
//  auto* context_menu = new QMenu();
//  context_menu->addAction(m_remove_page_act);
//  context_menu->addAction(m_remove_image_act);
//  context_menu->addAction(m_remove_text_act);
//  //  context_menu->addSeparator();
//  //  context_menu->addAction(m_move_page_up_act);
//  //  context_menu->addAction(m_move_page_down_act);

//  // index 0 is always the cover.
//  m_current_row = m_image_view->indexAt(event->pos()).row();

//  if (m_current_row > 0) {
//    context_menu->addSeparator();
//    context_menu->addAction(m_work_with_act);
//    //    context_menu->addSeparator();
//    //    context_menu->addAction(m_do_ocr_act);
//    //    context_menu->addAction(m_do_all_ocr_act);
//    context_menu->addSeparator();
//    context_menu->addAction(m_load_text_act);
//  }

//  context_menu->popup(event->globalPos());
//}

QSize PageView::minimumSizeHint() const
{
  return { 100, 150 };
}

QSize PageView::sizeHint() const
{
  return {120, 200};
}

void PageView::selectionChanged(const QItemSelection &selected_items,
                                const QItemSelection & /*deselected_items*/)
{
  if (selected_items.count() != 0) {
    emit selected();
    m_current_row = selected_items.indexes().at(0).row();

  } else {
    emit unselected();
    m_current_row = -1;
  }
}

void PageView::rowsMoved(const QModelIndex & /*parent*/,
                         int start_row,
                         int /*end*/,
                         const QModelIndex & /*destination*/,
                         int dest_row)
{
  emit pageMoved(start_row, dest_row);
}

void PageView::removePage()
{
  removeImage();
  //  removeText();
}

void PageView::moveUp()
{
  if (m_current_row == 0) { // already at top
    return;
  }

  bool good_move = m_image_view->moveThumbnail(m_current_row, m_current_row - 1);

  if (good_move) {
    m_image_view->setCurrentRow(--m_current_row);
  }
}

void PageView::moveDown()
{
  if (m_current_row == (m_image_view->model()->rowCount() - 1)) { // already at bottom
    return;
  }

  bool good_move = m_image_view->moveThumbnail(m_current_row, m_current_row + 1);

  if (good_move) {
    m_image_view->setCurrentRow(++m_current_row);
  }
}

void PageView::nonOcrImage() {}

void PageView::workOnImage()
{
  emit workOn(m_current_row);
}

void PageView::loadTextIntoEditor()
{
  emit loadText(m_image_view->currentIndex().row());
}

void PageView::itemClicked(const QModelIndex &index)
{
  m_current_row = index.row();
  emit loadText(m_current_row);
}

void PageView::itemDoubleClicked(const QModelIndex &index)
{
  m_current_row = index.row();
  workOnImage();
}
