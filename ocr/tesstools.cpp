
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

void getStringFromPage(const QString &datapath, const QString &lang, const DocumentData &page)
{
  setlocale(LC_ALL, "C");
  auto *api = new tesseract::TessBaseAPI();

  if (!api->Init(datapath.toStdString().c_str(),
                 lang.toStdString().c_str(),
                 tesseract::OEM_LSTM_ONLY)) {
    return;
  }

  cv::Mat mat_image = cv::imread(page->filename().toStdString(), cv::IMREAD_COLOR);
  QString out_text;
  //  monitor = new ETEXT_DESC();

  api->SetPageSegMode(tesseract::PSM_AUTO);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  api->SetImage(mat_image.data, mat_image.cols, mat_image.rows, 3, mat_image.step);
  // for some reason internal to tesseract it sometimes crashes with
  // a Segmentation Fault without this.
  api->ClearAdaptiveClassifier();
  out_text = QString::fromUtf8(api->GetUTF8Text());
  page->setText(out_text);

  QApplication::restoreOverrideCursor();

  api->End();
}

static void PreloadRenderers(tesseract::TessBaseAPI *api,
                             tesseract::PointerVector<tesseract::TessResultRenderer> *renderers,
                             tesseract::PageSegMode pagesegmode,
                             const char *outputbase)
{
  if (pagesegmode == tesseract::PSM_OSD_ONLY) {
#ifndef DISABLED_LEGACY_ENGINE
    renderers->push_back(new tesseract::TessOsdRenderer(outputbase));
#endif // ndef DISABLED_LEGACY_ENGINE

  } else {
    bool error = false;
    bool b;
    api->GetBoolVariable("tessedit_create_hocr", &b);

    if (b) {
      bool font_info;
      api->GetBoolVariable("hocr_font_info", &font_info);
      auto *renderer = new tesseract::TessHOcrRenderer(outputbase, font_info);

      if (renderer->happy()) {
        renderers->push_back(renderer);

      } else {
        delete renderer;
        qWarning() << QString("Error, could not create hOCR output file: %1").arg(errno);
        error = true;
      }
    }

    api->GetBoolVariable("tessedit_create_alto", &b);

    if (b) {
      tesseract::TessAltoRenderer *renderer = new tesseract::TessAltoRenderer(outputbase);

      if (renderer->happy()) {
        renderers->push_back(renderer);

      } else {
        delete renderer;
        qWarning() << QString("Error, could not create ALTO output file:  %1").arg(errno);
        error = true;
      }
    }

    api->GetBoolVariable("tessedit_create_tsv", &b);

    if (b) {
      bool font_info;
      api->GetBoolVariable("hocr_font_info", &font_info);
      tesseract::TessTsvRenderer *renderer = new tesseract::TessTsvRenderer(outputbase, font_info);

      if (renderer->happy()) {
        renderers->push_back(renderer);

      } else {
        delete renderer;
        qWarning() << QString("Error, could not create TSV output file:  %1").arg(errno);
        error = true;
      }
    }

    api->GetBoolVariable("tessedit_create_pdf", &b);

    if (b) {
#ifdef WIN32

      if (_setmode(_fileno(stdout), _O_BINARY) == -1) {
        qWarning() << QString("ERROR: cin to binary:  %1").arg(errno);
      }

#endif // WIN32
      bool textonly;
      api->GetBoolVariable("textonly_pdf", &textonly);
      tesseract::TessPDFRenderer *renderer = new tesseract::TessPDFRenderer(outputbase,
                                                                            api->GetDatapath(),
                                                                            textonly);

      if (renderer->happy()) {
        renderers->push_back(renderer);

      } else {
        delete renderer;
        qWarning() << QString("Error, could not create PDF output file:  % 1").arg(errno);
        error = true;
      }
    }

    api->GetBoolVariable("tessedit_write_unlv", &b);

    if (b) {
      api->SetVariable("unlv_tilde_crunching", "true");
      tesseract::TessUnlvRenderer *renderer = new tesseract::TessUnlvRenderer(outputbase);

      if (renderer->happy()) {
        renderers->push_back(renderer);

      } else {
        delete renderer;
        qWarning() << QString("Error, could not create UNLV output file:  %1").arg(errno);
        error = true;
      }
    }

    api->GetBoolVariable("tessedit_create_boxfile", &b);

    if (b) {
      tesseract::TessBoxTextRenderer *renderer = new tesseract::TessBoxTextRenderer(outputbase);

      if (renderer->happy()) {
        renderers->push_back(renderer);

      } else {
        delete renderer;
        qWarning() << QString("Error, could not create BOX output file:  %1").arg(errno);
        error = true;
      }
    }

    api->GetBoolVariable("tessedit_create_txt", &b);

    if (b || (!error && renderers->empty())) {
      tesseract::TessTextRenderer *renderer = new tesseract::TessTextRenderer(outputbase);

      if (renderer->happy()) {
        renderers->push_back(renderer);

      } else {
        delete renderer;
        qWarning() << QString("Error, could not create TXT output file:  %1").arg(errno);
      }
    }
  }

  if (!renderers->empty()) {
    // Since the PointerVector auto-deletes, null-out the renderers that are
    // added to the root, and leave the root in the vector.
    for (int r = 1; r < renderers->size(); ++r) {
      (*renderers)[0]->insert((*renderers)[r]);
      (*renderers)[r] = nullptr;
    }
  }
}

QString getStringFromImage(const QString &datapath, const QString &lang, const QImage &image)
{
  setlocale(LC_ALL, "C");
  auto *api = new tesseract::TessBaseAPI();

  if (!api->Init(datapath.toStdString().c_str(),
                 lang.toStdString().c_str(),
                 tesseract::OEM_LSTM_ONLY)) {
    return QString();
  }

  // pixToImage() and pixRead() of original file are equal according
  // to pixEqual().
  PIX *pix = ImageConverter::imageToPix(image);
  //  Pix *pix2 = pixRead(
  //    "/home/simonmeaden/.local/share/Biblos/ocr/The Flight of the Horse/ocrimage2.png");

  // some online reading suggested that the alpha channel (if any) should be removed
  // but still no text.
  pix = pixRemoveAlpha(pix);

  //  monitor = new ETEXT_DESC();

  api->SetPageSegMode(tesseract::PSM_AUTO);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  api->SetImage(pix);
  // for some reason internal to tesseract it sometimes crashes with
  // a Segmentation Fault without this.
  api->ClearAdaptiveClassifier();

  char *out_char = api->GetUTF8Text();
  QString out_text = QString::fromUtf8(out_char);
  delete[] out_char;

  QApplication::restoreOverrideCursor();

  api->End();

  return out_text;
}

} // namespace TessTools
