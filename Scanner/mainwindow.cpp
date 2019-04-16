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
  : QIODevice(parent), m_text_edit(text_edit)
{
  open(QIODevice::WriteOnly | QIODevice::Text);
}

TextEditIoDevice::~TextEditIoDevice() {}

void TextEditIoDevice::setTextEdit(QPlainTextEdit* text_edit)
{
  m_text_edit = text_edit;

  if (!m_pre_data.isEmpty()) {
    m_text_edit->setPlainText(m_pre_data);
    m_pre_data.clear();
  }
}

qint64 TextEditIoDevice::readData(char*, qint64)
{
  return 0;
}

qint64 TextEditIoDevice::writeData(const char* data, qint64 maxSize)
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
  m_logger = Log4Qt::Logger::logger(QStringLiteral("Scanner"));
  initGui();
  m_scan = new QScan(this);
  connect(m_scan, &QScan::scanCompleted, m_image_editor, &ScanEditor::setImage);
  connect(m_scan, &QScan::scanProgress, m_image_editor, &ScanEditor::setScanProgress);
  connect(m_scan, &QScan::scanFailed, this, &MainWindow::scanHasFailed);
  m_scan->init();
  QStringList scanners = m_scan->getDevices();

  for (int i = 0; i < scanners.size(); i++) {
    QString name = scanners.at(i);
    ScanDevice* s = m_scan->getDevice(name);
    int row = m_scanners->rowCount();
    m_scanners->insertRow(row);
    m_scanners->setItem(row, 0, new QTableWidgetItem(s->name));
    m_scanners->setItem(row, 1, new QTableWidgetItem(s->vendor));
    m_scanners->setItem(row, 2, new QTableWidgetItem(s->model));
    m_scanners->setItem(row, 3, new QTableWidgetItem(s->type));
  }
}

MainWindow::~MainWindow() {}

void MainWindow::setLogTextEdit(QPlainTextEdit* log_edit)
{
  m_log_edit = log_edit;
  m_h_splitter->replaceWidget(1, m_log_edit);
  m_empty_edit->deleteLater();
}

void MainWindow::initGui()
{
  QScreen* screen = QGuiApplication::primaryScreen();
  QSize size = screen->availableSize();
  int w = size.width() - 1200;
  int h = size.height() - 800;
  int x = int(w / 2.0);
  int y = int(h / 2.0);
  setGeometry(x, y, 1200, 800);
  QFrame* main_frame = new QFrame(this);
  setCentralWidget(main_frame);
  m_main_layout = new QGridLayout;
  main_frame->setLayout(m_main_layout);
  //
  m_image_editor = new ScanEditor(this);
  m_image_editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  connect(m_image_editor, &ScanEditor::scanCancelled, this,
          &MainWindow::cancelScanning);
  //
  QStringList labels;
  labels << "Name" << "Vendor" << "Model" << "Type";
  m_scanners = new QTableWidget(this);
  m_scanners->setHorizontalHeaderLabels(labels);
  m_scanners->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_scanners->horizontalHeader()->setStretchLastSection(true);
  m_scanners->setColumnCount(4);
  m_scanners->setSelectionMode(QAbstractItemView::SingleSelection);
  m_scanners->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_scanners->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  connect(m_scanners, &QTableWidget::clicked, this, &MainWindow::selectionChanged);
  connect(m_scanners, &QTableWidget::doubleClicked, this, &MainWindow::doubleClicked);
  //
  m_empty_edit = new QPlainTextEdit(this);
  //
  m_v_splitter = new QSplitter(Qt::Vertical);
  m_v_splitter->insertWidget(0, m_scanners);
  m_v_splitter->insertWidget(1, m_empty_edit);
  //  m_v_splitter->setSizes(QList<int>({INT_MAX, INT_MAX}));
  //
  m_h_splitter = new QSplitter(Qt::Horizontal);
  m_h_splitter->insertWidget(0, m_image_editor);
  m_h_splitter->insertWidget(1, m_v_splitter);
  //  m_h_splitter->setSizes(QList<int>({INT_MAX, INT_MAX}));
  //
  m_main_layout->addWidget(m_h_splitter, 0, 0, 1, 2);
  //
  m_scan_btn = new QPushButton(QStringLiteral("Start Scanning"), this);
  m_scan_btn->setEnabled(false);
  connect(m_scan_btn, &QPushButton::clicked, m_image_editor, &ScanEditor::scanningStarted);
  connect(m_scan_btn, &QPushButton::clicked, this, &MainWindow::startScanning);
  m_main_layout->addWidget(m_scan_btn, 1, 0);
  m_close_btn = new QPushButton(QStringLiteral("Close"), this);
  m_main_layout->addWidget(m_close_btn, 1, 1);
  connect(m_close_btn, &QPushButton::clicked, this, &MainWindow::close);
}

void MainWindow::resizeEvent(QResizeEvent* /*event*/)
{
  QSize size = m_h_splitter->size();
  int w = int(size.width() / 2.0);
  m_h_splitter->setSizes(QList<int>({ w, size.width() - w }));
  size = m_v_splitter->size();
  int h = int(size.height() / 2.0);
  m_h_splitter->setSizes(QList<int>({ w, size.height() - h }));
}

void MainWindow::selectionChanged()
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

  //  if (m_selected) {
  //    m_select_btn->setEnabled(true);
  //  }
}

void MainWindow::selectScanner()
{
  if (m_scan->openDevice(m_selected_name)) {
    m_scan_btn->setEnabled(true);
    //    m_geometry_btn->setEnabled(true);

  } else {
    m_logger->debug(QString("Unable to open %1").arg(m_selected_name));
  }
}

void MainWindow::startScanning()
{
  if (m_scan->startScanning(m_selected_name)) {
    // TODO
  }
}

void MainWindow::cancelScanning()
{
  m_scan->cancelScan(m_selected_name);
}

void MainWindow::scanHasFailed()
{}

void MainWindow::scanProgressed(const int&)
{}

void MainWindow::geometry()
{}

void MainWindow::doubleClicked(const QModelIndex& index)
{
  int row = index.row();
  QString data = m_scanners->item(row, 0)->text();

  if (m_scan->openDevice(data)) {
    m_selected_name = data;
    m_scan_btn->setEnabled(true);
    //    m_geometry_btn->setEnabled(true);

  } else {
    m_logger->debug(QString("Unable to open %1").arg(m_selected_name));
  }
}

