#include "mainwindow.h"

#include "logging.h"
#include "qscan.h"
#include "scandevice.h"
#include "scanoptions.h"

using namespace QScanner;

QSize MainWindow::SIZE = QSize(1200, 800);
const QString MainWindow::two_args = tr("%1 %2");
const QString MainWindow::one_arg = tr("%1");
const QString MainWindow::range_args = tr("%1%2 to %3%4 : %5%6");


MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  QScreen* screen = QGuiApplication::primaryScreen();
  QSize screen_size = screen->size();
  int x = int((screen_size.width() - SIZE.width()) / 2.0);
  int y = int((screen_size.height() - SIZE.height()) / 2.0);
  QPoint pos(x, y);
  setGeometry(QRect(pos, SIZE));

  //  loadPlugins();
  initGui();

  qCInfo(LogQScanTest) << tr("QScanTest loaded with plugins :");

  for (QString s : m_loaded_plugins) {
    qCInfo(LogQScanTest)
        << tr("  Plugin : %1").arg(s);
  }

  m_scan_lib = new QScan(this);
  connect(m_scan_lib, &QScan::optionsSet, this, &MainWindow::receiveScannerData);
  connect(m_scan_lib, &QScan::optionChanged, this, &MainWindow::receiveOptionChange);
  m_scan_lib->init();

  QStringList devices = m_scan_lib->devices();
  m_device_list->addItems(devices);
}

MainWindow::~MainWindow()
{
}

QFrame* MainWindow::initDevicesFrame()
{
  QFrame* frame = new QFrame(this);
  QGridLayout* grid1 = new QGridLayout;
  frame->setLayout(grid1);

  QLabel* lbl = new QLabel("Device names:", this);
  grid1->addWidget(lbl, 0, 0);

  m_device_list = new QListWidget(this);
  grid1->addWidget(m_device_list, 1, 0);
  //  connect(m_device_list, &QListWidget::clicked, this, &MainWindow::receiveScannerNameChange);
  connect(m_device_list, &QListWidget::doubleClicked, this, &MainWindow::receiveScannerNameChange);

  return frame;
}

QFrame* MainWindow::initCommonOptionsFrame()
{
  QFrame* frame = new QFrame(this);
  QFormLayout* form = new QFormLayout;
  frame->setLayout(form);

  m_name_lbl = new QLabel(this);
  form->addRow(tr("Device :"), m_name_lbl);

  m_model_lbl = new QLabel(this);
  form->addRow(tr("Model :"), m_model_lbl);

  m_source_box = new QComboBox(this);
  form->addRow(tr("Select Source :"), m_source_box);

  m_source_lbl = new QLabel(this);
  form->addRow(tr("Selected Source :"), m_source_lbl);

  m_mode_box = new QComboBox(this);
  form->addRow(tr("Select Mode :"), m_mode_box);

  m_mode_lbl = new QLabel(this);
  form->addRow(tr("Selected Mode :"), m_mode_lbl);

  m_x_lbl = new QLabel(this);
  form->addRow(tr("X :"), m_x_lbl);

  m_y_lbl = new QLabel(this);
  form->addRow(tr("Y :"), m_y_lbl);

  m_width_lbl = new QLabel(this);
  form->addRow(tr("Width :"), m_width_lbl);

  m_height_lbl = new QLabel(this);
  form->addRow(tr("Height :"), m_height_lbl);

  m_depth_lbl = new QLabel(this);
  form->addRow(tr("Depth :"), m_depth_lbl);

  m_resolution_lbl = new QLabel(this);
  form->addRow(tr("Resolution :"), m_resolution_lbl);

  m_resolution_x_lbl = new QLabel(this);
  form->addRow(tr("Resolution X :"), m_resolution_x_lbl);

  m_resolution_y_lbl = new QLabel(this);
  form->addRow(tr("Resolution Y :"), m_resolution_y_lbl);

  QFrame* stack_frame = new QFrame(this);
  m_res_stack = new QStackedLayout;
  stack_frame->setLayout(m_res_stack);
  m_res_list = new QComboBox(this);
  res_stack_list = m_res_stack->addWidget(m_res_list);
  m_res_range_lbl = new QLabel(this);
  res_stack_range = m_res_stack->addWidget(m_res_range_lbl);
  QLabel* lbl = new QLabel(this);
  res_stack_empty = m_res_stack->addWidget(lbl);
  m_res_stack->setCurrentIndex(res_stack_empty);
  form->addRow(tr("resolution Range :"), stack_frame);

  m_brightness_range_lbl = new QLabel(this);
  form->addRow(tr("Brightness Range :"), m_brightness_range_lbl);

  m_contrast_range_lbl = new QLabel(this);
  form->addRow(tr("Contrast Range :"), m_contrast_range_lbl);

  QFrame* filler = new QFrame(this);
  filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  form->addWidget(filler);

  return frame;
}

QFrame* MainWindow::initUnusualOptionsFrame()
{
  QFrame* frame = new QFrame(this);
  QFormLayout* form = new QFormLayout;
  frame->setLayout(form);

  half_toning_lbl = new QLabel(this);
  form->addRow(tr("Half toning :"), half_toning_lbl);

  m_dropout_lbl = new QLabel(this);
  form->addRow(tr("Dropout :"), m_dropout_lbl);

  m_sharpness_lbl = new QLabel(this);
  form->addRow(tr("Sharpness :"), m_sharpness_lbl);

  QFrame* filler = new QFrame(this);
  filler->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  form->addWidget(filler);

  return frame;
}

void MainWindow::initGui()
{
  QFrame* main_frame = new QFrame(this);
  QHBoxLayout* box = new QHBoxLayout;
  main_frame->setLayout(box);
  setCentralWidget(main_frame);

  box->addWidget(initDevicesFrame(), 1);
  box->addWidget(initCommonOptionsFrame(), 3);
  box->addWidget(initUnusualOptionsFrame(), 3);
}

void MainWindow::receiveScannerData()
{
  if (m_scan_lib->isDeviceAvaliable()) {
    m_name_lbl->setText(m_scan_lib->displayName());
    m_model_lbl->setText(m_scan_lib->model());

    auto* options = m_scan_lib->options();

    m_source_box->clear();
    m_source_box->addItems(options->sources());
    m_source_lbl->setText(options->source());
    m_mode_box->clear();
    m_mode_box->addItems(options->modes());
    m_mode_lbl->setText(options->mode());

    // common values
    QString units_str = options->unitsToString(ScanOptions::LEFT);
    QVariant var = options->optionData(ScanOptions::LEFT);
    RangeData r;

    if (var.canConvert<RangeData>()) {
      r = var.value<RangeData>();
      m_x_lbl->setText(range_args.arg(r.min).arg(units_str).arg(r.max).arg(units_str).arg(options->left()).arg(units_str));

    } else {
      m_x_lbl->setText(two_args.arg(options->left()).arg(units_str));
    }

    var = options->optionData(ScanOptions::TOP);

    if (var.canConvert<RangeData>()) {
      r = var.value<RangeData>();
      m_y_lbl->setText(range_args.arg(r.min).arg(units_str).arg(r.max).arg(units_str).arg(options->top()).arg(units_str));

    } else {
      m_y_lbl->setText(two_args.arg(options->top()).arg(units_str));
    }

    var = options->optionData(ScanOptions::LEFT);

    if (var.canConvert<RangeData>()) {
      m_width_lbl->setText(range_args.arg(options->left()).arg(units_str).arg(options->right()).arg(units_str).arg(
                             options->width()).arg(
                             units_str));

    } else {
      m_width_lbl->setText(two_args.arg(options->top()).arg(units_str));
    }

    var = options->optionData(ScanOptions::TOP);

    if (var.canConvert<RangeData>()) {
      m_height_lbl->setText(range_args.arg(options->top()).arg(units_str).arg(options->bottom()).arg(units_str).arg(
                              options->height()).arg(
                              units_str));

    } else {
      m_height_lbl->setText(two_args.arg(options->top()).arg(units_str));
    }

    units_str = options->unitsToString(ScanOptions::RESOLUTION);
    m_resolution_lbl->setText(two_args.arg(options->resolution()).arg(units_str));
    m_resolution_x_lbl->setText((options->resolutionX() == -1 ?
                                 tr("Not supplied") :
                                 two_args.arg(options->resolutionX()).arg(units_str)));
    m_resolution_y_lbl->setText((options->resolutionY() == -1 ?
                                 tr("Not supplied") :
                                 two_args.arg(options->resolutionY()).arg(units_str)));
    var = options->optionData(ScanOptions::RESOLUTION);
    units_str = options->unitsToString(ScanOptions::RESOLUTION);

    if (var.canConvert<QList<int>>()) {
      m_res_list->clear();
      QList<int> values = var.value<QList<int>>();
      QStringList items;

      for (int value : values) {
        m_res_list->addItem(QString::number(value));
      }

      m_res_stack->setCurrentIndex(res_stack_list);

    } else if (var.canConvert<RangeData>()) {
      r = var.value<RangeData>();
      QString s = tr("%1 %2 - %3 %4");
      m_res_range_lbl->setText(s.arg(r.min).arg(units_str).arg(r.max).arg(units_str));
      m_res_stack->setCurrentIndex(res_stack_range);

    } else {
      m_res_stack->setCurrentIndex(res_stack_empty);
    }

    var = options->optionData(ScanOptions::BRIGHTNESS);

    if (var.canConvert<RangeData>()) {
      r = var.value<RangeData>();
      m_brightness_range_lbl->setText(range_args.arg(r.min).arg("").arg(r.max).arg("").arg(options->brightness()).arg(""));

    } else {
      m_brightness_range_lbl->setText((options->brightness() == -1 ?
                                       tr("Not Supplied") :
                                       one_arg.arg(options->brightness())));
    }

    var = options->optionData(ScanOptions::CONTRAST);

    if (var.canConvert<RangeData>()) {
      r = var.value<RangeData>();
      m_contrast_range_lbl->setText(range_args.arg(r.min).arg("").arg(r.max).arg("").arg(options->contrast()).arg(""));

    } else {
      m_contrast_range_lbl->setText((options->contrast() == -1 ?
                                     tr("Not Supplied") :
                                     one_arg.arg(options->contrast())));
    }

    m_depth_lbl->setText((options->depth() == -1 ?
                          tr("Not supplied") :
                          one_arg.arg(options->depth())));

    // uncommon values


    m_dialog->hide();
  }
}

void MainWindow::receiveScannerNameChange(const QModelIndex& index)
{
  if (index.isValid()) {
    QListWidgetItem* item = m_device_list->item(index.row());
    QString name = item->text();
    m_name_lbl->setText(name);
    m_scan_lib->setDevice(name);

    m_dialog = new QMessageBox(QMessageBox::Information,
                               tr(""),
                               tr("Loading scanner options"),
                               QMessageBox::NoButton,
                               this);
    m_dialog->setStandardButtons(0);
    m_dialog->setWindowFlags(Qt::FramelessWindowHint);
    m_dialog->setStyleSheet(
      "border: 0px solid transparent;"
      "border-radius: 40px;"
      "background-color: rgba(0, 0, 0, 0);"
    );
    int width = geometry().width();
    int height = geometry().height();
    int x = geometry().x();
    int y = geometry().y();
    int d_width = m_dialog->width();
    int d_height = m_dialog->height();
    m_dialog->show();
    m_dialog->setGeometry(QRect(
                            QPoint(
                              x + int((width - d_width) / 2.0),
                              y + int((height - d_height) / 2.0)
                            ),
                            m_dialog->size()
                          ));
  }
}

void MainWindow::receiveOptionChange(ScanOptions::AvailableOptions option)
{
  auto* options = m_scan_lib->options();

  switch (option) {
  case ScanOptions::LEFT:
    m_x_lbl->setText(two_args.arg(options->right()).arg(options->unitsToString(ScanOptions::LEFT)));
    break;

  case ScanOptions::TOP:
    m_y_lbl->setText(two_args.arg(options->top()).arg(options->unitsToString(ScanOptions::TOP)));
    break;

  case ScanOptions::RIGHT:
    m_width_lbl->setText(two_args.arg(options->width()).arg(options->unitsToString(ScanOptions::RIGHT)));
    break;

  case ScanOptions::BOTTOM:
    m_height_lbl->setText(two_args.arg(options->height()).arg(options->unitsToString(ScanOptions::BOTTOM)));
    break;

  case ScanOptions::RESOLUTION:
    m_resolution_lbl->setText(two_args.arg(options->resolution()).arg(options->unitsToString(
                                ScanOptions::RESOLUTION)));
    m_resolution_x_lbl->setText(two_args.arg(options->resolution()).arg(options->unitsToString(
                                  ScanOptions::RESOLUTION)));
    m_resolution_y_lbl->setText(two_args.arg(options->resolution()).arg(options->unitsToString(
                                  ScanOptions::RESOLUTION)));
    break;

  case ScanOptions::X_RESOLUTION:
    m_resolution_x_lbl->setText(two_args.arg(options->resolutionX()).arg(options->unitsToString(
                                  ScanOptions::RESOLUTION)));
    break;

  case ScanOptions::Y_RESOLUTION:
    m_resolution_y_lbl->setText(two_args.arg(options->resolutionY()).arg(options->unitsToString(
                                  ScanOptions::RESOLUTION)));
    break;

  //  case ScanOptions::BRIGHTNESS:
  //    m_brightness_lbl->setText(one_arg.arg(options->brightness()));
  //    break;

  //  case ScanOptions::CONTRAST:
  //    m_contrast_lbl->setText(one_arg.arg(options->brightness()));
  //    break;

  case ScanOptions::BIT_DEPTH:
    m_depth_lbl->setText(two_args.arg(options->depth()).arg("bits"));
    break;

  case ScanOptions::SOURCE:
    m_source_lbl->setText(options->source());
    break;

  case ScanOptions::MODE:
    m_source_lbl->setText(options->mode());
    break;
  }
}

//void MainWindow::loadPlugins()
//{
//  QDir plugins_dir = QDir(qApp->applicationDirPath());
//  plugins_dir.cd("../plugins");


//  foreach (QString filename, plugins_dir.entryList(QDir::Files)) {
//    if (filename == "Makefile" || filename.endsWith(".o") || filename.endsWith(".cpp") ||
//        filename.endsWith(".h")) {  // can remove this in installed versions.
//      continue;
//    }

//    /*
//      This bit is only used in a unix type OS's where you end
//      up with miltiple links to the same file.
//      ie. libname.so, libname.so.1, libname.so.1.0, libname.so.1.0.0
//      otherwise you get the same plugin loaded 4 times.
//    */
//    QFileInfo info(filename);
//    QString basename = info.baseName();

//    if (m_loaded_plugins.contains(basename)) {
//      continue;
//    }

//    QPluginLoader loader(plugins_dir.absoluteFilePath(filename));
//    QObject* plugin = loader.instance();

//    if (plugin) {
//      IPluginInterface* iplugin = dynamic_cast<IPluginInterface*>(plugin);

//      if (iplugin) {
//        m_loaded_plugins.append(basename);
//        QString name = iplugin->pluginName();

//        //        if (name == QStringLiteral("QScan Plugin")) {
//        ////          m_qscan
//        ////            = new (QScan())
//        //        }
//      }
//    }
//  }
//}
