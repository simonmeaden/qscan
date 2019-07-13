#ifndef STACKABLEFRAME_H
#define STACKABLEFRAME_H

#include <QFrame>
#include <QList>
#include<QToolBar>
#include <QMenu>

class StackableFrame : public QFrame
{
  Q_OBJECT
public: StackableFrame(QWidget* parent = nullptr);
  StackableFrame(const StackableFrame& frame);
  ~StackableFrame();

  int id() const;
  void setId(int id);

  void setFrameToTop();

  //  virtual QList<QToolBar*> toolbars() = 0;
  //  virtual QList<QMenu*> menus() = 0;

signals:
  void setTop(int id);

protected:
  int m_id;
};

#endif // STACKABLEFRAME_H
