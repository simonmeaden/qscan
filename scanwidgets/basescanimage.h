#ifndef BASESCANIMAGE_H
#define BASESCANIMAGE_H

#include <QLabel>
#include <QRect>
#include <QPoint>
#include <QtMath>

#include "logger.h"

class BaseScanImage : public QLabel
{
  Q_OBJECT
public:
  explicit BaseScanImage(QWidget* parent = nullptr);

  bool hasSelection();
  void cropToSelection();
  void setImage(const QImage& image);
  void fitBest();
  void fitHeight();
  void fitWidth();
  void clearSelection();
  void rotateBy(qreal angle);

signals:
  void imageIsLoaded();
  void adjustScrollbar(qreal);
  void selected();
  void unselected();
  void selectionUnderway();

public slots:

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
    RUBBERBANDING,
    RUBBERBAND_STARTING,
    RUBBERBAND_COMPLETE,
    RUBBERBAND_MOVE,
    EDGE_SELECTED,
    EDGE_STARTING,
    EDGE_DRAWING,
    DOING_NOTHING,
  };

  Log4Qt::Logger* m_logger;
  State m_state;
  QImage m_image;
  QRect m_rubber_band;
  QRect m_stretched_band;
  bool m_mouse_moved;
  int m_rb_start_x;
  int m_rb_start_y;
  int m_rb_end_x;
  int m_rb_end_y;
  QPoint m_edge_start;
  QPoint m_edge_finish;
  qreal m_scale_by;
  QRect m_default_crop_size;
  bool m_def_crop_set;
  bool m_is_inside;

  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void scaleImage(qreal factor);
  void rotateUsingEdge();

  static const int EDGE_WIDTH = 2;
  static const int RUBBERBAND_WIDTH = 2;
  static const int RUBBERBANDING_MIN = 2;
  static const QColor EDGE_COLOR;
  static const QColor EDGING_COLOR;
  static const QColor RUBBERBAND_COLOR;
  static const QColor RUBBERBANDING_COLOR;
  static const QBrush RUBBERBAND_BRUSH;
  static const qreal ZOOM_IN_FACTOR;
  static const qreal ZOOM_OUT_FACTOR;

};

#endif // BASESCANIMAGE_H
