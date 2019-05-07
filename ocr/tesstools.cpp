#include <utility>

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
#include "tesstools.h"



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

QString TessTools::makeBoxes(const QImage& qImage, const int page)
{
  PIX* pixs;
  char* outText;

  if ((pixs = qImage2PIX(qImage)) == nullptr) {
    emit log(LogLevel::INFO, QString("Unsupported image type"));
    return "";
  }

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

  if (api->Init(datapath, apiLang, tesseract::OEM_LSTM_ONLY)) {
    emit log(LogLevel::INFO, tr("Could not initialize tesseract."));
    return "";
  }

  // Set Page segmentation mode to PSM_AUTO (3)
  api->SetPageSegMode(tesseract::PSM_AUTO);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  api->SetImage(pixs);
  outText = api->GetUTF8Text();

  QApplication::restoreOverrideCursor();

  pixDestroy(&pixs);
  api->End();
  delete api;
  return QString::fromUtf8(outText);
}

/*!
   Convert QT QImage to Leptonica PIX
   input: QImage
   result: PIX
*/
PIX* TessTools::qImage2PIX(const QImage& qimage)
{
  PIX* pixs;

  QImage image = qimage.rgbSwapped();
  int width = image.width();
  int height = image.height();
  int depth = image.depth();
  int wpl = image.bytesPerLine() / 4;

  pixs = pixCreate(width, height, depth);
  pixSetWpl(pixs, wpl);
  pixSetColormap(pixs, nullptr);
  l_uint32* datas = pixs->data;

  for (int y = 0; y < height; y++) {
    l_uint32* lines = datas + y * wpl;
    QByteArray a(reinterpret_cast<const char*>(image.scanLine(y)),
                 image.bytesPerLine());

    for (int j = 0; j < a.size(); j++) {
      *((l_uint8*)lines + j) = a[j];
    }
  }

  const qreal toDPM = 1.0 / 0.0254;
  int resolutionX = int(image.dotsPerMeterX() / toDPM);
  int resolutionY = int(image.dotsPerMeterY() / toDPM);

  if (resolutionX < 300) {
    resolutionX = 300;
  }

  if (resolutionY < 300) {
    resolutionY = 300;
  }

  pixSetResolution(pixs, resolutionX, resolutionY);

  return pixEndianByteSwapNew(pixs);
}

/*!
   Convert Leptonica PIX to QImage
   input: PIX
   result: QImage
*/
QImage TessTools::PIX2qImage(PIX* pixImage)
{
  int width = pixGetWidth(pixImage);
  int height = pixGetHeight(pixImage);
  int depth = pixGetDepth(pixImage);
  int bytesPerLine = pixGetWpl(pixImage) * 4;
  l_uint32* datas = pixGetData(pixEndianByteSwapNew(pixImage));

  QImage::Format format;

  if (depth == 1) {
    format = QImage::Format_Mono;

  } else if (depth == 8) {
    format = QImage::Format_Indexed8;

  } else {
    format = QImage::Format_RGB32;
  }

  QImage result(
    reinterpret_cast<uchar*>(datas), width, height, bytesPerLine, format);

  // Set resolution
  l_int32 xres, yres;
  pixGetResolution(pixImage, &xres, &yres);
  const qreal toDPM = 1.0 / 0.0254;
  result.setDotsPerMeterX(int(xres * toDPM));
  result.setDotsPerMeterY(int(yres * toDPM));

  // Handle palette
  QVector<QRgb> _bwCT;
  _bwCT.append(qRgb(255, 255, 255));
  _bwCT.append(qRgb(0, 0, 0));

  QVector<QRgb> _grayscaleCT(256);

  for (int i = 0; i < 256; i++) {
    _grayscaleCT[i] = qRgb(i, i, i);
  }

  switch (depth) {
  case 1:
    result.setColorTable(_bwCT);
    break;

  case 8:
    result.setColorTable(_grayscaleCT);
    break;

  default:
    result.setColorTable(_grayscaleCT);
  }

  if (result.isNull()) {
    static QImage none(0, 0, QImage::Format_Invalid);
    emit log(LogLevel::INFO, tr("Invalid format!!!"));
    return none;
  }

  return result.rgbSwapped();
}

QImage TessTools::GetThresholded(const QImage& qImage)
{
  // TODO(zdenop): Check this for memory leak
  PIX* pixs = qImage2PIX(qImage);

  // Set tessdata as Enviromental Variable to avoid problems
  QByteArray byteArray1 = m_datapath.toUtf8();
#ifdef _WIN32
  QString envQString = "TESSDATA_PREFIX=" + byteArray1;
  const char* env = qString2Char(envQString);
  putenv(env);
#else
  const char* datapath = byteArray1.data();
  setenv("TESSDATA_PREFIX", datapath, 1);
#endif

  // TODO(zdenop): Why apiLang = qString2Char(getLang()) do not work???
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QByteArray byteArray = getLang().toAscii();
#else
  QByteArray byteArray = m_lang.toLocal8Bit();
#endif
  const char* apiLang = byteArray.constData();

  setlocale(LC_NUMERIC, "C");

  auto* api = new tesseract::TessBaseAPI();

  if (api->Init(nullptr, apiLang)) {
    emit log(LogLevel::INFO, tr("Could not initialize tesseract.\n"));
    return QImage();
  }

  api->SetImage(pixs);
  PIX* pixq = api->GetThresholdedImage();
  QImage tresholdedImage = PIX2qImage(pixq);
  api->End();
  delete api;
  pixDestroy(&pixs);
  pixDestroy(&pixq);

  return tresholdedImage;
}

/*!
   Get QList<QString> with list of available languages
*/
QList<QString> TessTools::getLanguages(const QString& datapath)
{
  QList<QString> languages;
  QDir dir(datapath);

  if (!dir.exists()) {
    QMessageBox msgBox;
    msgBox.setText(
      tr("Cannot find the tessdata directory '%1'!\n").arg(datapath) +
      tr("Please check your configuration or tesseract instalation"));
    msgBox.exec();
    return languages;
  }

  QString filter = "*.";
  filter += kTrainedDataSuffix;
  QStringList filters;
  filters << filter.trimmed();
  dir.setNameFilters(filters);

  QFileInfoList list = dir.entryInfoList();

  for (const auto& fileInfo : list) {
    languages.append(QString("%1").arg(fileInfo.baseName()));
  }

  qSort(languages);

  return languages;
}
