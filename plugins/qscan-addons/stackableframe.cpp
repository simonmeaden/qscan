#include "stackableframe.h"

/*!
  \class StackableFrame StackableFrame.h

  \brief StackableFrame::StackableFrame

  The StackableFrame is used in conjumction with a QStackedLayout
  to allow multiple pages to be stacked on top of each other.

  \code
    // first create the layout.
    QStackedLayout *layout = new QstackedLayout;

    ...

    // then the StackableFrame
    StackableFrame* frame = new StackableFrame(this);
    // Get the layout id
    int id = layout->addWidget(frame);
    // set the id to the frame
    frame->setId(id);
    // connect the setTop(int) to the setCurrentIndex(int) method.
    connect(frame, &StackableFrame::setTop, layout, &QStackedLayout::setCurrentIndex);
  \endcode

  Please note that the setTop() signal can be sent by calling the setFrameToTop()
  method from within your code. Alternatively you can emit the signal manually,
  passing the id obtained by the id() method.

  \param parent
*/
StackableFrame::StackableFrame(QWidget* parent)
  : QFrame(parent)
{

}

StackableFrame::StackableFrame(const StackableFrame& frame)
  : QFrame(dynamic_cast<QWidget*>(frame.parent()))
{
  this->m_id = frame.m_id;
}

StackableFrame::~StackableFrame() {}

/*!
   \brief Gets the id.

   \return an int id value.
*/
int StackableFrame::id() const
{
  return m_id;
}

/*!
   \brief Sets the id.

   \param id an int id value.
*/
void StackableFrame::setId(int id)
{
  m_id = id;
}

/*!
   \brief Send the setTop(id) message.
*/
void StackableFrame::setFrameToTop()
{
  emit setTop(m_id);
}
