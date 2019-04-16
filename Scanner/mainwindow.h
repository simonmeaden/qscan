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

#include "scaneditor.h"

class QScan;
typedef QSharedPointer<QImage> Image;

class TextEditIoDevice : public QIODevice
{
  Q_OBJECT

public: TextEditIoDevice(QPlainTextEdit* text_edit, QObject* parent = nullptr);
  ~TextEditIoDevice();

  void setTextEdit(QPlainTextEdit* m_text_edit);

protected:
  qint64 readData(char* /*data*/, qint64 /*maxSize*/);
  qint64 writeData(const char* data, qint64 maxSize);

private:
  QPlainTextEdit* m_text_edit;
  QString m_pre_data;
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  void setLogTextEdit(QPlainTextEdit* log_edit);

protected:
  Log4Qt::Logger* m_logger;
  QScan* m_scan;
  QFrame* m_main;
  QGridLayout* m_main_layout;
  ScanEditor* m_image_editor;
  QTableWidget* m_scanners;
  QPushButton* m_close_btn;
  QPushButton* m_scan_btn;
  QSplitter* m_h_splitter;
  QSplitter* m_v_splitter;

  QString m_selected_name;
  bool m_selected;
  QTextStream* m_log_stream;
  QPlainTextEdit* m_empty_edit;
  QPlainTextEdit* m_log_edit;

  void initGui();
  void resizeEvent(QResizeEvent*);
  void selectionChanged();
  void selectScanner();
  void startScanning();
  void cancelScanning();
  //  void
  //  scanIsCompleted(const QImage& image);
  void scanHasFailed();
  void scanProgressed(const int&);
  void geometry();
  void doubleClicked(const QModelIndex& index);

private:
};

#endif // MAINWINDOW_H
