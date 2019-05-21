#ifndef OCRIMAGE_H
#define OCRIMAGE_H

#include <QWidget>
#include <QList>

#include "basescanimage.h"

class OcrImage: public BaseScanImage
{
  Q_OBJECT

  //  enum Operations
  //  {
  //    BINARISE,
  //    CUT,
  //    CROP,
  //    DENOISE,
  //    INVERT,
  //  };

public:
  explicit OcrImage(QWidget* parent = nullptr);
  ~OcrImage() = default;

  //  void setImage(const QImage& image);

  void undoAllChanges();
  void undoLastChange();

  void cutSelection();
  void cropToSelection();

  void binarise();
  void setThreshold(int thresh_value = 100);
  void acceptThreshold();
  void applyThreshold();
  void cancelThreshold();

  void invert();

  void denoise();

signals:
  void thresholdAccepted();
  void disableBinarise();
  void enableBinarise();

protected:
  Mat m_mat_image;
  Mat* m_modifiable_mat{};
  int m_modifiable_int{};
  //  QList<Operations> m_operations;
  //  QList<QVariant> m_op_data;
  QList<QImage> m_op_images;


};

#endif // OCRIMAGE_H
