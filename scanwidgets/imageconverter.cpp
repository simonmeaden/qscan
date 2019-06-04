#include "imageconverter.h"

namespace ImageConverter {

/*!
   \brief Converts a QImage to OpenCV Mat.

  By default to image data is cloned to avoid any memory issues caused by
  deleting the Mat image before finishing with the QImage. If you know that you
  will be keeping the QImage until after finishing with the Mat then set this
  flag to false.

   \param image - the QImage object to convert.
   \param clone_image_data - clone the image data (default true).
   \return an OpenCV Mat object.
  \see matToImage
  \see isEquals
*/
cv::Mat imageToMat(const QImage& image, bool clone_image_data)
{
  // Not all cases are covered.
  switch (image.format()) {
  // 8-bit, 4 channel
  case QImage::Format_ARGB32:
  case QImage::Format_ARGB32_Premultiplied: {
    cv::Mat mat(image.height(),
                image.width(),
                CV_8UC4,
                const_cast<uchar*>(image.bits()),
                static_cast<size_t>(image.bytesPerLine()));

    return (clone_image_data ? mat.clone() : mat);
  }

  // 8-bit, 3 channel
  case QImage::Format_RGB32: {
    if (!clone_image_data) {
      qWarning() << QString(
          "ImageConverter::imageToMat() - Conversion requires cloning so we "
          "don't modify the original QImage data");
    }

    cv::Mat mat(image.height(),
                image.width(),
                CV_8UC4,
                const_cast<uchar*>(image.bits()),
                static_cast<size_t>(image.bytesPerLine()));

    cv::Mat matNoAlpha;

    cv::cvtColor(mat, matNoAlpha, cv::COLOR_BGRA2BGR); // drop the all-white alpha channel

    return matNoAlpha;
  }

  // 8-bit, 3 channel
  case QImage::Format_RGB888: {
    if (!clone_image_data) {
      qWarning() << QString(
          "ImageConverter::imageToMat() - Conversion requires cloning so we "
          "don't modify the original QImage data");
    }

    QImage swapped = image.rgbSwapped();

    return cv::Mat(swapped.height(),
                   swapped.width(),
                   CV_8UC3,
                   const_cast<uchar*>(swapped.bits()),
                   static_cast<size_t>(swapped.bytesPerLine()))
           .clone();
  }

  // 8-bit, 1 channel
  case QImage::Format_Indexed8: {
    cv::Mat mat(image.height(),
                image.width(),
                CV_8UC1,
                const_cast<uchar*>(image.bits()),
                static_cast<size_t>(image.bytesPerLine()));

    return (clone_image_data ? mat.clone() : mat);
  }

  default:
    qWarning() << QString("ImageConverter::imageToMat() - QImage format not "
                          "handled in switch : %1")
                      .arg(image.format());
    break;
  }

  return cv::Mat();
}



/*!
   \brief Converts a QPixmap to OpenCV Mat.

  By default to image data is cloned to avoid any memory issues caused by deleting
    the Mat image before finishing with the QImage. If you know that you will be
      keeping the QImage until after finishing with the Mat then set this flag to false.

   \param pixmap - the OpenCV Mat to convert.
   \param clone_image_data - clone the image data (default true).
   \return a QImage object.
*/
cv::Mat pixmapToMat(const QPixmap& pixmap, bool clone_image_data)
{
  return imageToMat(pixmap.toImage(), clone_image_data);
}



/*!
   \brief Converts an OpenCV Mat image to a QImage.

  No need to worry about cloning data here as QImage uses implicit data sharing.

   \param pixmap - the OpenCV Mat to convert.
   \return a QImage object.
*/
QImage matToImage(cv::Mat mat)
{
  switch (mat.type()) {
  // 8-bit, 4 channel
  case CV_8UC4: {
    QImage image(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_ARGB32);

    return image;
  }

  // 8-bit, 3 channel
  case CV_8UC3: {
    QImage image(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888);

    return image.rgbSwapped();
  }

  // 8-bit, 1 channel
  case CV_8UC1: {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    QImage image(
      mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Grayscale8);
#else
    static QVector<QRgb> sColorTable;

    // only create our color table the first time
    if (sColorTable.isEmpty()) {
      sColorTable.resize(256);

      for (int i = 0; i < 256; ++i) {
        sColorTable[i] = qRgb(i, i, i);
      }
    }

    QImage image(
      inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_Indexed8);

    image.setColorTable(sColorTable);
#endif

    return image;
  }

  default:
    qWarning() << QString("ASM::cvMatToQImage() - cv::Mat image type not "
                          "handled in switch: %1")
                      .arg(mat.type());
    break;
  }

  return QImage();
}



/*!
   \brief Converts an OpenCV Mat image to a QPixmap.

  No need to worry about cloning data here as QImage uses implicit data sharing.

   \param pixmap - the OpenCV Mat to convert.
   \return a QImage object.
*/
QPixmap matToPixmap(cv::Mat mat)
{
  return QPixmap::fromImage(matToImage(mat));
}



/*!
   \brief Checks two OpenCV Mat objects for equality.

  \param mat1 - an OpenCV Mat object.
   \param mat2 - an OpenCV Mat object.
   \return true if both images are the same, otherwise false.
*/
bool isEqual(const cv::Mat& mat1, const cv::Mat& mat2)
{
  if (mat1.dims == mat2.dims && mat1.size == mat2.size && mat1.elemSize() == mat2.elemSize()) {
    if (mat1.isContinuous() && mat2.isContinuous()) {
      return 0 == memcmp(mat1.ptr(), mat2.ptr(), mat1.total() * mat1.elemSize());
    }

    const cv::Mat* arrays[] = { &mat1, &mat2, nullptr };
    uchar* ptrs[2];
    cv::NAryMatIterator it(arrays, ptrs, 2);

    for (unsigned int p = 0; p < it.nplanes; p++, ++it)
      if (0 != memcmp(it.ptrs[0], it.ptrs[1], it.size * mat1.elemSize())) {
        return false;
      }

    return true;
  }

  return false;
}



/*!
   \brief Checks two QImage objects for equality.

  This method was included only for completeness, but is not really needed as Qt has
  included an = operator so
  \code
    if (img1 == img2) {
    ...
    }
  \endcode
  will work perfectly well. (That's all this method does internally.)

  \param mat1 - a QImage object.
   \param mat2 - an OpenCV Mat
   \return true if both images are the same, otherwise false.
*/
bool isEqual(const QImage& img1, const QImage& img2)
{
  // only included this for completeness.
  return (img1 == img2);
}

} // end of namespace

#include "imageconverter.h"
