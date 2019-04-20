#ifndef SCANIMAGE_H
#define SCANIMAGE_H

#include <QClipboard>
#include <QtWidgets>

#include "log4qt/logger.h"

class ScanImage : public QLabel
{
  Q_OBJECT
public:
  ScanImage(QWidget* parent);

  void
  setImage(const QImage& image);
  void
  rotateBy(int angle);

  QSize
  minimumSizeHint() const override;
  QSize
  sizeHint() const override;
  bool
  hasSelection();
  QRect
  selection();
  void
  cropToSelection();
  void
  copySelection();
  void
  selectAll();
  void
  clearSelection();

protected:
  enum State
  {
    STRETCH_TOP,
    STRETCH_TOPLEFT,
    STRETCH_TOPRIGHT,
    STRETCH_BOTTOM,
    STRETCH_BOTTOMLEFT,
    STRETCH_BOTTOMRIGHT,
    STRETCH_LEFT,
    STRETCH_RIGHT,
    RUBBER_BANDING,
    RB_COMPLETE,
    RB_MOVE,
    RB_NOTHING,
  };
  Log4Qt::Logger* m_logger;
  QImage m_image;
  qreal m_scaled_by;
  qreal m_angle;
  int m_rb_start_x, m_rb_start_y, m_rb_end_x, m_rb_end_y;
  QRect m_rubber_band, m_stretched_band, m_scaled_rect;
  bool m_is_inside;
  State m_state;

  void
  paintEvent(QPaintEvent*) override;
  void
  enterEvent(QEvent*) override;
  void
  leaveEvent(QEvent*) override;
  void
  mousePressEvent(QMouseEvent* event) override;
  void
  mouseReleaseEvent(QMouseEvent* event) override;
  void
  mouseMoveEvent(QMouseEvent* event) override;
  void
  paintRubberBand(QPainter* painter);
  QPointF
  getRotatedPoint(QPointF p, QPointF center, qreal angleRads);
  QRectF
  getMinimumBoundingRect(QRect r, qreal angleRads);
  qreal
  getMax(qreal a, qreal b, qreal c, qreal d);
};

#endif // SCANIMAGE_H
