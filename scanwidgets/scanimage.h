#ifndef SCANIMAGE_H
#define SCANIMAGE_H

#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QPoint>
#include <QtMath>

#include "logger.h"

#include "scanwidgets_global.h"

class ScanEditor;

class SCANWIDGETSSHARED_EXPORT ScanImage : public QLabel
{
  Q_OBJECT
public:
  explicit ScanImage(QString  datadir, QWidget* parent);

  void setImage(const QImage& image);
  //  void rotateBy(int angle);
  void cropToContent();
  void rotate180();
  void rotateCW();
  void rotateACW();
  void rotateByAngle();
  void rotateByEdge();
  void rotateBy(qreal angle);
  void scaleBy();
  void save();
  //  void save(const QString& doc_name = QString());
  void saveAs();
  void saveAsCover();
  void zoomIn();
  void zoomOut();
  void fitBest();
  void fitHeight();
  void fitWidth();
  void rescan();
  void scale();

  QSize minimumSizeHint() const override;
  QSize sizeHint() const override;
  bool hasSelection();
  QRect selection();
  void cropToSelection();
  void copySelection();
  void selectAll();
  void clearSelection();
  void setDefaultPageCropSize();
  QPair<QImage, QImage> splitPages();
  QImage splitLeftPage();
  QImage splitRightPage();
  QImage makePage();

signals:
  void selected();
  void unselected();
  void selectionUnderway();
  void imageIsLoaded();
  void adjustScrollbar(qreal);
  void sendImage(QImage);
  void sendImages(QImage, QImage);
  void sendCover(const QImage&);

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
  QString m_datadir;
  QImage m_image;
  qreal m_scale_by;
  QMatrix m_matrix;
  qreal m_rotation;

  int m_rb_start_x;
  int m_rb_start_y;
  int m_rb_end_x;
  int m_rb_end_y;
  QRect m_rubber_band;
  QRect m_stretched_band;
  QRect m_default_crop_size;
  bool m_def_crop_set;
  bool m_is_inside;
  State m_state;
  QString m_filename;
  QPoint m_edge_start;
  QPoint m_edge_finish;
  bool m_mouse_moved;

  void contextMenuEvent(QContextMenuEvent* event) override;
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
  void enableSetDefaultCropSize();
  void disableSetDefaultCropSize();
  void enableSelectionActions();
  void disableSelectionActions();
  void enableNoSelectionActions();
  void disableNoSelectionActions();

  //  QAction* m_cut_act;
  QAction* m_copy_selection_act{};
  QAction* m_crop_to_selection_act{};
  QAction* m_clear_selection_act{};
  QAction* m_crop_to_content_act{};
  QAction* m_rotate_cw_act{};
  QAction* m_rotate_acw_act{};
  QAction* m_rotate_180_act{};
  QAction* m_rotate_by_angle_act{};
  QAction* m_rotate_by_edge_act{};
  QAction* m_rescan_act{};
  QAction* m_scale_act{};
  QAction* m_selectall_act{};
  QAction* m_save_act{};
  QAction* m_save_as_act{};
  QAction* m_save_as_cover_act{};
  QAction* m_set_def_crop_act{};
  QAction* m_split_pages_act{};
  QAction* m_split_right_act{};
  QAction* m_split_left_act{};
  QAction* m_make_page_act{};

  void initActions();

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
