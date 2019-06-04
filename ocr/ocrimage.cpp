#include "ocrimage.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "imageconverter.h"

using namespace cv;

OcrImage::OcrImage(QWidget* parent)
  : BaseScanImage(parent)
{
  setStyleSheet("border: 2px solid red;");
}

void OcrImage::undoAllChanges()
{
  // TODO undo LAST change.
  BaseScanImage::undoAllChanges();
  //  m_mat_image = ImageConverter::imageToMat(m_image);
}

void OcrImage::undoLastChange()
{
  // TODO undo last change.
}

void OcrImage::cutSelection()
{
  m_op_images.append(m_modified_image);
  BaseScanImage::cutSelection();
}

void OcrImage::cropToSelection()
{
  m_op_images.append(m_modified_image);
  BaseScanImage::cropToSelection();
}

void OcrImage::binarise()
{
  m_op_images.append(m_modified_image);
  m_modifiable_int = 200;

  // create a modifiable image.
  m_modifiable_mat = new Mat(ImageConverter::imageToMat(m_modified_image));
  // convert to grayscale
  cvtColor(*m_modifiable_mat, *m_modifiable_mat, CV_BGR2GRAY);
  QImage image = ImageConverter::matToImage(*m_modifiable_mat);
  m_modified_image = image;
  //  updateImage();
  scaleModifiedImage();
  emit disableBinarise();
}

void OcrImage::acceptThreshold()
{
  delete m_modifiable_mat;
  emit thresholdAccepted();
}

void OcrImage::applyThreshold()
{
  Mat modified;
  threshold(*m_modifiable_mat, modified, m_modifiable_int, 255, THRESH_BINARY);
  QImage image = ImageConverter::matToImage(modified);
  m_modified_image = image;
  //  updateImage();
  scaleModifiedImage();
}

void OcrImage::cancelThreshold()
{
  // update to the pre-biniarise image
  m_modified_image = m_op_images.takeLast();
  emit enableBinarise();
  //  updateImage();
  scaleModifiedImage();
  delete m_modifiable_mat;
}

void OcrImage::invert()
{
  m_op_images.append(m_modified_image);

  // create a modifiable image.
  Mat modified = ImageConverter::imageToMat(m_modified_image);
  bitwise_not(modified, modified);
  QImage image = ImageConverter::matToImage(modified);
  m_modified_image = image;
  //  updateImage();
  scaleModifiedImage();
}

void OcrImage::setThreshold(int thresh_value)
{
  // modify modifiable image to new threshold
  m_modifiable_int = thresh_value;
}

void OcrImage::denoise()
{
  m_op_images.append(m_modified_image);
  m_modifiable_int = 10; // a reasonable value

  // create a modifiable image.
  m_modifiable_mat = new Mat(ImageConverter::imageToMat(m_modified_image));

  if (m_modifiable_mat->type()) {

  }
}



