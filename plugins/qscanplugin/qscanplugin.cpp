#include "qscanplugin.h"

#include "scaneditor.h"
#include "ocreditor.h"

const QString QScanPlugin::m_plugin_name = "OCR Plugin";
const QString QScanPlugin::m_plugin_group = "OCR";
const QString QScanPlugin::m_vendor = "SM Electronic Components";
const int QScanPlugin::m_major_version = QSCAN_VERSION_MAJOR;
const int QScanPlugin::m_minor_version = QSCAN_VERSION_MINOR;
const int QScanPlugin::m_build_version = QSCAN_VERSION_BUILD;
const QString QScanPlugin::m_version = QString("%1.%2.%3")
                                       .arg(QScanPlugin::m_major_version)
                                       .arg(QScanPlugin::m_minor_version)
                                       .arg(QScanPlugin::m_build_version);
bool QScanPlugin::m_loaded = false;

QScanPlugin::QScanPlugin(QObject* parent)
  : QObject(parent)
  , m_scan_editor(nullptr)
  , m_ocr_editor(nullptr)
{
}

QString QScanPlugin::pluginGroup() const
{
  return m_plugin_group;
}

QString QScanPlugin::pluginName() const
{
  return m_plugin_name;
}

QString QScanPlugin::vendor() const
{
  return "SM Electronic Components";
}

bool QScanPlugin::loaded() const
{
  return m_loaded;
}

void QScanPlugin::setLoaded(bool loaded)
{
  m_loaded = loaded;
}

QString QScanPlugin::version() const
{
  return m_version;
}

int QScanPlugin::majorVersion() const
{
  return m_major_version;
}

int QScanPlugin::minorVersion() const
{
  return m_minor_version;
}

int QScanPlugin::buildVersion() const
{
  return m_build_version;
}

void QScanPlugin::enumerateMenu(QAction* act, QMenu* parent_menu)
{
  QMenu* sub;

  if ((sub = act->menu())) {
    parent_menu->addMenu(sub);

  }   else {
    // act is either a menu action or a seperator.
    parent_menu->addAction(act);
  }
}

void QScanPlugin::addMenuAction(QMap<QString, QMenu*> menus, QMap<QString, QMenu*> list)
{
  QMenu* menu;

  for (auto name : menus.keys()) {
    if (list.contains(name)) {
      menu = list.value(name);
      QMenu* sub = menus.value(name);

      for (auto act : sub->actions()) {
        enumerateMenu(act, menu);
      }

    } else {
      menu = new QMenu(name);
      list.insert(name, menu);
    }
  }
}

QMap<QString, QMenu*> QScanPlugin::menus()
{
  QMap<QString, QMenu*> list;

  if (m_scan_editor) {
    QMap<QString, QMenu*> menus = m_scan_editor->menus();
    addMenuAction(menus, list);
  }

  if (m_ocr_editor) {
    QMap<QString, QMenu*> menus = m_ocr_editor->menus();
    addMenuAction(menus, list);
  }

  return list;
}

//QList<QToolBar*> QScanPlugin::toolbars()
//{
//  QList<QToolBar*> list;
//  return list;
//}

QList<StackableFrame*> QScanPlugin::editors(QWidget* parent)
{
  m_scan_editor = new ScanEditor(parent);
  m_ocr_editor = new OcrEditor(parent);
  connect(m_scan_editor, &ScanEditor::sendWorkData, m_ocr_editor, &OcrEditor::setData);
  connect(m_ocr_editor, &OcrEditor::makeCompleted, m_scan_editor, &ScanEditor::completeDocument);
  QList<StackableFrame*> list;
  list << m_scan_editor << m_ocr_editor;
  return list;
}
