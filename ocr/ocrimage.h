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

  void binarise();
  //  void setThreshold(int thresh_value = 100);
  void acceptChanges();
  void applyThreshold(int value);
  void applyRescale(double value);
  void cancelChanges();

  void denoise();

  void deskew();

  // 180 gives a good starter value.
  static const int BASE_THRESHOLD = 180;
  static const int BASE_RESCALE = 1.0;

  bool isInverted() const;
  void setInverted(bool isInverted);

  signals:
  void enableModification();

  protected:
  //  QImage m_temp_image;
  //  QList<Operations> m_operations;
  //  QList<QVariant> m_op_data;
  int m_changes{};
  QList<QLineF> m_lines;
  qreal m_angle{};
  //  bool m_binarised;

  void paintEvent(QPaintEvent *event) override;
  //  double houghTransform(Mat &im /*, Mat& orig*/);
  void clearChanges();
};

#endif // OCRIMAGE_H
