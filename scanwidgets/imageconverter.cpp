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
      qWarning() << QString("ImageConverter::imageToMat() - Conversion requires cloning so we "
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
      qWarning() << QString("ImageConverter::imageToMat() - Conversion requires cloning so we "
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
  case QImage::Format_Grayscale8:
  case QImage::Format_Indexed8: {
    cv::Mat mat(image.height(),
                image.width(),
                CV_8UC1,
                const_cast<uchar*>(image.bits()),
                static_cast<size_t>(image.bytesPerLine()));

    return (clone_image_data ? mat.clone() : mat);
  }

    //  case QImage::Format_Grayscale8:
    //    cv::Mat mat(image.height(),
    //                image.width(),
    //                CV_8UC1,)

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
QImage matToImage(const cv::Mat &mat)
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
QPixmap matToPixmap(const cv::Mat &mat)
{
  return QPixmap::fromImage(matToImage(mat));
}

PIX *imageToPix(const QImage &image)
{
  PIX *pixs;
  quint32 *lines;

  QImage swapped = image.rgbSwapped();
  int height = swapped.height();
  int bytes_per_line = swapped.bytesPerLine();
  int words_per_line = bytes_per_line / 4;

  pixs = pixCreate(swapped.width(), height, swapped.depth());
  pixSetWpl(pixs, words_per_line);
  pixSetColormap(pixs, nullptr);

  // This is really just for my use as I am storing the
  // images in png files and QImage doesn't store that info.
  pixSetInputFormat(pixs, IFF_PNG);

  // set the xres and yres values.
  pixSetXRes(pixs, ImageConverter::dpmToDpi(image.dotsPerMeterX()));
  pixSetYRes(pixs, ImageConverter::dpmToDpi(image.dotsPerMeterY()));

  quint32 *datas = pixs->data;

  for (int y = 0; y < height; y++) {
    lines = datas + y * words_per_line;
    QByteArray a((const char *) swapped.scanLine(y), bytes_per_line);

    for (int j = 0; j < a.size(); j++) {
      *((quint8 *) (lines) + j) = a[j];
    }
  }

  return pixEndianByteSwapNew(pixs);
}

QImage pixToImage(PIX *pixImage)
{
  int width = pixGetWidth(pixImage);
  int height = pixGetHeight(pixImage);
  int depth = pixGetDepth(pixImage);
  int bytesPerLine = pixGetWpl(pixImage) * 4;
  quint32 *s_data = pixGetData(pixEndianByteSwapNew(pixImage));
  PixColormap *pix_color_map = pixImage->colormap;

  QImage::Format format;

  if (depth == 1) {
    format = QImage::Format_Mono;

  } else if (depth == 8) {
    format = QImage::Format_Indexed8;

  } else {
    format = QImage::Format_RGB32;
  }

  QImage result((uchar *) s_data, width, height, bytesPerLine, format);

  result.setDotsPerMeterX(ImageConverter::dpiToDpm(pixImage->xres));
  result.setDotsPerMeterY(ImageConverter::dpiToDpm(pixImage->yres));

  if (pix_color_map != nullptr) {
    qint32 colors_allocated = pix_color_map->nalloc;
    auto *pix_ct_array = static_cast<RGBA_QUAD *>(pix_color_map->array);

    QVector<QRgb> color_table(colors_allocated);

    for (int i = 0; i < colors_allocated; i++) {
      RGBA_Quad pix_ct_value = pix_ct_array[i];
      unsigned int image_ct_value = 0xff000000 + pix_ct_value.blue + (pix_ct_value.green << 8)
                                    + (pix_ct_value.red << 16);
      color_table.insert(i, image_ct_value);
    }

    result.setColorTable(color_table);
  }

  if (result.isNull()) {
    static QImage none(0, 0, QImage::Format_Invalid);
    qWarning() << "***Invalid format!!!";
    return none;
  }

  return result.rgbSwapped();
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

  \param img1 - a QImage object.
   \param img2 - a QImage object.
   \return true if both images are the same, otherwise false.
*/
bool isEqual(const QImage& img1, const QImage& img2)
{
  // only included this for completeness.
  return (img1 == img2);
}

/*!
   \brief Checks two PIX image objects for equality.

  \param pix1 - a Leptonica PIX object.
   \param pix2 - a Leptonica PIX object.
   \return true if both images are the same, otherwise false.
*/
bool isEqual(PIX *pix1, PIX *pix2)
{
  int same;
  int result;

  if ((pix1->colormap != nullptr && pix2->colormap != nullptr) && (pix1->d == pix2->d)) {
    result = pixEqualWithCmap(pix1, pix2, &same);

  } else {
    result = pixEqual(pix1, pix2, &same);
  }

  if (result == 1) { // 1 == error in comparison.
    return false;
  }

  return (same == 1);
}

/*!
  \brief Convert a dots-per-inch value to dots-per-meter.

  Most images use a dots-per-inch value, however Qt at least,
  probably others as well, use a dots-per-meter value.

  \param value the dpi value.
  \return an integer ppm value;
*/
int dpiToDpm(int value)
{
  int ppm = int(double(value) / 25.4) * 1000.0;
  return ppm;
}

/*!
  \brief Convert a  dots-per-meter value to dots-per-inch.

  Most images use a dots-per-inch value, however Qt at least,
  probably others as well, use a dots-per-meter value.

  \param value the ppm value.
  \return an integer dpi value;
*/
int dpmToDpi(int value)
{
  int dpi = int(double(value) * 25.4) / 1000.0;
  return dpi;
}

} // namespace ImageConverter

#include "imageconverter.h"
