#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iplugininterface.h"
#include "iscanwidgetinterface.h"
#include "qscanlogging.h"
#include "iscaninterface.h"

using namespace QScanner;

QSize MainWindow::SIZE = QSize(1200, 800);

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  QScreen* screen = QGuiApplication::primaryScreen();
  QSize screen_size = screen->size();
  int x = int((screen_size.width() - SIZE.width()) / 2.0);
  int y = int((screen_size.height() - SIZE.height()) / 2.0);
  QPoint pos(x, y);
  setGeometry(QRect(pos, SIZE));

  loadPlugins();

  qCInfo(QscanScanner) << tr("QScanTest loaded with plugins :");

  for (QString s : m_loaded_plugins) {
    qCInfo(QscanScanner)
        << tr("  Plugin : %1").arg(s);
  }
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::loadPlugins()
{
  QDir plugins_dir = QDir(qApp->applicationDirPath());
  plugins_dir.cd("../plugins");


  foreach (QString filename, plugins_dir.entryList(QDir::Files)) {
    if (filename == "Makefile" || filename.endsWith(".o") || filename.endsWith(".cpp") ||
        filename.endsWith(".h")) {  // can remove this in installed versions.
      continue;
    }

    /*
      This bit is only used in a unix type OS's where you end
      up with miltiple links to the same file.
      ie. libname.so, libname.so.1, libname.so.1.0, libname.so.1.0.0
      otherwise you get the same plugin loaded 4 times.
    */
    QFileInfo info(filename);
    QString basename = info.baseName();

    if (m_loaded_plugins.contains(basename)) {
      continue;
    }

    QPluginLoader loader(plugins_dir.absoluteFilePath(filename));
    QObject* plugin = loader.instance();

    if (plugin) {
      IPluginInterface* iplugin = dynamic_cast<IPluginInterface*>(plugin);

      if (iplugin) {
        m_loaded_plugins.append(basename);
        QString name = iplugin->pluginName();

        //        if (name == QStringLiteral("QScan Plugin")) {
        ////          m_qscan
        ////            = new (QScan())
        //        }
      }
    }
  }
}
