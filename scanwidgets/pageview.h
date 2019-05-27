#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QAction>
#include <QHBoxLayout>
#include <QImage>
#include <QMenu>
#include <QWidget>
#include <QContextMenuEvent>

#include "imageview.h"
#include "scanpage.h"

class PageView : public QWidget
{
  Q_OBJECT
public:
  explicit PageView(QWidget* parent = nullptr);

  void append(const QImage& thumbnail);
  void remove(int index);
  void insert(int index, const QImage& thumbnail, bool has_text = false);
  void setCover(const QImage& cover);
  void setHasText(int index, bool has_text);
  bool hasText(int page_no);

signals:
  void pageMoved(int from, int to);
  void sendOcrPage(int);
  void workOn(int);
  void clearSaveAllFlag();
  void loadText(int);

protected:
  ImageList m_pages;
  QMap<int, bool> m_has_text;
  ImageView* m_image_list;
  QImage m_cover;
  int m_current_row{};

  void contextMenuEvent(QContextMenuEvent* event) override;
  QSize minimumSizeHint() const override;
  QSize sizeHint() const override;

  QAction* m_remove_page_act{};
  QAction* m_move_page_up_act{};
  QAction* m_move_page_down_act{};
  QAction* m_load_text_act{};
  QAction* m_work_with_act{};
  //  QAction* m_do_ocr_act{};
  //  QAction* m_do_all_ocr_act{};

  void rowsMoved(const QModelIndex&,
                 int start,
                 int,
                 const QModelIndex&,
                 int row);
  void moveUp();
  void moveDown();
  //  void doOcr();
  //  void doAllOcr();
  void workOnImage();
  void loadTextIntoEditor();

  static const QString HASTEXT;
  static const QString HASNOTEXT;
};

#endif // PAGEVIEW_H
