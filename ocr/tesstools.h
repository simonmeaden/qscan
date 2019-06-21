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
#include <QString>
#include <QtDebug>

#include "logger.h"

#include "documentdata.h"
#include "imageconverter.h"
#include "ocr_global.h"

// void monitorProgress(ETEXT_DESC *monitor, int page);
//void ocrProcess(tesseract::TessBaseAPI* api, ETEXT_DESC* monitor);

namespace TessTools {

void getStringFromPage(const QString &datapath, const QString &lang, const OcrData &page);
QString getStringFromImage(const QString &datapath,
                           const QString &lang,
                           const QImage &image,
                           const QRect &rect);

} // namespace TessTools

#endif // TESSTOOLS_H
