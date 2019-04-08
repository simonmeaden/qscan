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

#include <QMainWindow>
#include <QtWidgets>

#include <log4qt/logger.h>

class QScan;
typedef QSharedPointer<QImage> Image;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

protected:
  Log4Qt::Logger* m_logger;
  QScan* m_scan;
  QFrame* m_main;
  QLabel* m_image;
  QTableWidget* m_scanners;
  QPushButton* m_close_btn;
  QPushButton* m_scan_btn;
  QPushButton* m_cancel_btn;
  QPushButton* m_select_btn;
  QSplitter* m_splitter;

  QString m_selected_name;
  bool m_selected;

  void initGui();
  void resizeEvent(QResizeEvent*);
  void selectionChanged();
  void selectScanner();
  void startScanning();
  void cancelScanning();
  void scanIsCompleted(Image image);
  void scanHasFailed();
  void scanProgressed(double);

private:
};

#endif // MAINWINDOW_H
