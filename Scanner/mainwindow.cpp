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

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m_selected(false)
{

  initGui();

  m_scan = new QScan(this);
  m_scan->init();

  QStringList scanners = m_scan->getDevices();

  for (int i = 0; i < scanners.size(); i++) {
    QString name = scanners.at(i);
    Device s = m_scan->getDevice(name);
    int row = m_scanners->rowCount();
    m_scanners->insertRow(row);
    m_scanners->setItem(row, 0, new QTableWidgetItem(s->name));
    m_scanners->setItem(row, 1, new QTableWidgetItem(s->vendor));
    m_scanners->setItem(row, 2, new QTableWidgetItem(s->model));
    m_scanners->setItem(row, 3, new QTableWidgetItem(s->type));
  }
}

MainWindow::~MainWindow() {}

void
MainWindow::initGui()
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

  QGridLayout* main_layout = new QGridLayout(this);
  main_frame->setLayout(main_layout);

  m_splitter = new QSplitter(this);

  m_image = new QLabel(this);
  m_image->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_splitter->addWidget(m_image);

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
  m_scanners->setSelectionMode(QAbstractItemView::SingleSelection);
  m_scanners->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_scanners->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  connect(
    m_scanners, &QTableWidget::clicked, this, &MainWindow::selectionChanged);
  m_splitter->addWidget(m_scanners);

  main_layout->addWidget(m_splitter, 0, 0);

  QFrame* btn_frame = new QFrame(this);
  QHBoxLayout* btn_layout = new QHBoxLayout(this);
  btn_frame->setLayout(btn_layout);
  main_layout->addWidget(btn_frame, 1, 0);

  m_select_btn = new QPushButton(QStringLiteral("Select Scanner"), this);
  m_select_btn->setEnabled(false);
  connect(
    m_select_btn, &QPushButton::clicked, this, &MainWindow::selectScanner);
  btn_layout->addWidget(m_select_btn);
  m_scan_btn = new QPushButton(QStringLiteral("Start Scanning"), this);
  m_scan_btn->setEnabled(false);
  connect(m_scan_btn, &QPushButton::clicked, this, &MainWindow::startScanning);
  btn_layout->addWidget(m_scan_btn);
  m_cancel_btn = new QPushButton(QStringLiteral("Cancel Scanning"), this);
  m_cancel_btn->setEnabled(false);
  btn_layout->addWidget(m_cancel_btn);
  m_close_btn = new QPushButton(QStringLiteral("Close"), this);
  btn_layout->addWidget(m_close_btn);
  connect(m_close_btn, &QPushButton::clicked, this, &MainWindow::close);
}

void
MainWindow::resizeEvent(QResizeEvent* /*event*/)
{
  QSize size = m_splitter->size();
  int w = int(size.width() / 2.0);
  m_splitter->setSizes(QList<int>({ w, size.width() - w }));
}

void
MainWindow::selectionChanged()
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
  if (m_selected) {
    m_select_btn->setEnabled(true);
  }
}

void
MainWindow::selectScanner()
{
  bool rst = m_scan->openDevice(m_selected_name);
  if (rst) {
    m_scan_btn->setEnabled(true);
  }
}

void
MainWindow::startScanning()
{}
