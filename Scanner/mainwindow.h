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
#include <QSpinBox>
#include <QDir>
#include <QFile>
#include <QGridLayout>
#include <QStackedLayout>
#include <QGuiApplication>
#include <QHeaderView>
#include <QImage>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QPixmapCache>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScreen>
#include <QScrollArea>
#include <QTextStream>
#include <QToolBar>
#include <QVariant>
#include <QtHelp/QtHelp>

using Image = QSharedPointer<QImage>;

class ScanDevice;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

protected:
  QMenu* m_main_menu;

  QStackedLayout* m_main_layout{};

  QString m_config_dir;
  QString m_data_dir;
  QString m_lang;
  QString m_current_doc_name;

  QString m_selected_name;
  bool m_selected;
  QList<QMenu*> m_menus;

  int m_stack_range_id{};
  int m_stack_list_id{};

  QPixmapCache::Key help_key;
  //  QPixmapCache::Key scan_key;
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

  //  QAction* m_scan_act{};
  QAction* m_help_act{};
  QAction* m_rot_left_act{};
  QAction* m_rot_right_act{};
  QAction* m_rot_angle_act{};
  QAction* m_rot_edge_act{};
  QAction* m_copy_act{};
  QAction* m_crop_act{};
  QAction* m_scale_act{};
  QAction* m_save_act{};
  QAction* m_save_as_act{};
  QAction* m_zoom_in_act{};
  QAction* m_zoom_out_act{};
  QAction* m_fit_best_act{};
  QAction* m_fit_width_act{};
  QAction* m_fit_height_act{};
  QAction* m_close_act{};
  QAction* m_doc_completed_act{};

  void loadPlugins();

  bool close();

  void initGui();
  void initActions();
  void initToolbar();
  void initMenu();
  void makeConnections();

  void scanProgressed(const int&);
  void imageLoaded();
  void enableNoSelectionActions();
  void enableSelectionActions();
  void enableScanningToolbars(bool enable);
  void disableSelectionActions();
  void disableNoSelectionActions();
  void enableImageLoadedActions();
  void disableImageLoadedActions();
  QToolBar* initMainToolbar();
  QToolBar* initRightToolbar();
  void initPixmaps();
  void makeDelayedConnections();

  //  void enumerateMenu(QAction* act, QMenu* pareny_menu);
  //  void addMenuAction(QMap<QString, QMenu*> menus, QMap<QString, QMenu*> menu_map);


  static const QString OPTIONS_FILE;
  static const QString CURRENT_DOCUMENT;
  static const QString TESSERACT;
  static const QString LANGUAGE;

};

#endif // MAINWINDOW_H
