#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtDebug>

#include <QMainWindow>
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>

#include <QtPlugin>
#include <QPluginLoader>

#include <QDir>

namespace Ui {
class MainWindow;
}

class QScan;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow* ui;
  QList<QString> m_loaded_plugins;
  QScan *m_qscan;

  void loadPlugins();

  static QSize SIZE;

};

#endif // MAINWINDOW_H
