#include "qscanplugin.h"

#include "scaneditor.h"
#include "ocreditor.h"
#include "qmenuutils.h"
#include "scaninterface.h"

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

QList<QMenu*> QScanPlugin::menus()
{
  QList<QMenu*> menu_list;
  QList<QMenu*> menus;

  if (m_scan_editor) {
    menus = m_scan_editor->menus();
    QMenuUtils::mergeMenus(menus, menu_list);
  }

  if (m_ocr_editor) {
    menus = m_ocr_editor->menus();
    QMenuUtils::mergeMenus(menus, menu_list);
  }

  return menu_list;
}

QList<QToolBar*> QScanPlugin::toolbars()
{
  QList<QToolBar*> toolbar_list;

  if (m_scan_editor) {
    toolbar_list.append(m_scan_editor->toolbars());
  }

  if (m_ocr_editor) {
    toolbar_list.append(m_ocr_editor->toolbars());
  }

  return toolbar_list;
}

QList<StackableFrame*> QScanPlugin::editors(QWidget* parent)
{
  m_scan_editor = new ScanEditor(parent);
  m_ocr_editor = new OcrEditor(parent);
  connect(m_scan_editor, &ScanEditor::sendWorkData, m_ocr_editor, &OcrEditor::setData);
  connect(m_ocr_editor, &OcrEditor::makeCompleted, m_scan_editor, &ScanEditor::completeDocument);
  connect(m_scan_editor, &ScanEditor::goToOcrEditor, m_ocr_editor, &StackableFrame::setFrameToTop);
  connect(m_ocr_editor, &OcrEditor::goToScanEditor, m_scan_editor, &StackableFrame::setFrameToTop);
  QList<StackableFrame*> list;
  list << m_scan_editor << m_ocr_editor;
  return list;
}
