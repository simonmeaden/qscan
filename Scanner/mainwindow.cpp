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

#include "qscan.h"
#include "scaninterface.h"

/*TextEditIoDevice
  =============================================================================*/
TextEditIoDevice::TextEditIoDevice(QPlainTextEdit* text_edit, QObject* parent)
  : QIODevice(parent)
  , m_text_edit(text_edit)
{
  open(QIODevice::WriteOnly | QIODevice::Text);
}

void
TextEditIoDevice::setTextEdit(QPlainTextEdit* text_edit)
{
  m_text_edit = text_edit;

  if (!m_pre_data.isEmpty()) {
    m_text_edit->setPlainText(m_pre_data);
    m_pre_data.clear();
  }
}

qint64
TextEditIoDevice::readData(char*, qint64)
{
  return 0;
}

qint64
TextEditIoDevice::writeData(const char* data, qint64 maxSize)
{
  if (m_text_edit) {
    QString d(data);
    d = d.trimmed();

    if (d.endsWith("\n")) {
      d = d.left(d.length() - 1);
    }

    m_text_edit->appendPlainText(d);

  } else {
    m_pre_data = QString(data);
  }

  return maxSize;
}

/*MainWindow
  =============================================================================*/

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_selected(false)
{
  m_logger = Log4Qt::Logger::logger(tr("Scanner"));
  m_scan_lib = new QScan(this);

  scan_key = QPixmapCache::insert(QPixmap(":/icons/scan"));
  rot_left_key = QPixmapCache::insert(QPixmap(":/icons/rotate-left"));
  rot_right_key = QPixmapCache::insert(QPixmap(":/icons/rotate-right"));
  rot_angle_key = QPixmapCache::insert(QPixmap(":/icons/rotate-angle"));
  rot_edge_key = QPixmapCache::insert(QPixmap(":/icons/rotate-edge"));
  copy_key = QPixmapCache::insert(QPixmap(":/icons/copy"));
  scale_key = QPixmapCache::insert(QPixmap(":/icons/scale"));
  crop_key = QPixmapCache::insert(QPixmap(":/icons/crop"));
  save_key = QPixmapCache::insert(QPixmap(":/icons/save"));
  save_as_key = QPixmapCache::insert(QPixmap(":/icons/save-as"));
  close_key = QPixmapCache::insert(QPixmap(":/icons/close"));
  zoom_in_key = QPixmapCache::insert(QPixmap(":/icons/zoom-in"));
  zoom_out_key = QPixmapCache::insert(QPixmap(":/icons/zoom-out"));
  fit_best_key = QPixmapCache::insert(QPixmap(":/icons/fit-best"));
  fit_width_key = QPixmapCache::insert(QPixmap(":/icons/fit-width"));
  fit_height_key = QPixmapCache::insert(QPixmap(":/icons/fit-height"));

  m_config_dir = "/home/simon/.config/Biblos";
  m_lang = "eng";

  initActions();
  initGui();
  connectActions();

  connect(
    m_scan_lib, &QScan::scanCompleted, m_image_editor, &ScanEditor::setImage);
  connect(m_scan_lib,
          &QScan::scanProgress,
          m_image_editor,
          &ScanEditor::setScanProgress);
  connect(m_scan_lib, &QScan::scanFailed, this, &MainWindow::scanHasFailed);
  connect(m_scan_lib, &QScan::optionsSet, this, &MainWindow::receiveOptionsSet);
  connect(
    m_scan_lib, &QScan::modeChanged, this, &MainWindow::receiveModeChange);
  connect(
    m_scan_lib, &QScan::sourceChanged, this, &MainWindow::receiveSourceChange);

  connect(m_image_editor,
          &ScanEditor::selectionUnderway,
          this,
          &MainWindow::modifyingSelection);
  connect(m_image_editor,
          &ScanEditor::selected,
          this,
          &MainWindow::editorHasSelection);
  connect(
    m_image_editor, &ScanEditor::imageIsLoaded, this, &MainWindow::imageLoaded);
  connect(m_image_editor,
          &ScanEditor::unselected,
          this,
          &MainWindow::editorHasNoSelection);
  m_scan_lib->init();
  QStringList scanners = m_scan_lib->devices();

  for (int i = 0; i < scanners.size(); i++) {
    ScanDevice* s = m_scan_lib->device(scanners.at(i));
    int row = m_scanners->rowCount();
    m_scanners->insertRow(row);
    auto* name_item = new QTableWidgetItem(s->name);
    name_item->setFlags(Qt::ItemIsEnabled);
    auto* vendor_item = new QTableWidgetItem(s->vendor);
    vendor_item->setFlags(Qt::ItemIsEnabled);
    auto* modelitem = new QTableWidgetItem(s->model);
    modelitem->setFlags(Qt::ItemIsEnabled);
    auto* type_item = new QTableWidgetItem(s->type);
    type_item->setFlags(Qt::ItemIsEnabled);
    m_scanners->setItem(row, 0, name_item);
    m_scanners->setItem(row, 1, vendor_item);
    m_scanners->setItem(row, 2, modelitem);
    m_scanners->setItem(row, 3, type_item);
  }
}

MainWindow::~MainWindow() = default;

void
MainWindow::setLogTextEdit(QPlainTextEdit* log_edit)
{
  m_log_edit = log_edit;
  m_main_layout->replaceWidget(m_empty_edit, m_log_edit);
  m_empty_edit->deleteLater();
}

bool
MainWindow::close()
{
  if (!m_scan_lib->isScanning()) {
    return QMainWindow::close();
  }

  auto* msg_box =
    new QMessageBox(QMessageBox::Warning,
                    tr("Scanning"),
                    tr("The scanner is still scanning. Please wait or "
                       "press Cancel to cancel the scan."),
                    QMessageBox::Ok | QMessageBox::Cancel,
                    this);
  if (msg_box->exec() == QMessageBox::Cancel) {
    cancelScanning();
  }

  return true;
}

void
MainWindow::initToolbar()
{
  QToolBar* toolbar = addToolBar(tr("Edit"));
  toolbar->setMovable(false);
  toolbar->addAction(m_save_act);
  toolbar->addAction(m_save_as_act);
  toolbar->addSeparator();
  toolbar->addAction(m_scan_act);
  toolbar->addSeparator();
  toolbar->addAction(m_copy_act);
  toolbar->addAction(m_crop_act);
  toolbar->addAction(m_scale_act);
  toolbar->addSeparator();
  toolbar->addAction(m_rot_left_act);
  toolbar->addAction(m_rot_right_act);
  toolbar->addAction(m_rot_angle_act);
  toolbar->addAction(m_rot_edge_act);
  toolbar->addSeparator();
  toolbar->addAction(m_zoom_in_act);
  toolbar->addAction(m_zoom_out_act);
  toolbar->addAction(m_fit_best_act);
  toolbar->addAction(m_fit_width_act);
  toolbar->addAction(m_fit_height_act);

  QWidget* spacer_widget = new QWidget(this);
  spacer_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  spacer_widget->setVisible(true);
  toolbar->addWidget(spacer_widget);

  toolbar->addSeparator();
  toolbar->addAction(m_close_act);
}

QFrame*
MainWindow::initModeFrame()
{
  int col = 0;
  QFrame* mode_frame = new QFrame(this);
  auto* mode_layout = new QGridLayout;
  mode_frame->setLayout(mode_layout);

  QLabel* lbl = new QLabel(tr("Mode :"), this);
  lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_main_layout->addWidget(lbl, 1, 3);
  mode_layout->addWidget(lbl, 0, col++);

  m_mode_box = new QComboBox(this);
  m_mode_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  mode_layout->addWidget(m_mode_box, 0, col++);

  lbl = new QLabel(tr("Current Mode :"), this);
  lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  mode_layout->addWidget(lbl, 0, col++);

  m_curr_mode = new QLabel(tr("No Mode"), this);
  m_curr_mode->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  mode_layout->addWidget(m_curr_mode, 0, col);

  return mode_frame;
}

QFrame*
MainWindow::initSourceFrame()
{
  int col = 0;
  QFrame* source_frame = new QFrame(this);
  auto* source_layout = new QGridLayout;
  source_frame->setLayout(source_layout);

  QLabel* lbl = new QLabel(tr("Source :"), this);
  lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  source_layout->addWidget(lbl, 0, col++);

  m_source_box = new QComboBox(this);
  m_source_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  source_layout->addWidget(m_source_box, 0, col++);

  lbl = new QLabel(tr("Current Source :"), this);
  lbl->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  source_layout->addWidget(lbl, 0, col++);

  m_curr_src = new QLabel(tr("No Source"), this);
  m_curr_src->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  source_layout->addWidget(m_curr_src, 0, col++);

  return source_frame;
}

QFrame*
MainWindow::initResolutionFrame()
{
  int col = 0;
  QFrame* res_frame = new QFrame(this);
  auto* res_layout = new QGridLayout;
  res_frame->setLayout(res_layout);

  QLabel* lbl = new QLabel("Min Resolution :", this);
  res_layout->addWidget(lbl, 0, col++);
  m_min_res = new QLabel(this);
  m_min_res->setNum(1);
  res_layout->addWidget(m_min_res, 0, col++);
  lbl = new QLabel("Resolution :", this);
  res_layout->addWidget(lbl, 0, col++);
  m_res_box = new QSpinBox(this);
  m_res_box->setMinimum(1);
  m_res_box->setMaximum(1);
  res_layout->addWidget(m_res_box, 0, col++);
  lbl = new QLabel("dpi", this);
  res_layout->addWidget(lbl, 0, col++, Qt::AlignLeft);
  lbl = new QLabel("Max Resolution :", this);
  res_layout->addWidget(lbl, 0, col++);
  m_max_res = new QLabel(this);
  m_max_res->setNum(1);
  res_layout->addWidget(m_max_res, 0, col);

  return res_frame;
}

void
MainWindow::initGui()
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
  m_main_layout = new QGridLayout;
  main_frame->setLayout(m_main_layout);

  m_image_editor = new ScanEditor(m_scan_lib, m_config_dir, m_lang, this);
  m_image_editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  installEventFilter(m_image_editor);
  connect(m_image_editor,
          &ScanEditor::scanCancelled,
          this,
          &MainWindow::cancelScanning);

  int row = 0;

  // Source Scanner list.
  QStringList labels;
  labels << "Name"
         << "Vendor"
         << "Model"
         << "Type";
  m_scanners = new QTableWidget(this);
  m_scanners->setHorizontalHeaderLabels(labels);
  m_scanners->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeToContents);
  m_scanners->horizontalHeader()->setStretchLastSection(true);
  m_scanners->setColumnCount(4);
  m_scanners->setSelectionMode(QAbstractItemView::NoSelection);
  m_scanners->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_scanners->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  connect(m_scanners,
          &QTableWidget::clicked,
          this,
          &MainWindow::scannerSelectionChanged);
  connect(
    m_scanners, &QTableWidget::doubleClicked, this, &MainWindow::doubleClicked);
  m_main_layout->addWidget(m_scanners, row++, 1, 1, 4);

  // Scanner source info
  m_main_layout->addWidget(initSourceFrame(), row++, 1);
  // Scanner mode info
  m_main_layout->addWidget(initModeFrame(), row++, 1);
  // scanner resolution info
  m_main_layout->addWidget(initResolutionFrame(), row++, 1);

  // logger editor
  m_empty_edit = new QPlainTextEdit(this);
  m_empty_edit->setReadOnly(true);
  m_empty_edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_main_layout->addWidget(m_empty_edit, row, 1);

  m_main_layout->addWidget(m_image_editor, 0, 0, row + 1, 1);
  m_main_layout->setColumnStretch(0, 30);
  m_main_layout->setColumnStretch(1, 10);

  initToolbar();
}

void
MainWindow::initActions()
{
  QPixmap scan_icon, rot_left_icon, rot_right_icon, rot_angle_icon,
    rot_edge_icon, copy_icon, scale_icon, crop_icon, close_icon, save_icon,
    save_as_icon, zoom_in_icon, zoom_out_icon, fit_best_icon, fit_width_icon,
    fit_height_icon;
  QPixmapCache::find(scan_key, &scan_icon);
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
  QPixmapCache::find(close_key, &close_icon);

  m_close_act = new QAction(QIcon(close_icon), tr("Close Application"), this);
  m_scan_act = new QAction(QIcon(scan_icon), tr("Scan"), this);
  m_scan_act->setEnabled(false);
  m_rot_left_act =
    new QAction(QIcon(rot_left_icon), tr("Rotate Anti-clockwise"), this);
  m_rot_right_act =
    new QAction(QIcon(rot_right_icon), tr("Rotate Clockwise"), this);
  m_rot_angle_act =
    new QAction(QIcon(rot_angle_icon), tr("Rotate by angle"), this);
  m_rot_edge_act =
    new QAction(QIcon(rot_edge_icon), tr("Rotate by defined edge"), this);
  m_copy_act =
    new QAction(QIcon(copy_icon), tr("Copy selection to clipboard"), this);
  m_scale_act = new QAction(QIcon(scale_icon), tr("Scale image"), this);
  m_save_act = new QAction(QIcon(save_icon), tr("Save image"), this);
  m_save_as_act = new QAction(QIcon(save_as_icon), tr("save as image"), this);
  m_crop_act = new QAction(QIcon(crop_icon), tr("Crop to selection"), this);
  m_zoom_in_act = new QAction(QIcon(zoom_in_icon), tr("Zoom in"), this);
  m_zoom_out_act = new QAction(QIcon(zoom_out_icon), tr("zoom out"), this);
  m_fit_best_act = new QAction(QIcon(fit_best_icon), tr("Best fit"), this);
  m_fit_width_act =
    new QAction(QIcon(fit_width_icon), tr("Fit to width"), this);
  m_fit_height_act =
    new QAction(QIcon(fit_height_icon), tr("Fit to feight"), this);

  disableImageLoadedBtns();
  disableSelectionBtns();
  disableNoSelectionBtns();
}

void
MainWindow::connectActions()
{
  connect(m_close_act, &QAction::triggered, this, &MainWindow::close);
  connect(m_scan_act, &QAction::triggered, this, &MainWindow::startScanning);
  connect(m_rot_left_act,
          &QAction::triggered,
          m_image_editor,
          &ScanEditor::rotateACW);
  connect(m_rot_right_act,
          &QAction::triggered,
          m_image_editor,
          &ScanEditor::rotateCW);
  connect(m_rot_angle_act,
          &QAction::triggered,
          m_image_editor,
          &ScanEditor::rotateByAngle);
  connect(m_rot_edge_act,
          &QAction::triggered,
          m_image_editor,
          &ScanEditor::rotateByEdge);
  connect(m_copy_act,
          &QAction::triggered,
          m_image_editor,
          &ScanEditor::copySelection);
  connect(m_scale_act, &QAction::triggered, m_image_editor, &ScanEditor::scale);
  connect(m_crop_act,
          &QAction::triggered,
          m_image_editor,
          &ScanEditor::cropToSelection);
  connect(m_save_act, &QAction::triggered, m_image_editor, &ScanEditor::save);
  connect(
    m_save_as_act, &QAction::triggered, m_image_editor, &ScanEditor::saveAs);
  connect(
    m_zoom_in_act, &QAction::triggered, m_image_editor, &ScanEditor::zoomIn);
  connect(
    m_zoom_out_act, &QAction::triggered, m_image_editor, &ScanEditor::zoomOut);
  connect(
    m_fit_best_act, &QAction::triggered, m_image_editor, &ScanEditor::fitBest);
  connect(m_fit_width_act,
          &QAction::triggered,
          m_image_editor,
          &ScanEditor::fitWidth);
  connect(m_fit_height_act,
          &QAction::triggered,
          m_image_editor,
          &ScanEditor::fitHeight);
}

void
MainWindow::scannerSelectionChanged()
{
  QObject* obj = sender();

  if (obj == m_scanners) {
    QList<QTableWidgetItem*> list = m_scanners->selectedItems();

    if (!list.isEmpty()) {
      QTableWidgetItem* item = list.at(0);
      m_selected_name = item->data(Qt::DisplayRole).toString();
      m_selected = true;

    } else {
      m_selected = false;
    }
  }
}

void
MainWindow::startScanning()
{
  m_scan_lib->startScanning(m_selected_name);
}

void
MainWindow::cancelScanning()
{
  m_scan_lib->cancelScan(/*m_selected_name*/);
}

void
MainWindow::scanHasFailed()
{
  QMessageBox::warning(this,
                       tr("Scan failed"),
                       tr("The scanner returned with a Scan Failed message."),
                       QMessageBox::Ok);
}

void
MainWindow::scanProgressed(const int&)
{}

void
MainWindow::geometry()
{}

void
MainWindow::doubleClicked(const QModelIndex& index)
{
  int row = index.row();
  QString name = m_scanners->item(row, 0)->text();
  m_scan_act->setEnabled(false);

  if (m_scan_lib->openDevice(name)) {
    m_selected_name = name;
    m_image_editor->setSelectedName(m_selected_name);
    m_scan_act->setEnabled(true);
  } else {
    m_logger->debug(tr("Unable to open %1").arg(m_selected_name));
  }
}
void
MainWindow::receiveOptionsSet(ScanDevice* device)
{
  //  ScanDevice* device = m_scan_lib->device(m_selected_name);
  ScanOptions* options = device->options;

  m_mode_box->clear();
  QStringList list = options->modes();
  m_mode_box->addItems(list);

  m_source_box->clear();
  list = options->sources();
  m_source_box->addItems(list);

  int min = options->minResolution();
  int max = options->maxResolution();
  ScanUnits units = options->units();
  QString u = (units == ScanUnits::DPI ? "dpi" : "mm");
  int res = options->resolution();
  QString s = QString("%1%2");
  m_min_res->setText(s.arg(min).arg(u));
  m_max_res->setText(s.arg(max).arg(u));
  m_res_box->setMinimum(min);
  m_res_box->setMaximum(max);
  m_res_box->setValue(res);
  m_curr_src->setText(options->source());
  m_source_box->setCurrentText(options->source());
  m_curr_mode->setText(options->mode());
  m_mode_box->setCurrentText(options->mode());

  connect(m_mode_box,
          &QComboBox::currentTextChanged,
          this,
          &MainWindow::modeChangeSelected);
  connect(m_source_box,
          &QComboBox::currentTextChanged,
          this,
          &MainWindow::sourceChangeSelected);
}

void
MainWindow::receiveModeChange(ScanDevice* device)
{
  m_curr_mode->setText(device->options->mode());
}

void
MainWindow::receiveSourceChange(ScanDevice* device)
{
  m_curr_src->setText(device->options->source());
}

void
MainWindow::modeChangeSelected(const QString& mode)
{
  ScanDevice* device = m_scan_lib->device(m_selected_name);
  m_scan_lib->setScanMode(device, mode);
}

void
MainWindow::sourceChangeSelected(const QString& source)
{
  ScanDevice* device = m_scan_lib->device(m_selected_name);
  m_scan_lib->setSource(device, source);
}

void
MainWindow::modifyingSelection()
{
  disableSelectionBtns();
}

void
MainWindow::editorHasSelection()
{
  enableSelectionBtns();
  disableNoSelectionBtns();
}

void
MainWindow::editorHasNoSelection()
{
  disableSelectionBtns();
  enableNoSelectionBtns();
}

void
MainWindow::imageLoaded()
{
  enableImageLoadedBtns();
  enableNoSelectionBtns();
  disableSelectionBtns();
}

void
MainWindow::disableNoSelectionBtns()
{
  m_rot_left_act->setEnabled(false);
  m_rot_right_act->setEnabled(false);
  m_rot_angle_act->setEnabled(false);
  m_rot_edge_act->setEnabled(false);
  m_scale_act->setEnabled(false);
}

void
MainWindow::enableNoSelectionBtns()
{
  m_rot_left_act->setEnabled(true);
  m_rot_right_act->setEnabled(true);
  m_rot_angle_act->setEnabled(true);
  m_rot_edge_act->setEnabled(true);
  m_scale_act->setEnabled(true);
}

void
MainWindow::enableSelectionBtns()
{
  m_crop_act->setEnabled(true);
  m_copy_act->setEnabled(true);
}

void
MainWindow::disableSelectionBtns()
{
  m_crop_act->setEnabled(false);
  m_copy_act->setEnabled(false);
}

void
MainWindow::enableImageLoadedBtns()
{
  m_save_act->setEnabled(true);
  m_save_as_act->setEnabled(true);
  m_zoom_in_act->setEnabled(true);
  m_zoom_out_act->setEnabled(true);
  m_fit_best_act->setEnabled(true);
  m_fit_width_act->setEnabled(true);
  m_fit_height_act->setEnabled(true);
}

void
MainWindow::disableImageLoadedBtns()
{
  m_save_act->setEnabled(false);
  m_save_as_act->setEnabled(false);
  m_zoom_in_act->setEnabled(false);
  m_zoom_out_act->setEnabled(false);
  m_fit_best_act->setEnabled(false);
  m_fit_width_act->setEnabled(false);
  m_fit_height_act->setEnabled(false);
}
