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

//#include <QFrame>
//#include <QHBoxLayout>
#include <QImage>
//#include <QLabel>
//#include <QProgressDialog>
#include <QtWidgets>

#include "qscan_global.h"

class QScan;

class SCANSHARED_EXPORT ScanEditor : public QFrame
{
  Q_OBJECT
public: ScanEditor(QScan* scan, QWidget* parent = nullptr);
  ~ScanEditor();

  void setImage(const QImage& image);
  void setScanProgress(const int& progress);
  void scanningStarted();

  void setSelectedName(const QString& selected_name);

signals:
  void scanCancelled();

protected:
  QLabel* m_image;
  QProgressDialog* m_prog_dlg;
  QRubberBand* m_rubber_band;
  QPoint m_origin;
  QScan* m_scan_lib;
  QString m_selected_name;
  QAction* m_cut_act;
  QAction* m_copy_act;
  QAction* m_crop_to_selection_act;
  QAction* m_crop_to_content_act;
  QAction* m_rotate_cw_act;
  QAction* m_rotate_acw_act;
  QAction* m_rotate_180_act;
  QAction* m_rotate_act;
  QAction* m_rescan_act;
  QAction* m_scale_act;
  QAction* m_selectall_act;

  bool m_is_selection;
  bool m_select_all;
  int m_tl_x, m_tl_y, m_br_x, m_br_y;

  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent*) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void contextMenuEvent(QContextMenuEvent* event) override;
  void selectAll();
  void rotate180();
  void rotateCW();
  void rotateACW();
  void rotateAngle();
  void rotateBy(int angle);
  void cut();
  void copy();
  void crop();
  void cropToContent();
  void rescan();
  void scale();

};

#endif // SCANEDITOR_H
