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

#include "widgets_global.h"
#include "basescanimage.h"

namespace QScanner {

class WIDGETSSHARED_EXPORT OcrImage: public BaseScanImage
{
  Q_OBJECT

public:
  explicit OcrImage(QWidget* parent = nullptr);
  ~OcrImage() override = default;

  void undoAllChanges();

  void clearToBackground();
  void cropToSelection();
  QImage copySelection();

  void greyscale();
  void monochrome();
  void applyThreshold(int value);
  void invert();

  void rescale();
  void applyRescale(double value);

  void acceptChanges();
  void cancelChanges();

  void denoise();
  void applyDenoise(int filter_value, int template_value, int search_value);
  void deskew();

  // 180 gives a good starter value.
  static const int BASE_THRESHOLD = 180;
  static const qreal BASE_RESCALE;
  static const int BASE_TEMPLATE = 7;
  static const int BASE_SEARCH = 21;
  static const int BASE_FILTER = 3;
  static const QString BINARISE;
  static const QString RESCALE;
  static const QString DENOISE;
  static const QString DESKEW;
  static const QString CLEAR;
  static const QString CROP;

  bool isInverted() const;
  void setInverted(bool isInverted);

  bool isImageChanged() const;
  void setImageChanged(bool image_changed);

signals:
  void enableModification();
  void imageSizeChanged(int w, int h, int xres, int yres);

protected:
  QImage m_temp_image; // temporary image for display purposes.
  QMap<QString, QImage> m_undo_list;
  bool m_image_changed{};


  void scaleTemporaryImage();
};

} // end of namespace QScanner

#endif // OCRIMAGE_H
