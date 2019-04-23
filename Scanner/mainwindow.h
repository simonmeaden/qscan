/*
    Copyright © Simon Meaden 2019.
    This file is part of the QScan cpp library.

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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QComboBox>
#include <QGridLayout>
#include <QGuiApplication>
#include <QHeaderView>
#include <QIODevice>
#include <QImage>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QPixmapCache>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScreen>
#include <QScrollArea>
#include <QTableWidget>
#include <QToolBar>

#include <log4qt/logger.h>

#include "scaneditor.h"

class QScan;
typedef QSharedPointer<QImage> Image;

class TextEditIoDevice : public QIODevice
{
  Q_OBJECT

public:
  TextEditIoDevice(QPlainTextEdit* text_edit, QObject* parent = nullptr);
  //  ~TextEditIoDevice() override;

  void setTextEdit(QPlainTextEdit* m_text_edit);

protected:
  qint64 readData(char*, qint64) override;
  qint64 writeData(const char* data, qint64 maxSize) override;

private:
  QPlainTextEdit* m_text_edit;
  QString m_pre_data;
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

  void setLogTextEdit(QPlainTextEdit* log_edit);

protected:
  Log4Qt::Logger* m_logger;
  QScan* m_scan_lib;
  //  QFrame* m_main;
  QGridLayout* m_main_layout;
  ScanEditor* m_image_editor;
  QTableWidget* m_scanners;
  QComboBox *m_mode_box, *m_source_box;
  //  QPushButton* m_close_btn;

  QString m_selected_name;
  bool m_selected;
  QTextStream* m_log_stream;
  QPlainTextEdit* m_empty_edit;
  QPlainTextEdit* m_log_edit;

  QPixmapCache::Key scan_key;
  QPixmapCache::Key rot_left_key;
  QPixmapCache::Key rot_right_key;
  QPixmapCache::Key rot_angle_key;
  QPixmapCache::Key rot_edge_key;
  QPixmapCache::Key copy_key;
  QPixmapCache::Key scale_key;
  QPixmapCache::Key crop_key;
  QPixmapCache::Key save_key;
  QPixmapCache::Key save_as_key;
  QPixmapCache::Key zoom_in_key;
  QPixmapCache::Key zoom_out_key;
  QPixmapCache::Key fit_best_key;
  QPixmapCache::Key fit_width_key;
  QPixmapCache::Key fit_height_key;
  QPixmapCache::Key close_key;

  QAction* m_scan_act;
  QAction* m_rot_left_act;
  QAction* m_rot_right_act;
  QAction* m_rot_angle_act;
  QAction* m_rot_edge_act;
  QAction* m_copy_act;
  QAction* m_crop_act;
  QAction* m_scale_act;
  QAction* m_save_act;
  QAction* m_save_as_act;
  QAction* m_zoom_in_act;
  QAction* m_zoom_out_act;
  QAction* m_fit_best_act;
  QAction* m_fit_width_act;
  QAction* m_fit_height_act;
  QAction* m_close_act;

  void initGui();
  void initActions();
  void connectActions();

  void scannerSelectionChanged();
  void startScanning();
  void cancelScanning();

  void scanHasFailed();
  void scanProgressed(const int&);
  void geometry();
  void doubleClicked(const QModelIndex& index);
  void modifyingSelection();
  void editorHasSelection();
  void editorHasNoSelection();
  void imageLoaded();
  void receiveOptionsSet();
  void modeChanged(const QString& mode);
  void sourceChanged(const QString& source);
  void initToolbar();
  void enableNoSelectionBtns();
  void enableSelectionBtns();
  void disableSelectionBtns();
  void disableNoSelectionBtns();
  void enableImageLoadedBtns();
  void disableImageLoadedBtns();
};

#endif // MAINWINDOW_H
