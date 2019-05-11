#ifndef OPENCVIMAGE_H
#define OPENCVIMAGE_H

#include <QWidget>

#include <opencv2/opencv.hpp>

using namespace cv;

class OpencvImage
{
public:

  static Mat* toMat(const QImage& image);
  static QImage fromMat(Mat* mat);

  static QImage fromIplImage(const IplImage* iplImg);
  static IplImage* toIplImage(const QImage& image);

private:

  static QPixmap convert16uc1(const cv::Mat& source);
  static QPixmap convert8uc3(const cv::Mat& source);
  static QPixmap convert16uc3(const cv::Mat& source);
};

#endif // OPENCVIMAGE_H
