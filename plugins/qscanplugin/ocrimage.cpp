#include "ocrimage.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>

#include "imageconverter.h"

using namespace cv;

const qreal OcrImage::BASE_RESCALE = 1.0;
const QString OcrImage::BINARISE = tr("Binarise");
const QString OcrImage::RESCALE = tr("Rescale");
const QString OcrImage::DENOISE = tr("DeNoise");
const QString OcrImage::DESKEW = tr("Deskew");
const QString OcrImage::CLEAR = tr("Clear");
const QString OcrImage::CROP = tr("Crop");

OcrImage::OcrImage(QWidget* parent)
  : BaseScanImage(parent)
{
}

void OcrImage::undoAllChanges()
{
  // reverts to original image.
  BaseScanImage::undoAllChanges();
}

void OcrImage::clearToBackground()
{
  m_op_images.append(m_current_image);
  BaseScanImage::clearToBackground();
  m_image_changed = true;
  //  emit imageHasChanged(m_image_changed);
}

void OcrImage::cropToSelection()
{
  m_op_images.append(m_current_image);
  BaseScanImage::cropToSelection();
  m_image_changed = true;
  //  emit imageHasChanged(m_image_changed);
}

QImage OcrImage::copySelection()
{
  if (hasSelection()) {
    QRect copy_rect;
    copy_rect.setX(int(m_rubber_band.x() / m_scale_by));
    copy_rect.setY(int(m_rubber_band.y() / m_scale_by));
    copy_rect.setWidth(int(m_rubber_band.width() / m_scale_by));
    copy_rect.setHeight(int(m_rubber_band.height() / m_scale_by));

    QImage copied = m_current_image.copy(copy_rect);

    clearSelection();
    return copied;
  }

  return QImage();
}

void OcrImage::greyscale()
{
  if (m_current_image.format() != QImage::Format_Grayscale8) {
    // create a modifiable image.
    m_undo_list.insertMulti(BINARISE, m_current_image);
    // let's use QImages convertToFormat() method rather than more
    // costly conversions to/from Mat
    m_temp_image = m_current_image.convertToFormat(QImage::Format_Grayscale8);
    //    Mat mat = ImageConverter::imageToMat(m_current_image);
    //    // convert to grayscale
    //    cvtColor(mat, mat, CV_BGR2GRAY);
    //    m_temp_image = ImageConverter::matToImage(mat);
    scaleTemporaryImage();
    m_current_image = m_temp_image;
  }

}

void OcrImage::invert()
{
  m_op_images.append(m_current_image);

  // create a modifiable image.
  cv::Mat modified = ImageConverter::imageToMat(m_current_image);
  cv::bitwise_not(modified, modified);
  QImage image = ImageConverter::matToImage(modified);
  m_current_image = image;
  m_inverted = !m_inverted;
  m_image_changed = true;
  //  emit imageHasChanged(m_image_changed);
  scaleCurrentImage();
}

void OcrImage::acceptChanges()
{
  //  m_temp_image.save("temp_image.png", "PNG");
  m_current_image = m_temp_image;
  //  m_temp_image.save("current.png", "PNG");
  m_temp_image = QImage();
  scaleCurrentImage();
}

void OcrImage::applyThreshold(int value)
{
  if (m_temp_image.format() == QImage::Format_Mono) {
    m_temp_image = m_temp_image.convertToFormat(QImage::Format::Format_Grayscale8);
  }

  Mat mat = ImageConverter::imageToMat(m_current_image);
  threshold(mat, mat, value, 255, THRESH_BINARY);
  m_temp_image = ImageConverter::matToImage(mat);
  m_image_changed = true;
  //  emit imageHasChanged(m_image_changed);
  scaleTemporaryImage();
}

void OcrImage::rescale()
{
  m_undo_list.insertMulti(RESCALE, m_current_image);
  m_temp_image = m_current_image;
}

void OcrImage::applyRescale(double value)
{
  Mat mat = ImageConverter::imageToMat(m_current_image);
  cv::resize(mat, mat, cv::Size(), value, value, CV_INTER_CUBIC);
  m_temp_image = ImageConverter::matToImage(mat);

  scaleTemporaryImage();
  m_image_changed = true;
  //  emit imageHasChanged(m_image_changed);
  emit imageSizeChanged(m_temp_image.width(),
                        m_temp_image.height(),
                        Util::dpmToDpi(m_temp_image.dotsPerMeterX()),
                        Util::dpmToDpi(m_temp_image.dotsPerMeterY()));
}

void OcrImage::scaleTemporaryImage()
{
  int w = int(m_temp_image.width() * m_scale_by);
  int h = int(m_temp_image.height() * m_scale_by);
  m_display_image = m_temp_image.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  update();
}

void OcrImage::cancelChanges()
{
  m_image_changed = false;
  scaleCurrentImage();
}

void OcrImage::denoise()
{
  m_undo_list.insertMulti(DENOISE, m_current_image);
  m_temp_image = m_current_image;
}

//void OcrImage::revertDenoise()
//{

//}

void OcrImage::applyDenoise(int filter_value, int template_value, int search_value)
{
  Mat mat = ImageConverter::imageToMat(m_current_image);
  Mat dst(mat.rows, mat.cols, mat.type());

  if (/*m_current_image.format() == QImage::Format_Mono
      || */m_current_image.format() == QImage::Format_Grayscale8) {

    cv::fastNlMeansDenoising(mat, dst, filter_value, template_value, search_value);
    m_image_changed = true;
    //    emit imageHasChanged(m_image_changed);
  }/* else {

    cv::fastNlMeansDenoisingColored(mat, dst, 10, 10, 7, 21);
    m_image_changed=true;
  }*/

  m_temp_image = ImageConverter::matToImage(dst);
  scaleTemporaryImage();

}

void OcrImage::deskew()
{
  m_undo_list.insertMulti(DESKEW, m_current_image);
  m_op_images.append(m_current_image);
  rotateByEdge();
}

bool OcrImage::isInverted() const
{
  return m_inverted;
}

void OcrImage::setInverted(bool inverted)
{
  m_inverted = inverted;
}

bool OcrImage::isImageChanged() const
{
  return m_image_changed;
}

void OcrImage::setImageChanged(bool image_changed)
{
  m_image_changed = image_changed;
}
