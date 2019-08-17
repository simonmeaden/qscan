/*
  Copyright © Simon Meaden 2019.
  This file was developed as part of the Biblios application but could
  easily be used elsewhere.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
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

  QLabel* m_scanner_lbl, *m_mode_lbl, *m_res_lbl, *m_src_lbl;

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
  //  QAction* m_help_act{};
  //  QAction* m_rot_left_act{};
  //  QAction* m_rot_right_act{};
  //  QAction* m_rot_angle_act{};
  //  QAction* m_rot_edge_act{};
  //  QAction* m_copy_act{};
  //  QAction* m_crop_act{};
  //  QAction* m_scale_act{};
  QAction* m_save_act{};
  QAction* m_save_as_act{};
  //  QAction* m_zoom_in_act{};
  //  QAction* m_zoom_out_act{};
  //  QAction* m_fit_best_act{};
  //  QAction* m_fit_width_act{};
  //  QAction* m_fit_height_act{};
  QAction* m_close_act{};
  QAction* m_doc_completed_act{};

  void loadPlugins();

  bool close();

  void initGui();
  void initActions();
  void initToolbar();
  void initMenu();
  void initStatusbar();
  void makeConnections();

  void scanProgressed(const int&);
  void imageLoaded();
  //  void enableNoSelectionActions();
  //  void enableSelectionActions();
  //  void enableScanningToolbars(bool enable);
  //  void disableSelectionActions();
  //  void disableNoSelectionActions();
  void enableImageLoadedActions();
  //  void disableImageLoadedActions();
  QToolBar* initMainToolbar();
  //  QToolBar* initRightToolbar();
  void initPixmaps();
  void makeDelayedConnections();

  void setScanner(const QString& text);
  void setMode(const QString& text);
  void setSource(const QString& text);
  void setResolution(const QString& text);

  //  void enumerateMenu(QAction* act, QMenu* pareny_menu);
  //  void addMenuAction(QMap<QString, QMenu*> menus, QMap<QString, QMenu*> menu_map);


  static const QString OPTIONS_FILE;
  static const QString CURRENT_DOCUMENT;
  static const QString TESSERACT;
  static const QString LANGUAGE;

};

#endif // MAINWINDOW_H
