#include "pageview.h"

const QString PageView::HASTEXT = "border: 2px solid green; border-radius: 3px; margin: 2px; padding: 2px;";
const QString PageView::HASNOTEXT = "border: 2px solid red; border-radius: 3px;; margin: 2px; padding: 2px;";

PageView::PageView(QWidget* parent)
  : QWidget(parent)
{
  auto* layout = new QHBoxLayout;
  setLayout(layout);
  m_image_list = new ImageView(this);
  m_image_list->setSelectionMode(QAbstractItemView::SingleSelection);
  m_image_list->setDragDropMode(QAbstractItemView::InternalMove);
  connect(m_image_list->model(), &QAbstractItemModel::rowsMoved, this, &PageView::rowsMoved);
  m_remove_page_act = new QAction(tr("Remove the selected image and text"), this);
  m_remove_image_act = new QAction(tr("Remove the selected image"), this);
  m_remove_text_act = new QAction(tr("Remove the selected text"), this);
  m_save_as_image_act = new QAction(tr("Save the selected page as an internal image."), this);
  m_save_as_image_act->setToolTip(tr("Stores image for non-OCR, internal use."));
  m_move_page_up_act = new QAction(tr("Move page up"), this);
  m_move_page_down_act = new QAction(tr("Move page down"), this);
  m_load_text_act = new QAction(tr("Load text."), this);
  m_work_with_act = new QAction(tr("Work with selected image."), this);
  //  m_do_ocr_act = new QAction(tr("Run OCR on selected image."), this);
  //  m_do_all_ocr_act = new QAction(tr("Run OCR on all text-free images."),
  //  this);
  connect(m_remove_page_act, &QAction::triggered, this, &PageView::removePage);
  connect(m_remove_image_act, &QAction::triggered, this, &PageView::removeImage);
  connect(m_remove_text_act, &QAction::triggered, this, &PageView::removeText);
  connect(m_move_page_up_act, &QAction::triggered, this, &PageView::moveUp);
  connect(m_move_page_down_act, &QAction::triggered, this, &PageView::moveDown);
  connect(m_save_as_image_act, &QAction::triggered, this, &PageView::nonOcrImage);
  //  connect(m_do_ocr_act, &QAction::triggered, this, &PageView::doOcr);
  //  connect(m_do_all_ocr_act, &QAction::triggered, this, &PageView::doAllOcr);
  connect(m_work_with_act, &QAction::triggered, this, &PageView::workOnImage);
  connect(
    m_load_text_act, &QAction::triggered, this, &PageView::loadTextIntoEditor);
  layout->addWidget(m_image_list);
}

int PageView::appendThumbnail(const QImage &thumbnail, bool has_text,
                              bool is_internal_image) {
  // this list will always start at position 1.
  return m_image_list->appendThumbnail(thumbnail, false);
}

void PageView::removeImage()
{
  if (m_current_row == 0) {
    int answer =
      QMessageBox::warning(this,
                           tr("Remove Cover Image"),
                           tr("You are about to remove the Cover image\n"
                              "This cannot be undone.\n"
                              "Are you sure?"),
                           QMessageBox::Yes | QMessageBox::No,
                           QMessageBox::No);

    if (answer == QMessageBox::Yes) {
      m_image_list->setCover(QImage());
      emit removeCurrentImage(0);

    }

  } else {
    int answer = QMessageBox::warning(this,
                                      (isInternalImage(m_current_row)
                                       ? tr("Remove Internal Image")
                                       : tr("Remove OCR Image")),
                                      tr("You are about to remove the image\n"
                                         "This cannot be undone.\n"
                                         "Are you sure?"),
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No);

    if (answer == QMessageBox::Yes) {
      m_image_list->removeThumbnail(m_current_row);
      emit removeCurrentImage(m_current_row);
    }
  }
}

void PageView::removeText()
{
  if (m_current_row > 0) {
    int answer = QMessageBox::warning(this,
                                      tr("Remove Text"),
                                      tr("You are about to remove the text\n"
                                         "for this OCR image.\n"
                                         "This cannot be undone.\n"
                                         "Are you sure?"),
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No);

    if (answer == QMessageBox::Yes) {
      emit removeCurrentText(m_current_row);
    }
  }
}

void PageView::removeThumbnail(int index)
{
  m_image_list->removeThumbnail(index);
}

void PageView::insertThumbnail(int row, const QImage& thumbnail, bool has_text, bool is_internal_image)
{
  m_image_list->insertThumbnail(row, thumbnail, has_text, is_internal_image);
}

void PageView::setCover(const QImage& cover)
{
  m_image_list->setCover(cover);
}

void PageView::setHasText(int index, bool has_text)
{
  m_image_list->setHasText(index, has_text);
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
  m_image_list->setInternalImage(index, is_internal_image);
}

QMap<int, bool> PageView::has_text() const
{
  return m_has_text;
}

void PageView::setHas_text(const QMap<int, bool>& has_text)
{
  m_has_text = has_text;
}

void PageView::contextMenuEvent(QContextMenuEvent* event)
{
  auto* context_menu = new QMenu();
  context_menu->addAction(m_remove_page_act);
  context_menu->addAction(m_remove_image_act);
  context_menu->addAction(m_remove_text_act);
  context_menu->addSeparator();
  context_menu->addAction(m_move_page_up_act);
  context_menu->addAction(m_move_page_down_act);

  // index 0 is always the cover.
  m_current_row = m_image_list->indexAt(event->pos()).row();

  if (m_current_row > 0) {
    context_menu->addSeparator();
    context_menu->addAction(m_work_with_act);
    //    context_menu->addSeparator();
    //    context_menu->addAction(m_do_ocr_act);
    //    context_menu->addAction(m_do_all_ocr_act);
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

void PageView::removePage()
{
  removeImage();
  removeText();
}

void PageView::moveUp()
{
  QModelIndexList rows = m_image_list->selectionModel()->selectedRows();

  int row = rows.at(0).row();

  if (row == 0) { // already at top
    return;
  }

  m_image_list->moveThumbnail(row, row - 1);
}

void PageView::moveDown()
{
  QModelIndexList rows = m_image_list->selectionModel()->selectedRows();

  if (rows.size() > 1) {
    return;
  }

  int row = rows.at(0).row();

  if (row == (m_image_list->model()->rowCount() - 1)) { // already at bottom
    return;
  }

  m_image_list->moveThumbnail(row, row + 1);
}

void PageView::nonOcrImage()
{

}

// void PageView::doOcr()
//{
//  emit sendOcrPage(m_image_list->currentIndex().row());
//}

// void PageView::doAllOcr()
//{
//  emit clearSaveAllFlag();

//  for (int row = 1; m_image_list->count(); row++) {
//    emit sendOcrPage(row); // cover page is not included
//  }

//  emit clearSaveAllFlag();
//}

void PageView::workOnImage()
{
  emit workOn(m_current_row);
}

void PageView::loadTextIntoEditor()
{
  emit loadText(m_image_list->currentIndex().row());
}
