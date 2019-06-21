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
  connect(m_image_view->model(), &QAbstractItemModel::rowsMoved, this, &PageView::ocrRowsMoved);
  connect(m_image_view->selectionModel(),
          &QItemSelectionModel::selectionChanged,
          this,
          &PageView::ocrSelectionChanged);
  connect(m_image_view, &QListView::doubleClicked, this, &PageView::itemDoubleClicked);
  connect(m_image_view, &QListView::clicked, this, &PageView::itemClicked);
  layout->addWidget(m_image_view);

  m_doc_image_view = new ImageView(this);
  m_doc_image_view->setSelectionMode(QAbstractItemView::SingleSelection);
  //  m_internal_image_view->setDragDropMode(QAbstractItemView::InternalMove);
  //  connect(m_internal_image_view->model(), &QAbstractItemModel::rowsMoved, this, &PageView::rowsMoved);
  connect(m_doc_image_view->selectionModel(),
          &QItemSelectionModel::selectionChanged,
          this,
          &PageView::docSelectionChanged);
  //  connect(m_internal_image_view, &QListView::doubleClicked, this, &PageView::itemDoubleClicked);
  //  connect(m_internal_image_view, &QListView::clicked, this, &PageView::itemClicked);
  layout->addWidget(m_doc_image_view);
}

int PageView::appendOcrThumbnail(const QImage &thumbnail)
{
  return m_image_view->appendThumbnail(thumbnail);
}

void PageView::removeOcrThumbnail()
{
  if (m_ocr_current_row == 0) {
    m_image_view->setCover(QImage());
    m_ocr_current_row = -1;

  } else {
    m_image_view->removeThumbnail(m_ocr_current_row);
    m_ocr_current_row = -1;
  }
}

int PageView::appendDocImage(const QImage &thumbnail)
{
  return m_doc_image_view->appendThumbnail(thumbnail);
}

int PageView::currentPageNumber() const
{
  return m_ocr_current_row;
}

void PageView::removeOcrThumbnail(int index)
{
  m_image_view->removeThumbnail(index);
}

void PageView::replaceOcrThumbnail(int index, const QImage &image)
{
  m_image_view->replaceThumbnail(index, image);
}

void PageView::insertOcrThumbnail(int row, const QImage &thumbnail)
{
  m_image_view->insertThumbnail(row, thumbnail);
}

int PageView::moveOcrThumbnailToInternal(int row)
{
  QImage image = m_image_view->removeThumbnail(row);
  return m_doc_image_view->appendThumbnail(image);
}

void PageView::setCover(const QImage &cover)
{
  m_image_view->setCover(cover);
}

QSize PageView::minimumSizeHint() const
{
  return { 100, 150 };
}

QSize PageView::sizeHint() const
{
  return {120, 200};
}

void PageView::ocrSelectionChanged(const QItemSelection &selected_items,
                                   const QItemSelection & /*deselected_items*/)
{
  if (selected_items.count() != 0) {
    emit selected();
    m_ocr_current_row = selected_items.indexes().at(0).row();

  } else {
    emit unselected();
    m_ocr_current_row = -1;
  }
}

void PageView::docSelectionChanged(const QItemSelection &selected_items, const QItemSelection &)
{
  if (selected_items.count() != 0) {
    emit selected();
    m_doc_current_row = selected_items.indexes().at(0).row();

  } else {
    emit unselected();
    m_doc_current_row = -1;
  }
}

void PageView::ocrRowsMoved(const QModelIndex & /*parent*/,
                            int start_row,
                            int /*end*/,
                            const QModelIndex & /*destination*/,
                            int dest_row)
{
  emit ocrPageMoved(start_row, dest_row);
}

void PageView::docRowsMoved(
  const QModelIndex &, int start_row, int, const QModelIndex &, int dest_row)
{
  emit docPageMoved(start_row, dest_row);
}

void PageView::removeOcrPage()
{
  removeOcrThumbnail();
  //  removeText();
}

void PageView::moveUp()
{
  if (m_ocr_current_row == 0) { // already at top
    return;
  }

  bool good_move = m_image_view->moveThumbnail(m_ocr_current_row, m_ocr_current_row - 1);

  if (good_move) {
    m_image_view->setCurrentRow(--m_ocr_current_row);
  }
}

void PageView::moveDown()
{
  if (m_ocr_current_row == (m_image_view->model()->rowCount() - 1)) { // already at bottom
    return;
  }

  bool good_move = m_image_view->moveThumbnail(m_ocr_current_row, m_ocr_current_row + 1);

  if (good_move) {
    m_image_view->setCurrentRow(++m_ocr_current_row);
  }
}

void PageView::nonOcrImage() {}

void PageView::workOnImage()
{
  emit workOn(m_ocr_current_row);
}

void PageView::loadTextIntoEditor()
{
  emit loadText(m_image_view->currentIndex().row());
}

void PageView::itemClicked(const QModelIndex &index)
{
  m_ocr_current_row = index.row();
  emit loadText(m_ocr_current_row);
}

void PageView::itemDoubleClicked(const QModelIndex &index)
{
  m_ocr_current_row = index.row();
  workOnImage();
}
