#include "opencvimage.h"

Mat* OpencvImage::toMat(const QImage& image)
{
  QImage swapped = image.rgbSwapped();
  Mat* mat = new Mat(image.height(), image.width(), swapped.bits(), CV_8U, 3);
}

QImage OpencvImage::fromMat(Mat* mat)
{
  // swap BGR (Mat) to RGB (QImage)
  //  cvtColor(*mat, *mat, CV_BGR2RGB);
  QImage image = QImage(mat->data, mat->cols, mat->rows, QImage::Format_RGB888).rgbSwapped();
}

IplImage* OpencvImage::toIplImage(const QImage& image)
{

  IplImage* imgHeader = cvCreateImageHeader(cvSize(image.width(), image.height()), IPL_DEPTH_8U, 4);
  imgHeader->imageData = (char*) image.bits();

  uchar* newdata = (uchar*) malloc(sizeof(uchar) * image.byteCount());
  memcpy(newdata, image.bits(), image.byteCount());
  imgHeader->imageData = (char*) newdata;

  return imgHeader;
}

QImage* OpencvImage::fromIplImage(const IplImage* iplImg)
{
  int h = iplImg->height;
  int w = iplImg->width;
  int channels = iplImg->nChannels;
  char* data = iplImg->imageData;

  QImage* image = new QImage(w, h, QImage::Format_ARGB32);

  for (int y = 0; y < h; y++, data += iplImg->widthStep) {
    for (int x = 0; x < w; x++) {
      char r, g, b, a = 0;

      if (channels == 1) {
        r = data[x * channels];
        g = data[x * channels];
        b = data[x * channels];

      } else if (channels == 3 || channels == 4) {
        r = data[x * channels + 2];
        g = data[x * channels + 1];
        b = data[x * channels];
      }

      if (channels == 4) {
        a = data[x * channels + 3];
        image->setPixel(x, y, qRgba(r, g, b, a));

      } else {
        image->setPixel(x, y, qRgb(r, g, b));
      }
    }
  }

  return image;
}

QPixmap OpencvImage::convert16uc1(const cv::Mat& source)
{
  quint16* pSource = (quint16*) source.data;
  int pixelCounts = source.cols * source.rows;

  QImage dest(source.cols, source.rows, QImage::Format_RGB32);

  char* pDest = (char*) dest.bits();

  for (int i = 0; i < pixelCounts; i++) {
    quint8 value = (quint8)((*(pSource)) >> 8);
    *(pDest++) = value;  // B
    *(pDest++) = value;  // G
    *(pDest++) = value;  // R
    *(pDest++) = 0;      // Alpha
    pSource++;
  }

  return QPixmap::fromImage(dest);
}

QPixmap OpencvImage::convert8uc3(const cv::Mat& source)
{
  quint8* pSource = source.data;
  int pixelCounts = source.cols * source.rows;

  QImage dest(source.cols, source.rows, QImage::Format_RGB32);

  char* pDest = (char*) dest.bits();

  for (int i = 0; i < pixelCounts; i++) {
    *(pDest++) = *(pSource + 2);  // B
    *(pDest++) = *(pSource + 1);  // G
    *(pDest++) = *(pSource + 0);  // R
    *(pDest++) = 0;               // Alpha
    pSource += 3;
  }

  return QPixmap::fromImage(dest);
}

QPixmap OpencvImage::convert16uc3(const cv::Mat& source)
{
  quint16* pSource = (quint16*) source.data;
  int OpencvImage = source.cols * source.rows;

  QImage dest(source.cols, source.rows, QImage::Format_RGB32);

  char* pDest = (char*) dest.bits();

  for (int i = 0; i < pixelCounts; i++) {
    *(pDest++) = *(pSource + 2);  // B
    *(pDest++) = *(pSource + 1);  // G
    *(pDest++) = *(pSource + 0);  // R
    *(pDest++) = 0;               // Alpha
    pSource += 3;
  }

  return QPixmap::fromImage(dest);
}
