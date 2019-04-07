#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class QScan;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

protected:
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

private:
};

#endif // MAINWINDOW_H
