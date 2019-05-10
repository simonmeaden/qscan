#ifndef OCRIMAGE_H
#define OCRIMAGE_H

#include <QWidget>

#include "basescanimage.h"



class OcrImage: public BaseScanImage
{
public:
  OcrImage(QWidget* parent = nullptr);
};

#endif // OCRIMAGE_H
