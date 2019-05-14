#ifndef OCRIMAGE_H
#define OCRIMAGE_H

#include <QWidget>
#include <QList>

#include "basescanimage.h"

class OcrImage: public BaseScanImage
{
  Q_OBJECT

  enum Operations
  {
    BINARISE,
    CROP,
  };

public:
  explicit OcrImage(QWidget* parent = nullptr);
  ~OcrImage() = default;

  void setImage(const QImage& image);

  void undoAllChanges();
  void undoLastChange();

  void cropToSelection();

  void binarise();
  void setThreshold(int thresh_value = 100);
  void acceptThreshold();
  void applyThreshold();
  void cancelThreshold();

signals:
  void thresholdAccepted();

protected:
  Mat m_mat_image, *m_modifiable{};
  int m_modifiable_int;
  QList<Operations> m_operations;
  QList<QVariant> m_op_data;
  QList<QImage> m_op_images;


};

#endif // OCRIMAGE_H
