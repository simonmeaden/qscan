#ifndef QSCANPLUGIN_H
#define QSCANPLUGIN_H

#include <QObject>
#include <QtPlugin>

//#include "iplugininterface.h"
#include "iscaninterface.h"
#include "qscanplugin_global.h"

namespace QScanner {

class StackableFrame;
class QScan;

class QSCANPLUGINSHARED_EXPORT QScanPlugin
  : public QObject, public IScanInterface
{
  Q_OBJECT Q_PLUGIN_METADATA(IID IPluginInterface_iid FILE "plugininterface.json")
  Q_PLUGIN_METADATA(IID IScanInterface_iid FILE "scaninterface.json")

  Q_INTERFACES(QScanner::IPluginInterface)
  Q_INTERFACES(QScanner::IScanInterface)

public:
  QScanPlugin(QObject* parent = nullptr);

  bool isDeviceSet();

  // IPluginInterface interface
  QString pluginGroup() const override;
  QString pluginName() const override;
  QString vendor() const override;
  bool loaded() const override;
  void setLoaded(bool loaded) override;
  QString version() const override;
  int majorVersion() const override;
  int minorVersion() const override;
  int buildVersion() const override;

  // IScanInterface interface
  int topLeftX() override;
  void setTopLeftX(int value) override;
  int topLeftY() override;
  void setTopLeftY(int value) override;
  int bottomRightX() override;
  void setBottomRightX(int value) override;
  int bottomRightY() override;
  void setBottomRightY(int value) override;
  int contrast() override;
  void setContrast(int value) override;
  int brightness() override;
  void setBrightness(int value) override;
  int resolution() override;
  void setResolution(int value) override;
  int resolutionX() override;
  void setResolutionX(int value) override;
  int resolutionY() override;
  void setResolutionY(int value) override;
  void setPreview() override;
  void clearPreview() override;
  void setScanMode(const QString& mode) override;
  void setSource(const QString& source) override;

signals:
  void scanCompleted(const QImage&, const int resolution);
  void scanFailed();
  void scanOpenFailed();
  void scanProgress(const int&);
  void optionsSet(ScanDevice*);
  void sourceChanged(ScanDevice*);
  void modeChanged(ScanDevice*);

protected:
  QScan* m_qscan;
  ScanDevice* m_current_device;

private:
  // static values.
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

} // end of namespace QScanner

#endif // QSCANPLUGIN_H
