
/**********************************************************************
   File:        TessTools.cpp
   Description: Tesseract tools
   Author:      Zdenko Podobny
   Created:     2012-03-27

   (C) Copyright 2012-2013, Zdenko Podobny
 **
 ** Licensed under the Apache License, Version 2.0 (the "License");
 ** you may not use this file except in compliance with the License.
 ** You may obtain a copy of the License at
 **
 **    http://www.apache.org/licenses/LICENSE-2.0
 **
 ** Unless required by applicable law or agreed to in writing, software
 ** distributed under the License is distributed on an "AS IS" BASIS,
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ** See the License for the specific language governing permissions and
 ** limitations under the License.

 **********************************************************************/
#include <utility>

#include "tesstools.h"

#include <tesseract/baseapi.h>
#include <tesseract/genericvector.h>
#include <tesseract/renderer.h>

#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>

#include "imageconverter.h"
#include "documentdata.h"

//using namespace cv;

//void monitorProgress(ETEXT_DESC *monitor, int page) {
//  while (1) {
//    printf( "\r%3d%%", monitor[page].progress);
//    fflush (stdout);
//    if (monitor[page].progress==100)
//      break;
//  }
//}

//void ocrProcess(tesseract::TessBaseAPI *api, ETEXT_DESC *monitor) {
//  api->Recognize(monitor);
//}

namespace TessTools {

using namespace QScanner;

void getStringFromPage(const QString &datapath, const QString &lang, const DocumentData &doc_data)
{
  setlocale(LC_ALL, "C");
  auto *api = new tesseract::TessBaseAPI();

  if (!api->Init(datapath.toStdString().c_str(),
                 lang.toStdString().c_str(),
                 tesseract::OEM_LSTM_ONLY)) {
    return;
  }

  cv::Mat mat_image = cv::imread(doc_data->filename().toStdString(), cv::IMREAD_COLOR);
  QString out_text;
  //  monitor = new ETEXT_DESC();

  api->SetPageSegMode(tesseract::PSM_AUTO);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  api->SetImage(mat_image.data, mat_image.cols, mat_image.rows, 3, mat_image.step);
  // for some reason internal to tesseract it sometimes crashes with
  // a Segmentation Fault without this.
  api->ClearAdaptiveClassifier();
  out_text = QString::fromUtf8(api->GetUTF8Text());
  doc_data->appendText(out_text);

  QApplication::restoreOverrideCursor();

  api->End();
}

QString getStringFromImage(const QString &datapath,
                           const QString &lang,
                           const QImage &image,
                           const QRect &rect)
{
  setlocale(LC_ALL, "C");
  auto *api = new tesseract::TessBaseAPI();

  int result = api->Init(datapath.toStdString().c_str(),
                         lang.toStdString().c_str(),
                         tesseract::OEM_LSTM_ONLY);

  if (result == -1) {
    return QString();
  }

  // pixToImage() and pixRead() of original file are equal according
  // to pixEqual().
  PIX *pix = ImageConverter::imageToPix(image);
  api->SetPageSegMode(tesseract::PSM_AUTO);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  api->SetImage(pix);

  if (!rect.isNull()) {
    api->SetRectangle(rect.left(), rect.top(), rect.width(), rect.height());
  }

  // for some reason internal to tesseract it sometimes crashes with
  // a Segmentation Fault without this.
  api->ClearAdaptiveClassifier();

  QString out_text = QString::fromUtf8(api->GetUTF8Text());

  QApplication::restoreOverrideCursor();

  api->End();

  return out_text;
}

} // namespace TessTools
