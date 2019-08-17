#ifndef QMENUUTILS_H
#define QMENUUTILS_H

#include <QMenu>
#include <QMap>
#include <QString>

namespace QMenuUtils {

void mergeMenus(QMap<QString, QMenu*> child_menus, QMap<QString, QMenu*>& parent_menus);
void mergeMenus(QList<QMenu*> child_menus, QList<QMenu*>& parent_menus);

} // end of namespace QMenuUtils

#endif // QMENUUTILS_H
