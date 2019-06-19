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

  int appendThumbnail(const QImage &thumbnail,
                      bool has_text = false,
                      bool is_internal_image = false);
  void removeThumbnail(int index);
  void replaceThumbnail(int index,
                        const QImage &image,
                        bool has_text = false,
                        bool is_internal_image = false);
  void insertThumbnail(int index,
                       const QImage& thumbnail,
                       bool has_text = false,
                       bool is_internal_image = false);
  void setCover(const QImage& cover);
  bool hasText(int page_no);
  void setHasText(int index, bool has_text);
  bool isInternalImage(int page_no);
  void setIsInternal(int index, bool is_internal_image);

  QMap<int, bool> hasTextMap() const;
  void setHasTextMap(const QMap<int, bool> &has_text);

  void moveUp();
  void moveDown();
  void workOnImage();
  void removePage();
  void removeImage();
  //  void removeText();

  int currentPageNumber() const;

  signals:
  void pageMoved(int from, int to);
  void sendOcrPage(int);
  void workOn(int);
  void loadText(int);
  void removeCurrentImage(int);
  void removeCurrentText(int);
  void selected();
  void unselected();

  protected:
  ImageList m_images;
  QMap<int, bool> m_has_text;
  QMap<int, bool> m_is_internal_image;
  ImageView *m_image_view;
  QImage m_cover;
  int m_current_row{};
  int m_page_selected = -1;

  //  void contextMenuEvent(QContextMenuEvent *event) override;
  QSize minimumSizeHint() const override;
  QSize sizeHint() const override;

  void selectionChanged(const QItemSelection &selected_items, const QItemSelection &);

  //  QAction* m_remove_page_act{};
  //  QAction* m_remove_image_act{};
  //  QAction* m_remove_text_act{};
  //  QAction* m_move_page_up_act{};
  //  QAction* m_move_page_down_act{};
  //  QAction* m_load_text_act{};
  //  QAction* m_work_with_act{};
  //  QAction* m_save_as_image_act{};
  //  QAction* m_do_ocr_act{};
  //  QAction* m_do_all_ocr_act{};

  void rowsMoved(const QModelIndex &, int start_row, int, const QModelIndex &, int dest_row);
  void nonOcrImage();
  //  void doOcr();
  //  void doAllOcr();
  void loadTextIntoEditor();
  void itemClicked(const QModelIndex &index);
  void itemDoubleClicked(const QModelIndex &index);

  static const QString HASTEXT;
  static const QString HASNOTEXT;
};

#endif // PAGEVIEW_H
