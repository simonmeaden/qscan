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

OcrImage::OcrImage(QWidget* parent)
  : BaseScanImage(parent)
    /*, m_binarised(false)*/
{
  //  setStyleSheet("border: 2px solid red;");
}

void OcrImage::undoAllChanges()
{
  // reverts to original image.
  BaseScanImage::undoAllChanges();
}

void OcrImage::clearToBackground()
{
  m_op_images.append(m_modified_image);
  BaseScanImage::clearToBackground();
}

void OcrImage::cropToSelection()
{
  m_op_images.append(m_modified_image);
  BaseScanImage::cropToSelection();
}

QImage OcrImage::copySelection()
{
  if (hasSelection()) {
    QRect copy_rect;
    copy_rect.setX(int(m_rubber_band.x() / m_scale_by));
    copy_rect.setY(int(m_rubber_band.y() / m_scale_by));
    copy_rect.setWidth(int(m_rubber_band.width() / m_scale_by));
    copy_rect.setHeight(int(m_rubber_band.height() / m_scale_by));

    QImage copied = m_modified_image.copy(copy_rect);

    clearSelection();
    return copied;
  }

  return QImage();
}

void OcrImage::binarise()
{
  if (m_modified_image.format() != QImage::Format_Mono
      && m_modified_image.format() != QImage::Format_Grayscale8) {
    // create a modifiable image.
    m_undo_list.insertMulti(BINARISE, m_modified_image);
    Mat mat = ImageConverter::imageToMat(m_modified_image);
    // convert to grayscale
    cvtColor(mat, mat, CV_BGR2GRAY);
    m_temp_image = ImageConverter::matToImage(mat);

    scaleTemporaryImage();
  }

}

void OcrImage::acceptChanges()
{
  m_modified_image = m_temp_image;
  m_temp_image = QImage();
  scaleModifiedImage();
}

void OcrImage::applyThreshold(int value)
{
  Mat mat = ImageConverter::imageToMat(m_modified_image);
  threshold(mat, mat, value, 255, THRESH_BINARY);
  m_temp_image = ImageConverter::matToImage(mat);
  m_temp_image = m_temp_image.convertToFormat(QImage::Format::Format_Mono);

  scaleTemporaryImage();
}

void OcrImage::rescale()
{
  m_undo_list.insertMulti(RESCALE, m_modified_image);
  m_temp_image = m_modified_image;
}

void OcrImage::applyRescale(double value)
{
  Mat mat = ImageConverter::imageToMat(m_modified_image);
  cv::resize(mat, mat, cv::Size(), value, value, CV_INTER_CUBIC);
  m_temp_image = ImageConverter::matToImage(mat);

  scaleTemporaryImage();
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
  scaleModifiedImage();
}

void OcrImage::denoise()
{
  m_undo_list.insertMulti(DENOISE, m_modified_image);
  m_temp_image = m_modified_image;
}

//void OcrImage::revertDenoise()
//{

//}

void OcrImage::applyDenoise(int filter_value, int template_value, int search_value)
{
  Mat mat = ImageConverter::imageToMat(m_modified_image);
  Mat dst;

  if (!(m_modified_image.format() == QImage::Format_Mono
        || m_modified_image.format() == QImage::Format_Grayscale8)) {

    cv::fastNlMeansDenoising(mat, dst, filter_value, template_value, search_value);

  }/* else {

  //    cv::fastNlMeansDenoisingColored(mat, dst, 10, 10, 7, 21);
  }*/

  m_temp_image = ImageConverter::matToImage(dst);
  scaleTemporaryImage();

}

void OcrImage::deskew()
{
  m_undo_list.insertMulti(DESKEW, m_modified_image);
  m_op_images.append(m_modified_image);
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

bool OcrImage::hasChanges()
{
  return (!m_undo_list.isEmpty());
}
