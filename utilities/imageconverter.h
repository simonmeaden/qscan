#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H

#include "leptonica/allheaders.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"

#include <QImage>
#include <QPixmap>
#include <QtDebug>

#include "docutil.h"

/*
   Endianness
   ---

   Although not totally clear from the docs, some of QImage's formats we use here are
   endian-dependent. For example:

      Little Endian
         QImage::Format_ARGB32 the bytes are ordered:    B G R A
         QImage::Format_RGB32 the bytes are ordered:     B G R (255)
         QImage::Format_RGB888 the bytes are ordered:    R G B

      Big Endian
         QImage::Format_ARGB32 the bytes are ordered:    A R G B
         QImage::Format_RGB32 the bytes are ordered:     (255) R G B
         QImage::Format_RGB888 the bytes are ordered:    R G B

   Notice that Format_RGB888 is the same regardless of endianness. Since OpenCV
   expects (B G R) we need to swap the channels for this format.

   This is why some conversions here swap red and blue and others do not.

   This code assumes little endian. It would be possible to add conversions for
   big endian machines though. If you are using such a machine, please feel free
   to submit a pull request on the GitHub page.
*/
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
  #error                                                                         \
  "Some of QImage's formats are endian-dependant. This file assumes little endian. See comment at top of header."
#endif

/*!
  Functions to convert between various image formats to/from QImage and QPixmap.

  Supported formats are:
  \list
  \li OpenCV Mat
  \endlist

  QImage <-> Mat - Thanks to Andy Maloney <asmaloney@gmail.com> from his tutorial
  https://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap.

*/
namespace ImageConverter {

/*!
   \brief Converts a QImage object to an OpenCV Mat format image.

  By default image data is cloned to avoid any memory issues caused by
  deleting the Mat image before finishing with the QImage. If you know that you
  will be keeping the QImage until after finishing with the Mat then set this
  flag to false.

  \see matToImage
  \see isEquals
*/
cv::Mat imageToMat(const QImage& image_in, bool clone_image_data = true);
QImage matToImage(const cv::Mat& mat);

cv::Mat pixmapToMat(const QPixmap& pixmap, bool clone_image_data = true);
QPixmap matToPixmap(const cv::Mat& mat);

PIX* imageToPix(const QImage& image);
QImage pixToImage(PIX* pixImage);

bool isEqual(const cv::Mat& mat1, const cv::Mat& mat2);
bool isEqual(const QImage& img1, const QImage& img2);
bool isEqual(PIX* img1, PIX* img2);

// QImage fromIplImage(const IplImage* iplImg);
// IplImage* toIplImage(const QImage& image);

} // namespace ImageConverter

#endif // IMAGECONVERTER_H
