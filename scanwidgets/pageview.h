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

  void appendThumbnail(const QImage& thumbnail);
  void removeThumbnail(int index);
  void insertThumbnail(int index,
                       const QImage& thumbnail,
                       bool has_text = false,
                       bool is_internal_image = false);
  void setCover(const QImage& cover);
  bool hasText(int page_no);
  void setHasText(int index, bool has_text);
  bool isInternalImage(int page_no);
  void setIsInternal(int index, bool is_internal_image);

  QMap<int, bool> has_text() const;
  void setHas_text(const QMap<int, bool>& has_text);

signals:
  void pageMoved(int from, int to);
  void sendOcrPage(int);
  void workOn(int);
  void clearSaveAllFlag();
  void loadText(int);
  void removeCurrentImage(int);
  void removeCurrentText(int);

protected:
  ImageList m_images;
  QMap<int, bool> m_has_text;
  QMap<int, bool> m_is_internal_image;
  ImageView* m_image_list;
  QImage m_cover;
  int m_current_row{};

  void contextMenuEvent(QContextMenuEvent* event) override;
  QSize minimumSizeHint() const override;
  QSize sizeHint() const override;

  QAction* m_remove_page_act{};
  QAction* m_remove_image_act{};
  QAction* m_remove_text_act{};
  QAction* m_move_page_up_act{};
  QAction* m_move_page_down_act{};
  QAction* m_load_text_act{};
  QAction* m_work_with_act{};
  QAction* m_save_as_image_act{};
  //  QAction* m_do_ocr_act{};
  //  QAction* m_do_all_ocr_act{};

  void rowsMoved(const QModelIndex&, int start, int, const QModelIndex&, int row);
  void removePage();
  void removeImage();
  void removeText();
  void moveUp();
  void moveDown();
  void nonOcrImage();
  //  void doOcr();
  //  void doAllOcr();
  void workOnImage();
  void loadTextIntoEditor();

  static const QString HASTEXT;
  static const QString HASNOTEXT;
};

#endif // PAGEVIEW_H
