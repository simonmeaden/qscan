#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtDebug>

#include <QMainWindow>
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QMessageBox>

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>

#include <QDir>
#include <QVariant>

namespace QScanner {
class QScanner;
class ScanDevice;
}

#include "scanoptions.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

protected:

private:
  QList<QString> m_loaded_plugins;
  QScanner::QScanner* m_scan_lib;

  QListWidget* m_device_list;
  QStackedLayout* m_res_stack;
  int res_stack_empty, res_stack_list, res_stack_range;
  QLabel* m_name_lbl, *m_model_lbl, *m_source_lbl, *m_mode_lbl;
  QLabel* m_x_lbl, *m_y_lbl, *m_width_lbl, *m_height_lbl, *m_depth_lbl/*, *m_brightness_lbl, *m_contrast_lbl*/;
  QLabel* m_resolution_lbl, *m_resolution_x_lbl, *m_resolution_y_lbl;
  QLabel* m_res_range_lbl, *m_brightness_range_lbl, *m_contrast_range_lbl;
  QLabel* half_toning_lbl, *m_dropout_lbl, *m_sharpness_lbl;
  QComboBox* m_mode_box, *m_source_box, *m_res_list;
  QMessageBox* m_dialog;

  void initGui();
  QFrame* initDevicesFrame();
  QFrame* initCommonOptionsFrame();
  QFrame* initUnusualOptionsFrame();

  void receiveScannerData();
  void receiveScannerNameChange(const QModelIndex& index);

  void receiveOptionChange(QScanner::ScanOptions::AvailableOptions option);

  //  void loadPlugins();

  static QSize SIZE;
  static const QString two_args;
  static const QString range_args;
  static const QString one_arg;


};

#endif // MAINWINDOW_H
