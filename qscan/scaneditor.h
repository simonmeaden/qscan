/*
    Copyright © Simon Meaden 2019.
    This file was developed as part of the QScan cpp library but could
    easily be used elsewhere.

    QScan is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QScan is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QScan.  If not, see <http://www.gnu.org/licenses/>.
    It is also available on request from Simon Meaden simonmeaden@sky.com.
*/
#ifndef SCANEDITOR_H
#define SCANEDITOR_H

#include <QAction>
#include <QFrame>
#include <QHBoxLayout>
#include <QImage>
#include <QInputDialog>
#include <QMenu>
#include <QProgressDialog>
#include <QScrollArea>
#include <QScrollBar>

#include "qscan_global.h"
#include "scanimage.h"

class QScan;

class SCANSHARED_EXPORT ScanEditor : public QFrame
{
  Q_OBJECT
public:
  ScanEditor(QScan* scan, QWidget* parent = nullptr);
  ~ScanEditor() override;

  void setImage(const QImage& image);
  void setScanProgress(const int& progress);
  void scanningStarted();

  void setSelectedName(const QString& selected_name);

  void selectAll();
  void rotate180();
  void rotateCW();
  void rotateACW();
  void rotateByAngle();
  void rotateByEdge();
  void copySelection();
  void cropToSelection();
  void clearSelection();
  void cropToContent();
  void rescan();
  void scale();
  void save();
  void saveAs();
  void zoomIn();
  void zoomOut();
  void fitBest();
  void fitHeight();
  void fitWidth();

signals:
  void scanCancelled();
  void selected();
  void unselected();
  void selectionUnderway();
  void imageIsLoaded();

protected:
  ScanImage* m_image_display;
  QProgressDialog* m_prog_dlg;
  QPoint m_origin;
  QScan* m_scan_lib;
  QString m_selected_name;
  QScrollArea* scroll;

  //  QAction* m_cut_act;
  QAction* m_copy_selection_act;
  QAction* m_crop_to_selection_act;
  QAction* m_clear_selection_act;
  QAction* m_crop_to_content_act;
  QAction* m_rotate_cw_act;
  QAction* m_rotate_acw_act;
  QAction* m_rotate_180_act;
  QAction* m_rotate_by_angle_act;
  QAction* m_rotate_by_edge_act;
  QAction* m_rescan_act;
  QAction* m_scale_act;
  QAction* m_selectall_act;
  QAction* m_save_act;
  QAction* m_save_as_act;

  bool m_select_all;
  //  int m_tl_x, m_tl_y, m_br_x, m_br_y;

  void contextMenuEvent(QContextMenuEvent* event) override;
  bool eventFilter(QObject* obj, QEvent* event) override;

  void adjustScrollbar(qreal factor);
  void initActions();
};

#endif // SCANEDITOR_H
