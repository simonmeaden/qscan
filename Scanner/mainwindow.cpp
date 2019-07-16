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
#include "mainwindow.h"

#include <opencv2/opencv.hpp>
#include <yaml-cpp/yaml.h>
#include <qyaml-cpp/QYamlCpp>

#include "iplugininterface.h"
#include "stackableframe.h"
#include "qmenuutils.h"

/*MainWindow
  =============================================================================*/
const QString MainWindow::OPTIONS_FILE = QStringLiteral("scanoptions.yaml");
const QString MainWindow::CURRENT_DOCUMENT = QStringLiteral("current document");
const QString MainWindow::TESSERACT = "tesseract";
const QString MainWindow::LANGUAGE = "language";

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_selected(false)
{
  QString home_dir = QStandardPaths::locate(
                       QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);
  QString config_path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
  QString data_dir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);

  m_config_dir = "/home/simonmeaden/.config/Biblos";
  m_data_dir = "/home/simonmeaden/.local/share/Biblos/ocr";
  m_lang = "eng";

  initPixmaps();
  initActions();
  initGui();
  initMenu();
  /* must be after initGui() else no QStackedlayout
     also after initMenus() as this creates master menus.
  */
  loadPlugins();
  makeConnections();



  //  makeDelayedConnections();
}

MainWindow::~MainWindow()
{
}

bool MainWindow::close()
{
  //  if (!m_scan_lib->isScanning()) {
  return QMainWindow::close();
  //  }

  //  auto* msg_box = new QMessageBox(QMessageBox::Warning,
  //                                  tr("Scanning"),
  //                                  tr("The scanner is still scanning. Please wait or "
  //                                     "press Cancel to cancel the scan."),
  //                                  QMessageBox::Ok | QMessageBox::Cancel,
  //                                  this);

  //  if (msg_box->exec() == QMessageBox::Cancel) {
  //    cancelScanning();
  //  }

  //  return true;
}

void MainWindow::initGui()
{
  QScreen* screen = QGuiApplication::primaryScreen();
  QSize size = screen->availableSize();
  int w = size.width() - 1200;
  int h = size.height() - 800;
  int x = int(w / 2.0);
  int y = int(h / 2.0);
  setGeometry(x, y, 1200, 1000);
  QFrame* main_frame = new QFrame(this);
  setCentralWidget(main_frame);
  m_main_layout = new QStackedLayout;
  main_frame->setLayout(m_main_layout);

  initToolbar();
}

QToolBar* MainWindow::initMainToolbar()
{
  QToolBar* toolbar = addToolBar("action");
  addToolBar(Qt::TopToolBarArea, toolbar);
  toolbar->setMovable(true);
  toolbar->addAction(m_save_act);
  toolbar->addAction(m_save_as_act);
  toolbar->addSeparator();
  //  toolbar->addAction(m_scan_act);
  toolbar->addSeparator();
  toolbar->addAction(m_zoom_in_act);
  toolbar->addAction(m_zoom_out_act);

  QWidget* spacer_widget = new QWidget(this);
  spacer_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  spacer_widget->setVisible(true);
  toolbar->addWidget(spacer_widget);

  toolbar->addSeparator();
  toolbar->addAction(m_help_act);
  toolbar->addSeparator();
  toolbar->addAction(m_close_act);

  return toolbar;
}


QToolBar* MainWindow::initRightToolbar()
{
  QToolBar* toolbar = addToolBar("Right");
  addToolBar(Qt::RightToolBarArea, toolbar);
  toolbar->setOrientation(Qt::Vertical);
  toolbar->setMovable(true);
  toolbar->addAction(m_copy_act);
  toolbar->addAction(m_crop_act);
  toolbar->addAction(m_scale_act);
  toolbar->addSeparator();
  toolbar->addAction(m_rot_left_act);
  toolbar->addAction(m_rot_right_act);
  toolbar->addAction(m_rot_angle_act);
  toolbar->addAction(m_rot_edge_act);
  toolbar->addSeparator();
  toolbar->addAction(m_fit_best_act);
  toolbar->addAction(m_fit_width_act);
  toolbar->addAction(m_fit_height_act);
  //  spacer_widget = new QWidget(this);
  //  spacer_widget->setSizePolicy(QSizePolicy::Expanding,
  //  QSizePolicy::Preferred); spacer_widget->setVisible(true);
  //  toolbar3->addWidget(spacer_widget);
  toolbar->setMovable(false);
  return toolbar;
}

void MainWindow::loadPlugins()
{
  QDir plugins_dir = QDir(qApp->applicationDirPath());
  plugins_dir.cd("../plugins");
  QList<QString> loaded_plugins;

  foreach (QString filename, plugins_dir.entryList(QDir::Files)) {
    if (filename == "Makefile"  || filename.endsWith(".o") ||
        filename.endsWith(".cpp") || filename.endsWith(".h")) { // can remove this in installed versions.
      continue;
    }

    // This bit is only used in a unix type OS's where you end
    // up with miltiple links to the same file.
    // ie. libname.so, libname.so.1, libname.so.1.0, libname.so.1.0.0
    // otherwise you get the same plugin loaded 4 times.
    QFileInfo info(filename);
    QString basename = info.baseName();

    if (loaded_plugins.contains(basename)) {
      continue;
    }

    loaded_plugins.append(basename);

    QPluginLoader loader(plugins_dir.absoluteFilePath(filename));
    QObject* plugin = loader.instance();

    if (plugin) {
      IPluginInterface* plugins = dynamic_cast<IPluginInterface*>(plugin);

      if (plugins) {
        QList<StackableFrame*> frames = plugins->editors(centralWidget());

        if (!frames.isEmpty()) {
          for (auto* frame : frames) {
            int id = m_main_layout->addWidget(frame);
            frame->setId(id);
          }
        }

        QList<QMenu*> menus = plugins->menus();

        if (!menus.isEmpty()) {
          QMenuUtils::mergeMenus(menus, m_menus);
        }

        menuBar()->clear();

        for (QMenu* menu : m_menus) {
          menuBar()->addMenu(menu);
        }

        //        QList<QToolBar*> toolbars = plugins->toolbars();

        //        if (!toolbars.isEmpty()) {
        //          for (auto* toolbar : toolbars) {
        //            addToolBar(toolbar);
        //          }
        //        }

        // TODO add in toolbar & menu

      } else {
        qDebug() << tr("Plugin error : %1").arg(loader.errorString());
      }
    }
  }
}

//QToolBar* MainWindow::initResolutionToolbar()
//{
//  m_res_bar = addToolBar("mode bar");

//  auto* stack_frame = new QFrame(this);
//  m_res_layout = new QStackedLayout;
//  stack_frame->setLayout(m_res_layout);

//  {
//    m_res_range = new QFrame(this);
//    auto* range_layout = new QGridLayout;
//    m_res_range->setLayout(range_layout);
//    /*===*/
//    auto* lbl = new QLabel(tr("Min Resolution :"), this);
//    lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    range_layout->addWidget(lbl, 0, 0);

//    QString s(tr("%1 %2"));
//    m_min_res = new QLabel(s.arg(1).arg("dpi"), this);
//    m_min_res->setAlignment(Qt::AlignLeft);
//    m_min_res->setContentsMargins(0, 0, 0, 0);
//    m_min_res->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    range_layout->addWidget(m_min_res, 0, 1);
//    /*===*/
//    lbl = new QLabel(tr("Resolution :"), this);
//    lbl->setContentsMargins(30, 0, 0, 0);
//    lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    range_layout->addWidget(lbl, 0, 2);

//    // the edit lineedit will only accept integers between 100 and 999
//    m_res_edit = new QLineEdit(this);
//    m_res_edit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    m_res_validator = new QIntValidator(1, 1, this);
//    m_res_edit->setValidator(m_res_validator);
//    m_res_edit->setText("1");
//    range_layout->addWidget(m_res_edit, 0, 3);
//    lbl = new QLabel("dpi", this);
//    lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    range_layout->addWidget(lbl, 0, 4);

//    /*===*/
//    lbl = new QLabel("Max Resolution :", this);
//    lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    range_layout->addWidget(lbl, 0, 5);

//    m_max_res = new QLabel(s.arg(1).arg("dpi"), this);
//    m_max_res->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    m_max_res->setAlignment(Qt::AlignLeft);
//    m_max_res->setContentsMargins(0, 0, 0, 0);
//    range_layout->addWidget(m_max_res, 0, 6);

//    m_stack_range_id = m_res_layout->addWidget(m_res_range);
//  }
//  {
//    m_res_list = new QFrame(this);
//    m_res_list->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    auto* list_layout = new QGridLayout;
//    m_res_list->setLayout(list_layout);
//    m_res_list->setVisible(true);

//    auto* lbl = new QLabel(tr("Resolution :"), this);
//    lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    list_layout->addWidget(lbl, 0, 0);

//    m_res_combo = new QComboBox(this);
//    m_res_combo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    m_res_combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
//    list_layout->addWidget(m_res_combo, 0, 1);

//    auto* spacer_widget = new QWidget(this);
//    spacer_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
//    list_layout->addWidget(spacer_widget, 0, 2);

//    m_stack_list_id = m_res_layout->addWidget(m_res_list);
//  }
//  m_res_bar->addWidget(stack_frame);
//  return m_res_bar;
//}

void MainWindow::initToolbar()
{
  initMainToolbar();
  initRightToolbar();
}

void MainWindow::initMenu()
{
  QString menu_name = tr("&File");
  QMenu* file_menu = menuBar()->addMenu(menu_name);
  file_menu->addAction(m_close_act);
  m_menus.append(file_menu);
}

void MainWindow::initActions()
{
  QPixmap scan_icon, rot_left_icon, rot_right_icon, rot_angle_icon, rot_edge_icon, copy_icon,
          scale_icon, crop_icon, close_icon, save_icon, save_as_icon, zoom_in_icon, zoom_out_icon,
          fit_best_icon, fit_width_icon, fit_height_icon, help_icon;
  QPixmapCache::find(help_key, &help_icon);
  //  QPixmapCache::find(scan_key, &scan_icon);
  QPixmapCache::find(rot_left_key, &rot_left_icon);
  QPixmapCache::find(rot_right_key, &rot_right_icon);
  QPixmapCache::find(rot_angle_key, &rot_angle_icon);
  QPixmapCache::find(rot_edge_key, &rot_edge_icon);
  QPixmapCache::find(copy_key, &copy_icon);
  QPixmapCache::find(scale_key, &scale_icon);
  QPixmapCache::find(crop_key, &crop_icon);
  QPixmapCache::find(save_key, &save_icon);
  QPixmapCache::find(save_as_key, &save_as_icon);
  QPixmapCache::find(zoom_in_key, &zoom_in_icon);
  QPixmapCache::find(zoom_out_key, &zoom_out_icon);
  QPixmapCache::find(fit_best_key, &fit_best_icon);
  QPixmapCache::find(fit_width_key, &fit_width_icon);
  QPixmapCache::find(fit_height_key, &fit_height_icon);
  QPixmapCache::find(QPixmapCache::insert(QPixmap(":/qscan_icons/close")), &close_icon);

  m_close_act = new QAction(close_icon, tr("Close Application"), this);
  m_help_act = new QAction(QIcon(help_icon), tr("Help"), this);
  //  m_scan_act = new QAction(QIcon(scan_icon), tr("Scan"), this);
  //  m_scan_act->setEnabled(false);
  m_rot_left_act = new QAction(QIcon(rot_left_icon), tr("Rotate Anti-clockwise"), this);
  m_rot_right_act = new QAction(QIcon(rot_right_icon), tr("Rotate Clockwise"), this);
  m_rot_angle_act = new QAction(QIcon(rot_angle_icon), tr("Rotate by angle"), this);
  m_rot_edge_act = new QAction(QIcon(rot_edge_icon), tr("Rotate by defined edge"), this);
  m_copy_act = new QAction(QIcon(copy_icon), tr("Copy selection to clipboard"), this);
  m_scale_act = new QAction(QIcon(scale_icon), tr("Scale image"), this);
  m_save_act = new QAction(QIcon(save_icon), tr("Save image"), this);
  m_save_as_act = new QAction(QIcon(save_as_icon), tr("save as image"), this);
  m_crop_act = new QAction(QIcon(crop_icon), tr("Crop to selection"), this);
  m_zoom_in_act = new QAction(QIcon(zoom_in_icon), tr("Zoom in"), this);
  m_zoom_out_act = new QAction(QIcon(zoom_out_icon), tr("zoom out"), this);
  m_fit_best_act = new QAction(QIcon(fit_best_icon), tr("Best fit"), this);
  m_fit_width_act = new QAction(QIcon(fit_width_icon), tr("Fit to width"), this);
  m_fit_height_act = new QAction(QIcon(fit_height_icon), tr("Fit to feight"), this);
  m_doc_completed_act = new QAction(tr("Document completed."), this);
  m_doc_completed_act->setToolTip(tr("This completes the document and prepares to start another."));

  disableImageLoadedActions();
  disableSelectionActions();
  disableNoSelectionActions();
}

void MainWindow::initPixmaps()
{
  help_key = QPixmapCache::insert(QPixmap(":/qscan_icons/help-contents"));
  rot_left_key = QPixmapCache::insert(QPixmap(":/qscan_icons/rotate-left"));
  rot_right_key = QPixmapCache::insert(QPixmap(":/qscan_icons/rotate-right"));
  rot_angle_key = QPixmapCache::insert(QPixmap(":/qscan_icons/rotate-angle"));
  rot_edge_key = QPixmapCache::insert(QPixmap(":/qscan_icons/rotate-edge"));
  copy_key = QPixmapCache::insert(QPixmap(":/qscan_icons/copy"));
  scale_key = QPixmapCache::insert(QPixmap(":/qscan_icons/scale"));
  crop_key = QPixmapCache::insert(QPixmap(":/qscan_icons/crop"));
  save_key = QPixmapCache::insert(QPixmap(":/qscan_icons/save"));
  save_as_key = QPixmapCache::insert(QPixmap(":/qscan_icons/save-as"));
  //  close_key = QPixmapCache::insert(QPixmap(":/qscan_icons/close"));
  zoom_in_key = QPixmapCache::insert(QPixmap(":/qscan_icons/zoom-in"));
  zoom_out_key = QPixmapCache::insert(QPixmap(":/qscan_icons/zoom-out"));
  fit_best_key = QPixmapCache::insert(QPixmap(":/qscan_icons/fit-best"));
  fit_width_key = QPixmapCache::insert(QPixmap(":/qscan_icons/fit-width"));
  fit_height_key = QPixmapCache::insert(QPixmap(":/qscan_icons/fit-height"));
}

void MainWindow::makeConnections()
{
  connect(m_close_act, &QAction::triggered, this, &MainWindow::close);
  //  connect(m_scan_act, &QAction::triggered, this, &MainWindow::startScanning);
  //  connect(m_rot_left_act, &QAction::triggered, m_image_editor, &ScanEditor::rotateACW);
  //  connect(m_rot_right_act, &QAction::triggered, m_image_editor, &ScanEditor::rotateCW);
  //  connect(m_rot_angle_act, &QAction::triggered, m_image_editor, &ScanEditor::rotateByAngle);
  //  connect(m_rot_edge_act, &QAction::triggered, m_image_editor, &ScanEditor::rotateByEdge);
  //  connect(m_copy_act, &QAction::triggered, m_image_editor, &ScanEditor::copySelection);
  //  connect(m_scale_act, &QAction::triggered, m_image_editor, &ScanEditor::scale);
  //  connect(m_crop_act, &QAction::triggered, m_image_editor, &ScanEditor::cropToSelection);
  //  connect(m_save_act, &QAction::triggered, m_image_editor, &ScanEditor::save);
  //  connect(m_save_as_act, &QAction::triggered, m_image_editor, &ScanEditor::saveAs);
  //  connect(m_zoom_in_act, &QAction::triggered, m_image_editor, &ScanEditor::zoomIn);
  //  connect(m_zoom_out_act, &QAction::triggered, m_image_editor, &ScanEditor::zoomOut);
  //  connect(m_fit_best_act, &QAction::triggered, m_image_editor, &ScanEditor::fitBest);
  //  connect(m_fit_width_act, &QAction::triggered, m_image_editor, &ScanEditor::fitWidth);
  //  connect(m_fit_height_act, &QAction::triggered, m_image_editor, &ScanEditor::fitHeight);
  //  connect(m_set_docname_act, &QAction::triggered, this, &MainWindow::createDocumentName);
  //  connect(m_doc_completed_act, &QAction::triggered, this, &MainWindow::completeDocument);

}

//void MainWindow::makeDelayedConnections()
//{
// These have been delayed until scanners have been properly detected.
//  connect(m_scanner_box,
//          qOverload<int>(&QComboBox::currentIndexChanged),
//          this,
//          &MainWindow::scannerSelectionChanged);
//  connect(m_mode_box,
//          &QComboBox::currentTextChanged,
//          this,
//          &MainWindow::modeChangeSelected);
//  connect(
//    m_scan_lib, &QScan::scanCompleted, m_image_editor, &ScanEditor::setImage);
//  connect(m_scan_lib,
//          &QScan::scanProgress,
//          m_image_editor,
//          &ScanEditor::setScanProgress);
//  connect(m_scan_lib, &QScan::scanFailed, this, &MainWindow::scanHasFailed);

//  connect(m_image_editor,
//          &ScanEditor::selectionUnderway,
//          this,
//          &MainWindow::modifyingSelection);
//  connect(m_image_editor,
//          &ScanEditor::selected,
//          this,
//          &MainWindow::editorHasSelection);
//  connect(
//    m_image_editor, &ScanEditor::imageIsLoaded, this, &MainWindow::imageLoaded);
//  connect(m_image_editor,
//          &ScanEditor::unselected,
//          this,
//          &MainWindow::editorHasNoSelection);
//}

//void MainWindow::scannerSelectionChanged(int index)
//{
//  if (!m_scan_lib->devices().isEmpty()) {
//    QString name = m_scan_lib->devices().at(index);

//    if (!name.isEmpty()) {
//      if (m_scan_lib->openDevice(name)) {
//        m_selected_name = name;
//        m_scan_act->setEnabled(true);

//      } else {
//        qDebug() << tr("Unable to open %1").arg(m_selected_name);
//      }
//    }
//  }
//}

//void MainWindow::startScanning()
//{
//  m_scan_lib->startScanning(m_selected_name);
//}

//void MainWindow::cancelScanning()
//{
//  m_scan_lib->cancelScan(/*m_selected_name*/);
//}

//void MainWindow::scanHasFailed()
//{
//  QMessageBox::warning(this,
//                       tr("Scan failed"),
//                       tr("The scanner returned with a Scan Failed message."),
//                       QMessageBox::Ok);
//}

void MainWindow::scanProgressed(const int&)
{}

// void MainWindow::geometry()
//{}

//void MainWindow::receiveOptionsSet(ScanDevice* device)
//{
//  // These objects are having their values set so we don't want to trigger a changed signal.
//  disconnect(m_mode_box, &QComboBox::currentTextChanged, this, &MainWindow::modeChangeSelected);
//  disconnect(m_source_box, &QComboBox::currentTextChanged, this, &MainWindow::sourceChangeSelected);
//  disconnect(m_res_edit, &QLineEdit::textChanged, this, &MainWindow::resolutionEdited);
//  disconnect(m_res_combo, &QComboBox::currentTextChanged, this, &MainWindow::resolutionEdited);

//  ScanOptions* options = device->options;
//  m_mode_box->clear();
//  QStringList list = options->modes();
//  m_mode_box->addItems(list);
//  m_source_box->clear();
//  list = options->sources();
//  m_source_box->addItems(list);
//  QVariant v = options->resolutionRange();

//  if (v.canConvert<ScanRange>()) {
//    ScanRange range = v.value<ScanRange>();
//    ScanUnits units = options->units();
//    QString u = (units == ScanUnits::DPI ? "dpi" : "mm");
//    int res = options->resolution();
//    QString s = QString("%1 %2");
//    m_min_res->setText(s.arg(range.min).arg(u));
//    m_max_res->setText(s.arg(range.max).arg(u));
//    m_res_validator->setBottom(range.min);
//    m_res_validator->setTop(range.max);
//    m_res_edit->setText(QString::number(res));
//    m_res_layout->setCurrentIndex(m_stack_range_id);

//  } else if (v.canConvert<QList<int>>()) {
//    m_res_combo->clear();

//    for (int value : v.value<QList<int>>()) {
//      m_res_combo->addItem(QString::number(value));
//    }

//    m_res_layout->setCurrentIndex(m_stack_list_id);
//  }

//  m_curr_src->setText(options->source());
//  m_source_box->setCurrentText(options->source());
//  m_curr_mode->setText(options->mode());
//  m_mode_box->setCurrentText(options->mode());

//  // reconnect the changed signals
//  connect(m_mode_box, &QComboBox::currentTextChanged, this, &MainWindow::modeChangeSelected);
//  connect(m_source_box, &QComboBox::currentTextChanged, this, &MainWindow::sourceChangeSelected);
//  connect(m_res_edit, &QLineEdit::textChanged, this, &MainWindow::resolutionEdited);
//  connect(m_res_combo, &QComboBox::currentTextChanged, this, &MainWindow::resolutionEdited);
//}

//void MainWindow::receiveModeChange(ScanDevice* device)
//{
//  m_curr_mode->setText(device->options->mode());
//}

//void MainWindow::receiveSourceChange(ScanDevice* device)
//{
//  m_curr_src->setText(device->options->source());
//}

//void MainWindow::receiveEditingImage()
//{
//  enableScanningToolbars(false);
//}

//void MainWindow::resolutionEdited(const QString& value)
//{
//  ScanDevice* device = m_scan_lib->device(m_selected_name);
//  m_scan_lib->setResolution(device, value.toInt());
//}

//void MainWindow::modeChangeSelected(const QString& mode)
//{
//  ScanDevice* device = m_scan_lib->device(m_selected_name);
//  m_scan_lib->setScanMode(device, mode);
//}

//void MainWindow::sourceChangeSelected(const QString& source)
//{
//  ScanDevice* device = m_scan_lib->device(m_selected_name);
//  m_scan_lib->setSource(device, source);
//}

//void MainWindow::modifyingSelection()
//{
//  disableSelectionActions();
//}

//void MainWindow::editorHasSelection()
//{
//  enableSelectionActions();
//  disableNoSelectionActions();
//}

//void MainWindow::editorHasNoSelection()
//{
//  disableSelectionActions();
//  enableNoSelectionActions();
//}

void MainWindow::imageLoaded()
{
  enableImageLoadedActions();
  enableNoSelectionActions();
  disableSelectionActions();
}

void MainWindow::disableNoSelectionActions()
{
  m_rot_left_act->setEnabled(false);
  m_rot_right_act->setEnabled(false);
  m_rot_angle_act->setEnabled(false);
  m_rot_edge_act->setEnabled(false);
  m_scale_act->setEnabled(false);
}

void MainWindow::enableNoSelectionActions()
{
  m_rot_left_act->setEnabled(true);
  m_rot_right_act->setEnabled(true);
  m_rot_angle_act->setEnabled(true);
  m_rot_edge_act->setEnabled(true);
  m_scale_act->setEnabled(true);
}

void MainWindow::enableSelectionActions()
{
  m_crop_act->setEnabled(true);
  m_copy_act->setEnabled(true);
}

void MainWindow::enableScanningToolbars(bool enable)
{
  //  if (enable) {
  //    removeToolBar(m_source_bar);
  //    removeToolBar(m_mode_bar);
  //    removeToolBar(m_res_bar);

  //  } else {
  //    addToolBar(m_source_bar);
  //    addToolBar(m_mode_bar);
  //    addToolBar(m_res_bar);
  //  }
}

void MainWindow::disableSelectionActions()
{
  m_crop_act->setEnabled(false);
  m_copy_act->setEnabled(false);
}

void MainWindow::enableImageLoadedActions()
{
  m_save_act->setEnabled(true);
  m_save_as_act->setEnabled(true);
  m_zoom_in_act->setEnabled(true);
  m_zoom_out_act->setEnabled(true);
  m_fit_best_act->setEnabled(true);
  m_fit_width_act->setEnabled(true);
  m_fit_height_act->setEnabled(true);
}

void MainWindow::disableImageLoadedActions()
{
  m_save_act->setEnabled(false);
  m_save_as_act->setEnabled(false);
  m_zoom_in_act->setEnabled(false);
  m_zoom_out_act->setEnabled(false);
  m_fit_best_act->setEnabled(false);
  m_fit_width_act->setEnabled(false);
  m_fit_height_act->setEnabled(false);
}

///*
//  Gets the document name or renames it if it exists. Pops up a dialog
//  asking for the document name if it doesn't exist, otherwise it
//  asks if you are sure, then if you are it asks for the new document
//  name, renaming the document directory if necessary.
//*/
//void MainWindow::createDocumentName()
//{
//  QString doc_name;
//  QString old_doc_name = m_image_editor->documentName();

//  if (old_doc_name.isEmpty()) {
//    doc_name = QInputDialog::getText(this,
//                                     tr("Get Document Name"),
//                                     tr("Please supply a name for this document.\n"
//                                        "Do not add file extension as this is\n"
//                                        "created internally."));

//    if (!doc_name.isEmpty()) {
//      QDir dir;
//      QString doc_path = m_data_dir + QDir::separator() + doc_name;
//      dir.mkpath(doc_path);
//      m_image_editor->setDocumentName(doc_name);
//      m_current_doc_name = doc_name;
//      setWindowTitle(doc_name);
//    }

//  } else {
//    int btn = QMessageBox::warning(this,
//                                   tr("Document name already set."),
//                                   tr("The document already has a name (%1).\n"
//                                      "This will cause the document to be renamed.\n"
//                                      "Are you sure!\n"
//                                      "Press 'OK' to continue or 'Cancel' to stop.")
//                                   .arg(old_doc_name),
//                                   QMessageBox::Ok | QMessageBox::Cancel,
//                                   QMessageBox::Cancel);

//    if (btn == QMessageBox::Ok) {
//      doc_name = QInputDialog::getText(this,
//                                       tr("Get Document Name"),
//                                       tr("Please supply a name for this document.\n"
//                                          "Do not add file extension as this is\n"
//                                          "created internally."));

//      if (!doc_name.isEmpty()) {
//        if (doc_name != old_doc_name) {
//          QDir dir;
//          QString old_doc_path = m_data_dir + QDir::separator() + old_doc_name;
//          QString doc_path = m_data_dir + QDir::separator() + doc_name;

//          if (dir.exists(old_doc_path)) {
//            dir.rename(old_doc_path, doc_path);

//          } else {
//            dir.mkpath(doc_path);
//          }

//          m_image_editor->setDocumentName(doc_name);
//          m_current_doc_name = doc_name;
//          setWindowTitle(doc_name);
//        }
//      }
//    }
//  }
//}

//void MainWindow::completeDocument()
//{
//  m_image_editor->completeDocument();
//}
