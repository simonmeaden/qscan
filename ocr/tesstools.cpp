
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

const char* TessTools::kTrainedDataSuffix = "traineddata";

/*!
   Create tesseract box data from QImage
*/
TessTools::TessTools(const QString& datapath, const QString& lang, QObject* parent)
  : QObject(parent)
{

  auto* datapath_ = datapath.toUtf8().constData();

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  auto* api_lang_ = lang.toAscii().constData();
#else
  auto* api_lang_ = lang.toLocal8Bit().constData();
#endif

  setlocale(LC_ALL, "C");
  auto* api = new tesseract::TessBaseAPI();

  init(datapath_, api_lang_, api);
}

TessTools::TessTools(const TessTools& other)
{
  setlocale(LC_ALL, "C");
  init(other.m_datapath, other.m_api_lang, other.m_api);
}

TessTools::~TessTools()
{
  m_api->End();
  delete m_api;
}

TessTools::TessTools(TessTools&& other) noexcept
  : m_datapath(std::exchange(other.m_datapath, nullptr))
  , m_api_lang(std::exchange(other.m_api_lang, nullptr))
{
  setlocale(LC_ALL, "C");
  init(m_datapath, m_api_lang, other.m_api);
}

TessTools& TessTools::operator=(TessTools&& other) noexcept
{
  std::swap(m_datapath, other.m_datapath);
  std::swap(m_api_lang, other.m_api_lang);
  m_api = other.m_api;
  return *this;
}

TessTools& TessTools::operator =(const TessTools& other)
{
  if (this != &other) {
    // deallocate existing m_api.
    if (m_api) {
      m_api->End();
    }

    init(other.m_datapath, other.m_api_lang, other.m_api);
  }

  return *this;
}

void TessTools::init(const char* datapath, const char* lang, tesseract::TessBaseAPI* api)
{
  m_datapath = datapath;
  m_api_lang = lang;
  m_api = api;

  if (!api->Init(m_datapath, m_api_lang, tesseract::OEM_LSTM_ONLY)) {
    emit log(LogLevel::INFO, tr("Could not initialize tesseract."));
  }
}


void TessTools::getStringFromImage(Page page)
{
  cv::Mat mat_image = cv::imread(page->imagePath().toStdString(), cv::IMREAD_COLOR);
  QString out_text;
  //  monitor = new ETEXT_DESC();

  m_api->SetPageSegMode(tesseract::PSM_AUTO);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  m_api->SetImage(mat_image.data, mat_image.cols, mat_image.rows, 3, mat_image.step);
  // for some reason internal to tesseract it sometimes crashes with
  // a Segmentation Fault without this.
  m_api->ClearAdaptiveClassifier();
  out_text = QString::fromUtf8(m_api->GetUTF8Text());
  page->setText(out_text);

  QApplication::restoreOverrideCursor();
}
