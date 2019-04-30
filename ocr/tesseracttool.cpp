#include "tesseracttool.h"

#include "log4qt/consoleappender.h"
#include "log4qt/log4qt.h"
#include "log4qt/logmanager.h"
#include "log4qt/ttcclayout.h"

const char* TesseractTool::kTrainedDataSuffix = "traineddata";

TesseractTool::TesseractTool(QObject* parent)
  : QObject(parent)
{
  m_logger = Log4Qt::Logger::logger(tr("Ocr"));
}

/*!
 * Create tesseract box data from QImage
 */
QString
TesseractTool::makeString(const QImage& qImage, const int page)
{
  PIX* pixs;
  char* outText;

  if ((pixs = qImage2PIX(qImage)) == nullptr) {
    m_logger->info("Unsupported image type");
    return "";
  }

  // http://code.google.com/p/tesseract-ocr/issues/detail?id=228
  setlocale(LC_NUMERIC, "C");
// QString to  const char *
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QByteArray byteArray = getLang().toAscii();
#else
  QByteArray byteArray = getLang().toLocal8Bit();
#endif
  const char* apiLang = byteArray.constData();

// workaroung if datapath/TESSDATA_PREFIX is set...
#ifdef _WIN32
  QString envQString = "TESSDATA_PREFIX=" + getDataPath();
  QByteArray byteArrayWin = envQString.toUtf8();
  const char* env = byteArrayWin.data();
  putenv(env);
#else
  QByteArray byteArray1 = getDataPath().toUtf8();
  const char* datapath = byteArray1.data();
  setenv("TESSDATA_PREFIX", datapath, 1);
#endif

  auto* api = new tesseract::TessBaseAPI();
  if (api->Init(nullptr, apiLang)) {
    m_logger->info(tr("Could not initialize tesseract."));
    return "";
  }
  // Initialize tesseract to use English (eng) and the LSTM OCR engine.
  api->Init(nullptr, "eng", tesseract::OEM_LSTM_ONLY);
  // Set Page segmentation mode to PSM_AUTO (3)
  api->SetPageSegMode(tesseract::PSM_AUTO);

  QApplication::setOverrideCursor(Qt::WaitCursor);

#ifdef TESSERACT_VERSION
  api->SetImage(pixs);
  outText = api->GetBoxText(page);
#else
  api->SetVariable("tessedit_create_boxfile", "1");
  int timeout_millisec = 0;
  const char* filename = NULL;
  const char* retry_config = NULL;
  STRING text_out;
  if (!api->ProcessPage(
        pixs, page, filename, retry_config, timeout_millisec, &text_out)) {
    msg("Error during processing.\n");
  }
  outText = text_out.string();
#endif // TESSERACT_VERSION
  QApplication::restoreOverrideCursor();

  pixDestroy(&pixs);
  api->End();
  delete api;
  return QString::fromUtf8(outText);
}

/*!
 * Convert QT QImage to PIX
 * input: QImage
 * result: PIX
 */
PIX*
TesseractTool::qImage2PIX(const QImage& qImage)
{
  PIX* pixs;

  QImage myImage = qImage.rgbSwapped();
  int width = myImage.width();
  int height = myImage.height();
  int depth = myImage.depth();
  int wpl = myImage.bytesPerLine() / 4;

  pixs = pixCreate(width, height, depth);
  pixSetWpl(pixs, wpl);
  pixSetColormap(pixs, nullptr);
  l_uint32* datas = pixs->data;

  for (int y = 0; y < height; y++) {
    l_uint32* lines = datas + y * wpl;
    QByteArray a(reinterpret_cast<const char*>(myImage.scanLine(y)),
                 myImage.bytesPerLine());
    for (int j = 0; j < a.size(); j++) {
      *((l_uint8*)lines + j) = a[j];
    }
  }

  const qreal toDPM = 1.0 / 0.0254;
  int resolutionX = int(myImage.dotsPerMeterX() / toDPM);
  int resolutionY = int(myImage.dotsPerMeterY() / toDPM);

  if (resolutionX < 300)
    resolutionX = 300;
  if (resolutionY < 300)
    resolutionY = 300;
  pixSetResolution(pixs, resolutionX, resolutionY);

  return pixEndianByteSwapNew(pixs);
}

/*!
 * Convert Leptonica PIX to QImage
 * input: PIX
 * result: QImage
 */
QImage
TesseractTool::PIX2qImage(PIX* pixImage)
{
  int width = pixGetWidth(pixImage);
  int height = pixGetHeight(pixImage);
  int depth = pixGetDepth(pixImage);
  int bytesPerLine = pixGetWpl(pixImage) * 4;
  l_uint32* datas = pixGetData(pixEndianByteSwapNew(pixImage));

  QImage::Format format;
  if (depth == 1)
    format = QImage::Format_Mono;
  else if (depth == 8)
    format = QImage::Format_Indexed8;
  else
    format = QImage::Format_RGB32;

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
    m_logger->info(tr("Invalid format!!!"));
    return none;
  }

  return result.rgbSwapped();
}

QImage
TesseractTool::GetThresholded(const QImage& qImage)
{
  // TODO(zdenop): Check this for memory leak
  PIX* pixs = qImage2PIX(qImage);

  // Set tessdata as Enviromental Variable to avoid problems
  QByteArray byteArray1 = getDataPath().toUtf8();
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
  QByteArray byteArray = getLang().toLocal8Bit();
#endif
  const char* apiLang = byteArray.constData();

  setlocale(LC_NUMERIC, "C");

  auto* api = new tesseract::TessBaseAPI();
  if (api->Init(nullptr, apiLang)) {
    m_logger->info(tr("Could not initialize tesseract.\n"));
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

QString
TesseractTool::getDataPath()
{
  QSettings settings(QSettings::IniFormat,
                     QSettings::UserScope,
                     SETTING_ORGANIZATION,
                     SETTING_APPLICATION);
  QString dataPath;
  if (settings.contains("Tesseract/DataPath")) {
    dataPath = settings.value("Tesseract/DataPath").toString();
  }
  return dataPath;
}

QString
TesseractTool::getLang()
{
  QSettings settings(QSettings::IniFormat,
                     QSettings::UserScope,
                     SETTING_ORGANIZATION,
                     SETTING_APPLICATION);
  QString lang;
  if (settings.contains("Tesseract/Lang")) {
    lang = settings.value("Tesseract/Lang").toString();
  }

  if (lang.isNull()) {
    m_logger->info(tr("You need to configure tesseract in Settings!"));
    return "";
  }
  return lang;
}

/*!
 * Get QList<QString> with list of available languages
 */
QList<QString>
TesseractTool::getLanguages(const QString& datapath)
{
  QList<QString> languages;
  QDir dir(datapath);

  if (!dir.exists()) {
    QMessageBox msgBox;
    msgBox.setText(
      QObject::tr("Cannot find the tessdata directory '%1'!\n").arg(datapath) +
      QObject::tr("Please check your configuration or tesseract instalation"));
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
