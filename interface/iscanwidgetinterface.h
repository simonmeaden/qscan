#ifndef ISCANWIDGETINTERFACE_H
#define ISCANWIDGETINTERFACE_H

#include <QWidget>
#include <QList>
#include <QMenu>
#include <QToolBar>

namespace QScanner {

class IScanWidgetInterface
{
public:
  virtual ~IScanWidgetInterface() {
  }

  virtual QList<QMenu*> menus() = 0;
  virtual QList<QToolBar*> toolbars() = 0;
  //  virtual QList<StackableFrame*> editors(QWidget* parent = nullptr) = 0;
};
#define IScanWidgetInterface_iid "org.smelecomp.IScanWidgetInterface/0.1.0"

} // end of namespace QScanner

Q_DECLARE_INTERFACE(QScanner::IScanWidgetInterface, IScanWidgetInterface_iid)

#endif // ISCANWIDGETINTERFACE_H
