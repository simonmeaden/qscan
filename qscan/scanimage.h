#ifndef SCANIMAGE_H
#define SCANIMAGE_H

#include <QApplication>
#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QPoint>
#include <QtMath>

#include "log4qt/logger.h"

class ScanEditor;

class ScanImage : public QLabel
{
  Q_OBJECT
public:
  explicit ScanImage(QWidget* parent);

  void setImage(const QImage& image);
  //  void rotateBy(int angle);
  void rotateBy(qreal angle);
  void rotateByEdge();
  void scaleBy();
  void save();
  void saveAs();
  void zoomIn();
  void zoomOut();
  void fitBest();
  void fitHeight();
  void fitWidth();

  QSize minimumSizeHint() const override;
  QSize sizeHint() const override;
  bool hasSelection();
  QRect selection();
  void cropToSelection();
  void copySelection();
  void selectAll();
  void clearSelection();

signals:
  void selected();
  void unselected();
  void selectionUnderway();
  void imageIsLoaded();
  void adjustScrollbar(qreal);

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
    EDGE_STARTING,
    EDGE_DRAWING,
    DOING_NOTHING,
  };
  Log4Qt::Logger* m_logger;
  QImage m_image;
  QImage m_scaled_image;
  qreal m_scale_by;
  QMatrix m_matrix;
  qreal m_rotation;
  ScanEditor* m_editor;

  int m_rb_start_x;
  int m_rb_start_y;
  int m_rb_end_x;
  int m_rb_end_y;
  QRect m_rubber_band;
  QRect m_stretched_band;
  QRect m_scaled_rect;
  bool m_is_inside;
  State m_state;
  QString m_filename;
  QPoint m_edge_start;
  QPoint m_edge_finish;
  bool m_mouse_moved;
  void paintEvent(QPaintEvent* event) override;
  void enterEvent(QEvent*) override;
  void leaveEvent(QEvent*) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

  void paintRubberBand(QPainter* painter);
  QPointF getRotatedPoint(QPointF p, QPointF center, qreal angleRads);
  QRectF getMinimumBoundingRect(QRect r, qreal angleRads);
  qreal getMax(qreal a, qreal b, qreal c, qreal d);
  void rotateUsingEdge();
  void scaleImage(qreal factor);

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

#endif // SCANIMAGE_H
