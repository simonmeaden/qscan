#include "ocrimage.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "imageconverter.h"

using namespace cv;

OcrImage::OcrImage(QWidget* parent)
  : BaseScanImage(parent)
{}

void OcrImage::setImage(const QImage& image)
{
  BaseScanImage::setImage(image);
  m_modified_image = image;
  //  m_mat_image = ImageConverter::imageToMat(m_image);
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

void OcrImage::cropToSelection()
{
  m_operations.append(CROP);
  m_op_data.append(selectionStart());
  m_op_images.append(m_modified_image);
  BaseScanImage::cropToSelection();
}

void OcrImage::binarise()
{
  // save undo shit
  m_operations.append(BINARISE);
  m_op_data.append(QVariant()); // no actual data.
  m_op_images.append(m_modified_image);
  m_modifiable_int = 200;

  // create a modifiable image.
  m_modifiable = new Mat(ImageConverter::imageToMat(m_modified_image));
  // convert to grayscale
  cvtColor(*m_modifiable, *m_modifiable, CV_BGR2GRAY);
  updateImage(ImageConverter::matToImage(*m_modifiable));
}

void OcrImage::acceptThreshold()
{
  m_op_data.insert(m_op_data.size() - 1, m_modifiable_int);
  delete m_modifiable;
  emit thresholdAccepted();
}

void OcrImage::applyThreshold()
{
  Mat modified;
  threshold(*m_modifiable, modified, m_modifiable_int, 200, THRESH_BINARY);
  updateImage(ImageConverter::matToImage(modified));
}

void OcrImage::cancelThreshold()
{
  // remove the last operation
  m_operations.takeLast();
  m_op_data.takeLast();
  m_modified_image = m_op_images.takeLast();
  // update to the pre-biniarise image
  updateImage(m_modified_image);
  delete m_modifiable;
}

void OcrImage::invert()
{
  // save undo shit
  m_operations.append(INVERT);
  m_op_data.append(QVariant()); // no actual data.
  m_op_images.append(m_modified_image);

  // create a modifiable image.
  Mat modified = ImageConverter::imageToMat(m_modified_image);
  bitwise_not(modified, modified);
  updateImage(ImageConverter::matToImage(modified));
}

void OcrImage::setThreshold(int thresh_value)
{
  // modify modifiable image to new threshold
  m_modifiable_int = thresh_value;
}

void OcrImage::denoise()
{
  // save undo shit
  m_operations.append(BINARISE);
  m_op_data.append(QVariant()); // no actual data.
  m_op_images.append(m_modified_image);
  m_modifiable_int = 10; // a reasonable value

  // create a modifiable image.
  m_modifiable = new Mat(ImageConverter::imageToMat(m_modified_image));

  if (m_modifiable->type()) {

  }
}



