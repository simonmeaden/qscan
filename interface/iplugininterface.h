#ifndef IPLUGININTERFACE_H
#define IPLUGININTERFACE_H

#include <QtPlugin>
#include <QToolBar>
#include <QMenu>
#include <QFrame>

#include "interface_global.h"
#include "stackableframe.h"

/*!
   \brief The interface for all EBookEdit plugins.
*/
class INTERFACESHARED_EXPORT IPluginInterface
{
public:
  virtual ~IPluginInterface() {
  }

  virtual QString pluginGroup() const = 0;
  virtual QString pluginName() const = 0;
  virtual QString vendor() const = 0;
  virtual bool loaded() const = 0;
  virtual void setLoaded(bool) = 0;
  virtual QString version() const = 0;
  virtual int majorVersion() const = 0;
  virtual int minorVersion() const = 0;
  virtual int buildVersion() const = 0;

  virtual QMap<QString, QMenu*> menus() = 0;
  //  virtual QList<QToolBar*> toolbars() = 0;
  virtual QList<StackableFrame*> editors(QWidget* parent = nullptr) = 0;

};
#define IPluginInterface_iid "uk.org.smelecomp.IPluginInterface/0.1.0"
Q_DECLARE_INTERFACE(IPluginInterface, IPluginInterface_iid)

#endif // IPLUGININTERFACE_H
