#ifndef QSCANPLUGIN_H
#define QSCANPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "qscanplugin_global.h"
#include "iscaninterface.h"

class ScanEditor;
class OcrEditor;

class QSCANPLUGINSHARED_EXPORT QScanPlugin : public QObject, public IScanInterface
{
  Q_OBJECT Q_PLUGIN_METADATA(IID IScanInterface_iid FILE "scaninterface.json")
  Q_INTERFACES(IPluginInterface)
  Q_INTERFACES(IScanInterface)
public:
  QScanPlugin(QObject* parent = nullptr);

  QString pluginGroup() const override;
  QString pluginName() const override;
  QString vendor() const override;
  bool loaded() const override;
  void setLoaded(bool loaded) override;
  QString version() const override;
  int majorVersion() const override;
  int minorVersion() const override;
  int buildVersion() const override;

  QList<QMenu*> menus() override;
  QList<QToolBar*> toolbars() override;
  QList<StackableFrame*> editors(QWidget* parent) override;

protected:
  ScanEditor* m_scan_editor;
  OcrEditor* m_ocr_editor;

  static const QString m_plugin_group;
  static const QString m_plugin_name;
  static const QString m_vendor;
  static const QString m_version;
  static const int m_major_version;
  static const int m_minor_version;
  static const int m_build_version;
  static bool m_loaded;
  static const QString m_file_filter;
  static const QString m_file_description;

  //  void enumerateMenu(QAction* act, QMenu* pareny_menu);
  //  void addMenuAction(QMap<QString, QMenu*> menus, QMap<QString, QMenu*> list);
};

#endif // QSCANPLUGIN_H
