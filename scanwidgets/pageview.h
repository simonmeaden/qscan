#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QAction>
#include <QHBoxLayout>
#include <QIcon>
#include <QImage>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QWidget>

#include "scanpage.h"

class PageView : public QWidget
{
  Q_OBJECT
public:
  explicit PageView(QWidget* parent = nullptr);

  int append(const QImage& thumbnail);
  void remove(int index);
  void insert(int index, const QImage& thumbnail);
  void setCover(const QImage& cover);

signals:
  void pageMoved(int from, int to);
  void sendOcrPage(int);

protected:
  QList<QImage> m_pages;
  QListWidget* m_image_list;
  QImage m_cover;

  void contextMenuEvent(QContextMenuEvent* event) override;
  QSize minimumSizeHint() const override;
  QSize sizeHint() const override;

  QAction* m_remove_page_act;
  QAction* m_move_page_up_act;
  QAction* m_move_page_down_act;
  QAction* m_do_ocr_act;

  void rowsMoved(const QModelIndex&,
                 int start,
                 int,
                 const QModelIndex&,
                 int row);
  void moveUp();
  void moveDown();
  void doOcr();
};

#endif // PAGEVIEW_H
