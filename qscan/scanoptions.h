#ifndef SCANOPTIONS_H
#define SCANOPTIONS_H

#include <QMap>
#include <QObject>
#include <QRect>

#include <sane/sane.h>
#include <sane/saneopts.h>

class ScanDevice;
typedef QSharedPointer<ScanDevice> Device;

enum ScanMode
{
  Default, /*!< Default - normally colour */
  Color,   /*!< Colour scan */
  Gray,    /*!< Greyscale */
  Mono,    /*!< Monochromatic */
};

enum ScanType
{
  Single,    /*!< Single page */
  ADF_Front, /*!< Auto Document Feeder front page only */
  ADF_Back,  /*!< Auto Document Feeder rear page only */
  ADF_Both,  /*!< Auto Document Feeder both front and rear pages */
  Batch,     /*!< A batch scan */
};

class ScanOptions : public QObject
{
  Q_OBJECT
public:
  explicit ScanOptions(QObject* parent = nullptr);
  ~ScanOptions();

  int
  getScannerValue(Device device, int option_id);

  int
  dpi() const;
  void
  setDpi(int dpi);

  ScanMode
  scanMode() const;
  void
  setScanMode(const ScanMode& scanMode);

  int
  depth() const;
  void
  setDepth(int depth);

  ScanType
  type() const;
  void
  setType(const ScanType& type);

  int
  paperWidth() const;
  void
  setPaperWidth(int paperWidth);

  int
  paperHeight() const;
  void
  setPaperHeight(int paperHeight);

  int
  brightness() const;
  void
  setBrightness(int brightness);

  int
  contrast() const;
  void
  setContrast(int contrast);

  int
  pageDelay() const;
  void
  setPageDelay(int pageDelay);

  int
  topLeftX() const;
  void
  setTopLeftX(int topLeftX);

  int
  topLeftY() const;
  void
  setTopLeftY(int topLeftY);

  int
  bottomRightX() const;
  void
  setBottomRightX(int bottomRightX);

  int
  bottomRightY() const;
  void
  setBottomRightY(int bottomRightY);

  QRect
  geometry();
  void
  setGeometry(QRect geometry);

  int
  scanResolutionX() const;
  void
  setScanResolutionX(int scan_resolution_x);

  int
  scanResolutionY() const;
  void
  setScanResolutionY(int scan_resolution_y);

  int
  scanResolution() const;
  void
  setScanResolution(int scan_resolution);

  QString
  name() const;
  void
  setName(const QString& name);

  QString
  description() const;
  void
  setDescription(const QString& description);

  QString
  title() const;
  void
  setTitle(const QString& title);

protected:
  QString m_name, m_description, m_title;
  int m_dpi;
  ScanMode m_mode;
  int m_depth;
  ScanType m_type;
  int m_paper_width;
  int m_paper_height;
  int m_brightness = -1;
  int m_contrast = -1;
  int m_page_delay;
  QRect m_geometry;
  int m_scan_resolution = -1;
  int m_scan_resolution_x = -1;
  int m_scan_resolution_y = -1;
};
typedef QSharedPointer<ScanOptions> Options;
typedef QMap<QString, Options> OptionsMap;

#endif // SCANOPTIONS_H
