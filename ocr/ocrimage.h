#ifndef OCRIMAGE_H
#define OCRIMAGE_H

#include <QList>
#include <QWidget>
#include <QtDebug>

#include <vector>

#include "basescanimage.h"

class OcrImage: public BaseScanImage
{
  Q_OBJECT

public:
  explicit OcrImage(QWidget* parent = nullptr);
  ~OcrImage() override = default;

  void undoAllChanges();
  void undoLastChange();

  void cutSelection();
  void cropToSelection();

  void binarise();
  //  void setThreshold(int thresh_value = 100);
  void acceptThreshold();
  void applyThreshold(int value);
  void cancelThreshold();
  void undoLast();

  void invert();

  void denoise();

  void descew();

  // 180 gives a good starter value.
  static const int BASE_THRESHOLD = 180;

  bool inverted() const;
  void setInverted(bool inverted);

  signals:
  void binariseCompleted();
  void disableBinarise();
  void enableBinarise();

  protected:
  //  QImage m_temp_image;
  //  QList<Operations> m_operations;
  //  QList<QVariant> m_op_data;
  QList<QImage> m_op_images;
  int m_binarise_changes{};
  bool m_inverted;
  bool m_binarised;

  //  double houghTransform(Mat &im /*, Mat& orig*/);
};

#endif // OCRIMAGE_H
