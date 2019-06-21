#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QAction>
#include <QContextMenuEvent>
#include <QHBoxLayout>
#include <QImage>
#include <QMenu>
#include <QWidget>
#include <QMessageBox>

#include "imageview.h"
#include "documentdata.h"

class PageView : public QWidget
{
  Q_OBJECT
public:
  explicit PageView(QWidget* parent = nullptr);

  int appendOcrThumbnail(const QImage &thumbnail);
  void removeOcrThumbnail(int index);
  void replaceOcrThumbnail(int index, const QImage &image);
  void insertOcrThumbnail(int index, const QImage &thumbnail);
  int moveOcrThumbnailToInternal(int row);
  void removeOcrThumbnail();

  int appendDocImage(const QImage &thumbnail);
  void removeDocImage(int page_no);

  void setCover(const QImage& cover);

  void moveUp();
  void moveDown();
  void workOnImage();
  void removeOcrPage();

  int currentPageNumber() const;

  signals:
  void ocrPageMoved(int from, int to);
  void docPageMoved(int from, int to);
  void sendOcrPage(int);
  void workOn(int);
  void loadText(int);
  void removeCurrentImage(int);
  void removeCurrentText(int);
  void selected();
  void unselected();

  protected:
  ImageView *m_image_view;
  ImageView *m_doc_image_view;
  QImage m_cover;
  int m_ocr_current_row{}, m_doc_current_row{};
  int m_page_selected = -1;

  QSize minimumSizeHint() const override;
  QSize sizeHint() const override;

  void ocrSelectionChanged(const QItemSelection &selected_items, const QItemSelection &);
  void docSelectionChanged(const QItemSelection &selected_items, const QItemSelection &);
  void ocrRowsMoved(const QModelIndex &, int start_row, int, const QModelIndex &, int dest_row);
  void docRowsMoved(const QModelIndex &, int start_row, int, const QModelIndex &, int dest_row);

  void nonOcrImage();
  void loadTextIntoEditor();
  void itemClicked(const QModelIndex &index);
  void itemDoubleClicked(const QModelIndex &index);

  static const QString HASTEXT;
  static const QString HASNOTEXT;
};

#endif // PAGEVIEW_H
