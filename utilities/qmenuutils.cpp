#include "qmenuutils.h"

namespace QMenuUtils {

namespace  {

void enumerateMenu(QAction* act, QMenu* parent_menu)
{
  QMenu* sub;

  if ((sub = act->menu())) {
    parent_menu->addMenu(sub);

  }   else {
    // act is either a menu action or a seperator.
    parent_menu->addAction(act);
  }
}

}

/*!
   \brief Merges a map of named menus into a master map.

  The function searches for a QMenu in the master map that matches a QMenu in
  the child map, then if it exists it adds all QAction's in the child menu to
  the end of the master menu of the same name. If there is no matching member
  in the master menu map then the QMenu is added as a new member;

   \param menus - the child map.
   \param menu_map - the master map.
*/
void mergeMenus(QMap<QString, QMenu*> child_menus, QMap<QString, QMenu*>& parent_menus)
{
  /* TODO - Might need to reorder the menus, for example make certain that the
    standard menus are first and the help if any last. ????*/
  QMenu* menu;

  for (auto name : child_menus.keys()) {
    if (parent_menus.contains(name)) {
      menu = parent_menus.value(name);
      QMenu* sub = child_menus.value(name);

      for (auto act : sub->actions()) {
        enumerateMenu(act, menu);
      }

    } else {
      menu = new QMenu(name);
      parent_menus.insert(name, menu);
    }
  }
}

void mergeMenus(QList<QMenu*> child_menus, QList<QMenu*>& parent_menus)
{
  for (auto c_menu : child_menus) {
    QString title = c_menu->title();

    // check for existing menu of same name.
    for (auto p_menu : parent_menus) {
      if (p_menu->title() == title) {
        for (auto act : c_menu->actions()) {
          enumerateMenu(act, p_menu);
        }

        // menu merged so exit function
        return;
      }
    }

    // no same name menu exists so add it.
    parent_menus.append(c_menu);
  }
}

}// end of namespace QMenuUtils
