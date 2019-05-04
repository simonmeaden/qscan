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

#include "logger.h"

#include "scaneditor.h"

class QScan;
using Image = QSharedPointer<QImage>;

class ScanDevice;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

  void setLogTextEdit(QPlainTextEdit* log_edit);

  QToolBar* initRightToolbar();

  QToolBar* initModeToolbar();

protected:
  Log4Qt::Logger* m_logger;
  QScan* m_scan_lib;
  //  QFrame* m_main;
  QGridLayout* m_main_layout{};
  ScanEditor* m_image_editor{};
  //  QTableWidget* m_scanners{};
  QComboBox* m_mode_box{}, *m_source_box{};
  //  QPushButton* m_close_btn;
  QString m_config_dir;
  QString m_data_dir;
  QString m_lang;
  QString m_options_file;
  QString m_current_doc_name;

  QString m_selected_name;
  bool m_selected;
  QTextStream* m_log_stream{};
  QPlainTextEdit* m_empty_edit{};
  QPlainTextEdit* m_log_edit{};
  QLabel* m_min_res{}, *m_max_res{}, *m_curr_src{}, *m_curr_mode{};
  //  QSpinBox* m_res_spin{};
  QIntValidator* m_res_validator{};
  QLineEdit* m_res_edit{};
  QFrame* m_res_range{}, *m_res_list{};
  QStackedLayout* m_res_layout{};
  int m_stack_range{}, m_stack_list{};
  QComboBox* m_res_combo{};
  QComboBox* m_scanner_box{};

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

  QAction* m_scan_act{};
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
  QAction* m_set_docname_act{};
  QAction* m_doc_completed_act{};

  bool close();

  void initGui();
  void initActions();
  void initToolbar();
  void initMenu();
  void connectActions();

  void scannerSelectionChanged(int index);
  void startScanning();
  void cancelScanning();

  void modeChangeSelected(const QString& mode);
  void receiveModeChange(ScanDevice* device);

  void sourceChangeSelected(const QString& source);
  void receiveSourceChange(ScanDevice* device);

  void resolutionEdited(const QString& value);

  void scanHasFailed();
  void scanProgressed(const int&);
  void geometry();
  void modifyingSelection();
  void editorHasSelection();
  void editorHasNoSelection();
  void imageLoaded();
  void receiveOptionsSet(ScanDevice* device);
  void enableNoSelectionActions();
  void enableSelectionActions();
  void disableSelectionActions();
  void disableNoSelectionActions();
  void enableImageLoadedActions();
  void disableImageLoadedActions();
  void createDocumentName();
  void completeDocument();

  static const QString OPTIONS_FILE;
  static const QString CURRENT_DOCUMENT;
  static const QString TESSERACT;
  static const QString LANGUAGE;
  void loadOptions();
  void saveOptions();
  void loadExistingFiles();
  QToolBar* initActionToolbar();
  QToolBar* initSourceToolbar();
  QToolBar* initResourceToolbar();
};

#endif // MAINWINDOW_H
