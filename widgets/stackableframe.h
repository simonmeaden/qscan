#ifndef STACKABLEFRAME_H
#define STACKABLEFRAME_H

#include <QFrame>
#include <QList>
#include <QToolBar>
#include <QMenu>

#include "widgets_global.h"

/*!
   \class StackableFrame
   \brief StackableFrame is a QFrame extension that allows interfacing to a QStackLayout.

   The StackableFrame is used in conjumction with a QStackedLayout
   to allow multiple pages to be stacked on top of each other.

   The Stackable Frame emits a setTop() signal when the setFrameToTop() method is called,
   normally internally in your code.

   Generally this class is only intended to be used internally to the various Biblos
   plugins that have a top-level frame. Each plugin has a list of frames that it supplies
   to the system, and they are generally added to the QStackedLayout during plugin loading
   in the loadPlugins() method at which point the setTop() signal is connected to the
   layout's setCurrentIndex() method.

  \code
    // first create the layout.
    QStackedLayout *layout = new QstackedLayout;
  \endcode

  \code
    // then the StackableFrame
    StackableFrame* frame = new StackableFrame(this);
    // Get the layout id
    int id = layout->addWidget(frame);
    // set the id to the frame
    frame->setId(id);
    // connect the setTop(int) to the setCurrentIndex(int) method.
    connect(frame, &StackableFrame::setTop, layout, &QStackedLayout::setCurrentIndex);
  \endcode

*/

class WIDGETSSHARED_EXPORT StackableFrame : public QFrame
{
  Q_OBJECT

public:
  /*!
     \brief StackableFrame
     \param parent
  */ StackableFrame(QWidget* parent = nullptr);
  StackableFrame(const StackableFrame& frame);
  ~StackableFrame();

  int id() const;
  void setId(int id);

  void setFrameToTop();

signals:
  /*!
     \fn StackableFrame::setTop()

     \brief This signal is emitted when  the setFrameToTop() method is called.

     This signal should be connected to your QStackedLayout::setCurrentIndex method,
     which is normally done when the plugin is loaded in the loadPlugins() method of
     your MainWindow class.

     \code
     connect(frame, &StackableFrame::setTop, m_main_layout, &QStackedLayout::setCurrentIndex);
     \endcode

     \see id()
     \see setId()
  */
  void setTop(int id);

protected:
  int m_id;
};

#endif // STACKABLEFRAME_H
