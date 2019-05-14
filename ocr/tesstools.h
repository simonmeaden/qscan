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
#ifndef TESSTOOLS_H
#define TESSTOOLS_H

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QImage>
#include <QMessageBox>
#include <QObject>
#include <QStringLiteral>

#include "logger.h"
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include "scanpage.h"

#include "ocr_global.h"

class Mat;

// void monitorProgress(ETEXT_DESC *monitor, int page);
void ocrProcess(tesseract::TessBaseAPI* api, ETEXT_DESC* monitor);

class OCRSHARED_EXPORT TessTools : public QObject
{
  Q_OBJECT
public:
  explicit TessTools(const QString& datapath, const QString& lang, QObject* parent = nullptr);
  TessTools(const TessTools& other);     // copy constructor
  TessTools(TessTools&& other) noexcept; // move constructor
  ~TessTools() override;                 // destructor

  TessTools& operator=(TessTools const& other);     // copy assignment
  TessTools& operator=(TessTools&& other) noexcept; // move assignment

  void getStringFromPage(Page page);
  QString getStringFromImage(const QImage& image);


signals:
  void log(LogLevel, const QString&);

protected:
  tesseract::TessBaseAPI* m_api{};
  const char* m_api_lang{};
  const char* m_datapath{};
  //  ETEXT_DESC *monitor;

  static const char* kTrainedDataSuffix;
  void init(const char* datapath, const char* lang, tesseract::TessBaseAPI* api);
};

#endif // TESSTOOLS_H
