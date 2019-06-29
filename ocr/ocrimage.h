#ifndef OCRIMAGE_H
#define OCRIMAGE_H

#include <QLineF>
#include <QList>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QWidget>
#include <QtDebug>
#include <QtMath>

#include <vector>

#include "basescanimage.h"

class OcrImage: public BaseScanImage
{
  Q_OBJECT

public:
  explicit OcrImage(QWidget* parent = nullptr);
  ~OcrImage() override = default;

  //  void setImage(const QImage &image) override;

  void undoAllChanges();
  void undoLastChange();

  void clearToBackground();
  void cropToSelection();
  QImage copySelection();

  void binarise();
  void applyThreshold(int value);

  void rescale();
  void revertRescale();
  void applyRescale(double value);

  void acceptChanges();
  void cancelCurrentChanges();

  void denoise();
  void deskew();

  void dumpImageChanges();

  // 180 gives a good starter value.
  static const int BASE_THRESHOLD = 180;
  static const qreal BASE_RESCALE;

  bool isInverted() const;
  void setInverted(bool isInverted);

  bool hasChanges();

signals:
  void enableModification();
  void imageSizeChanged(int w, int h, int xres, int yres);

protected:
  //  QImage m_temp_image;
  //  QList<Operations> m_operations;
  //  QList<QVariant> m_op_data;
  QImage m_image_base;
  int m_changes{};
  //  QList<QLineF> m_lines;
  //  qreal m_angle{};
  //  bool m_binarised;

  //  void paintEvent(QPaintEvent *event) override;
  //  double houghTransform(Mat &im /*, Mat& orig*/);
};

#endif // OCRIMAGE_H
