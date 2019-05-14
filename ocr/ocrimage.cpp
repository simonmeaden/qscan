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
  m_op_images.append(ImageConverter::imageToMat(m_modified_image));
  BaseScanImage::cropToSelection();
}

void OcrImage::binarise()
{
  // save undo shit
  m_operations.append(BINARISE);
  m_op_data.append(QVariant()); // no actual data.
  m_op_images.append(m_modified_image);
  m_modifiable_int = 255;

  // create a modifiable image.
  m_modifiable = new Mat(ImageConverter::imageToMat(m_modified_image));
  // convert to grayscale
  cvtColor(*m_modifiable, *m_modifiable, CV_BGR2GRAY);
  updateImage(ImageConverter::matToImage(*m_modifiable));
}

void OcrImage::acceptThreshold()
{
  m_op_data.insert(m_op_data.size() - 1, m_modifiable_int);
}

void OcrImage::cancelThreshold()
{
  // remove the last operation
  m_operations.takeLast();
  m_op_data.takeLast();
  m_modified_image = m_op_images.takeLast();
  // update to the pre-biniarise image
  updateImage(m_modified_image);
}

void OcrImage::setThreshold(int thresh_value)
{
  // modify modifiable image to new threshold
  m_modifiable_int = thresh_value;
  threshold(*m_modifiable, *m_modifiable, thresh_value, 255, THRESH_BINARY);
  updateImage(ImageConverter::matToImage(*m_modifiable));
}


