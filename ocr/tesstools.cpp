
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

#include <opencv2/opencv.hpp>

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

const char* TessTools::kTrainedDataSuffix = "traineddata";

/*!
   Create tesseract box data from QImage
*/
TessTools::TessTools(QString datapath, QString lang, QObject* parent)
  : QObject(parent)
  , m_datapath(std::move(datapath))
  , m_lang(std::move(lang))
{
  m_datapath += QStringLiteral("/tesseract/tessdata");
}

QString TessTools::getStringFromImage(const QString& image_path)
{
  cv::Mat im = cv::imread(image_path.toStdString());
  char* outText;

  setlocale(LC_ALL, "C");
  // QString to  const char *
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QByteArray byteArray = getLang().toAscii();
#else
  QByteArray byteArray = m_lang.toLocal8Bit();
#endif
  const char* apiLang = byteArray.constData();

  QByteArray byteArray1 = m_datapath.toUtf8();
  const char* datapath = byteArray1.data();
  //  setenv("TESSDATA_PREFIX", datapath, 1);

  auto* api = new tesseract::TessBaseAPI();
  //  monitor = new ETEXT_DESC();

  if (!api->Init(datapath, apiLang, tesseract::OEM_LSTM_ONLY)) {
    emit log(LogLevel::INFO, tr("Could not initialize tesseract."));
    return "";
  }

  // Set Page segmentation mode to PSM_AUTO (3)
  api->SetPageSegMode(tesseract::PSM_AUTO);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  api->SetImage(im.data, im.cols, im.rows, 3, im.step);
  outText = api->GetUTF8Text();

  QApplication::restoreOverrideCursor();

  api->End();
  delete api;
  return QString::fromUtf8(outText);
}
