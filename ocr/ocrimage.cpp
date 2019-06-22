#include "ocrimage.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "imageconverter.h"

using namespace cv;

const qreal OcrImage::BASE_RESCALE = 1.0;

OcrImage::OcrImage(QWidget *parent)
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

void OcrImage::undoLastChange()
{
  if (m_changes > 0) {
    QImage image = m_op_images.takeLast();
    m_modified_image = image;
    scaleModifiedImage();
    m_changes--;
  }
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
  if (!(m_modified_image.format() == QImage::Format_Mono
        || m_modified_image.format() == QImage::Format_Grayscale8)) {
    m_image_base = m_modified_image;
    m_op_images.append(m_modified_image);

    // create a modifiable image.
    Mat mat = ImageConverter::imageToMat(m_modified_image);
    m_changes = 0;
    // convert to grayscale
    cvtColor(mat, mat, CV_BGR2GRAY);
    QImage image = ImageConverter::matToImage(mat);
    m_modified_image = image;

    scaleModifiedImage();
  }

}

void OcrImage::dumpImageChanges()
{
  while (m_changes > 0) {
    m_op_images.takeLast();
    m_changes--; // dump unneeded changes
  }

  m_image_base = QImage();
}

void OcrImage::acceptChanges()
{
  dumpImageChanges();
  scaleModifiedImage();
}

void OcrImage::applyThreshold(int value)
{
  m_op_images.append(m_modified_image);
  m_changes++;
  Mat mat = ImageConverter::imageToMat(m_modified_image);
  threshold(mat, mat, value, 255, THRESH_BINARY);
  QImage image = ImageConverter::matToImage(mat);
  image = m_modified_image.convertToFormat(QImage::Format::Format_Mono);
  m_modified_image = image;

  scaleModifiedImage();
}

void OcrImage::rescale()
{
  m_image_base = m_modified_image;
}

void OcrImage::revertRescale()
{
  m_modified_image = m_image_base;
  dumpImageChanges();
  scaleModifiedImage();
}

void OcrImage::applyRescale(double value)
{
  m_op_images.append(m_modified_image);
  m_changes++;
  Mat mat = ImageConverter::imageToMat(m_modified_image);
  cv::resize(mat, mat, cv::Size(), value, value, CV_INTER_CUBIC);
  QImage image = ImageConverter::matToImage(mat);

  int dpm = image.dotsPerMeterX();
  //  image.setDotsPerMeterX(int(dpm * value));
  dpm = image.dotsPerMeterY();
  //  image.setDotsPerMeterY(int(dpm * value));

  m_modified_image = image;
  scaleModifiedImage();
  emit imageSizeChanged(m_modified_image.width(),
                        m_modified_image.height(),
                        Util::dpmToDpi(m_modified_image.dotsPerMeterX()),
                        Util::dpmToDpi(m_modified_image.dotsPerMeterY()));
}

void OcrImage::cancelCurrentChanges()
{
  // update to the pre-modified image
  m_modified_image = m_image_base;
  dumpImageChanges();
  scaleModifiedImage();
}

void OcrImage::denoise()
{
  m_op_images.append(m_modified_image);

  // TODO
}

void OcrImage::deskew()
{
  m_op_images.append(m_modified_image);
  rotateByEdge();
  /*
    This doesn't work well at the moment. Just use manual system.
    // create a modifiable image.
    Mat mat = ImageConverter::imageToMat(m_modified_image);

    std::vector<Vec4i> lines;
    HoughLinesP(mat, lines, 1, CV_PI / 180, 100, width() / 2.0f, 20);

    qreal angle = 0.;
    unsigned nb_lines = lines.size();

    for (unsigned i = 0; i < nb_lines; ++i) {
    QLineF line(QPointF((double) lines[i][0], (double) lines[i][1]),
                QPointF((double) lines[i][2], (double) lines[i][3]));
    m_lines.append(line);
    angle += atan2((double) lines[i][3] - lines[i][1], (double) lines[i][2] - lines[i][0]);
    }

    m_angle = qRadiansToDegrees(angle / nb_lines); // mean angle, in radians.

    rotateBy(-m_angle);
  */
}

bool OcrImage::isInverted() const
{
  return m_inverted;
}

void OcrImage::setInverted(bool inverted)
{
  m_inverted = inverted;
}

void OcrImage::paintEvent(QPaintEvent *event)
{
  BaseScanImage::paintEvent(event);

  QPainter painter(this);
  QPen pen(QColor("red"), 2);
  painter.setPen(pen);

  for (QLineF line : m_lines) {
    painter.drawLine(line);
  }
}

// double OcrImage::houghTransform(Mat &im) {
//  double skew;
//  double max_r = sqrt(pow(.5 * im.cols, 2) + pow(.5 * im.rows, 2));
//  int angleBins = 180;
//  Mat acc = Mat::zeros(Size(2 * max_r, angleBins), CV_32SC1);
//  int cenx = im.cols / 2;
//  int ceny = im.rows / 2;

//  for (int x = 1; x < im.cols - 1; x++) {
//    for (int y = 1; y < im.rows - 1; y++) {
//      if (im.at<uchar>(y, x) == 255) {
//        for (int t = 0; t < angleBins; t++) {
//          double r = (x - cenx) * cos((double)t / angleBins * CV_PI) +
//              (y - ceny) * sin((double)t / angleBins * CV_PI);
//          r += max_r;
//          acc.at<int>(t, int(r))++;
//        }
//      }
//    }
//  }

//  Mat thresh;
//  normalize(acc, acc, 255, 0, NORM_MINMAX);
//  convertScaleAbs(acc, acc);
//  /*debug
//      Mat cmap;
//      applyColorMap(acc,cmap,COLORMAP_JET);
//      imshow("cmap",cmap);
//      imshow("acc",acc);*/

//  Point maxLoc;
//  minMaxLoc(acc, nullptr, nullptr, nullptr, &maxLoc);
//  double theta = (double)maxLoc.y / angleBins * CV_PI;
//  double rho = maxLoc.x - max_r;

//  if (abs(sin(theta)) < 0.000001) { // check vertical
//    // when vertical, line equation becomes
//    // x = rho
//    double m = -cos(theta) / sin(theta);
//    Point2d p1 = Point2d(rho + im.cols / 2, 0);
//    Point2d p2 = Point2d(rho + im.cols / 2, im.rows);
//    //      line(orig, p1, p2, Scalar(0, 0, 255), 1);
//    skew = 90;
//    qWarning() << "skew angle "
//               << " 90";

//  } else {
//    // convert normal form back to slope intercept form
//    // y = mx + b
//    double m = -cos(theta) / sin(theta);
//    double b = rho / sin(theta) + im.rows / 2. - m * im.cols / 2.;
//    Point2d p1 = Point2d(0, b);
//    Point2d p2 = Point2d(im.cols, im.cols * m + b);
//    //    line(orig, p1, p2, Scalar(0, 0, 255), 1);
//    double skewangle;
//    skewangle = p1.x - p2.x > 0
//        ? (atan2(p1.y - p2.y, p1.x - p2.x) * 180. / CV_PI)
//        : (atan2(p2.y - p1.y, p2.x - p1.x) * 180. / CV_PI);
//    skew = skewangle;
//    qWarning() << "skew angle " << skewangle;
//  }

//  //    imshow("orig", orig);
//  return skew;
//}
