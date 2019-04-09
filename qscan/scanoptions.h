#ifndef SCANOPTIONS_H
#define SCANOPTIONS_H

#include <QMap>
#include <QObject>

#include <sane/sane.h>
#include <sane/saneopts.h>

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

  int dpi() const;
  void setDpi(int dpi);

  ScanMode mode() const;
  void setMode(const ScanMode& mode);

  int depth() const;
  void setDepth(int depth);

  ScanType type() const;
  void setType(const ScanType& type);

  int paperWidth() const;
  void setPaperWidth(int paperWidth);

  int paperHeight() const;
  void setPaperHeight(int paperHeight);

  int brightness() const;
  void setBrightness(int brightness);

  int contrast() const;
  void setContrast(int contrast);

  int pageDelay() const;
  void setPageDelay(int pageDelay);

protected:
  int m_dpi;
  ScanMode m_mode;
  int m_depth;
  ScanType m_type;
  int m_paper_width;
  int m_paper_height;
  int m_brightness;
  int m_contrast;
  int m_page_delay;
};
typedef QSharedPointer<ScanOptions> Options;

#endif // SCANOPTIONS_H
