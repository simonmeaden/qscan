/*
    Copyright © Simon Meaden 2019.
    This file was developed as part of the QScan cpp library but could
    easily be used elsewhere.

    QScan is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QScan is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QScan.  If not, see <http://www.gnu.org/licenses/>.
    It is also available on request from Simon Meaden simonmeaden@sky.com.
*/
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
  ScanOptions(QObject* parent = nullptr);
  ScanOptions(const ScanOptions& other);
  ~ScanOptions();

  ScanOptions operator = (const ScanOptions rhs);

  int getIntScannerValue(const ScanDevice* device, const int option_id, const SANE_Option_Descriptor* opt);
  QStringList getStringScannerValue(const ScanDevice* device, const int option_id, const SANE_Option_Descriptor* opt);

  int dpi() const;
  void setDpi(int dpi);

  QStringList scanModes() const;
  void setScanModes(const QStringList& modes);

  QStringList sources() const;
  void setSources(const QStringList& sources);

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

  int topLeftX() const;
  void setTopLeftX(int topLeftX);

  int topLeftY() const;
  void setTopLeftY(int topLeftY);

  int bottomRightX() const;
  void setBottomRightX(int bottomRightX);

  int bottomRightY() const;
  void setBottomRightY(int bottomRightY);

  QRect geometry();
  void setGeometry(QRect geometry);

  int scanResolutionX() const;
  void setScanResolutionX(int scan_resolution_x);

  int scanResolutionY() const;
  void setScanResolutionY(int scan_resolution_y);

  int scanResolution() const;
  void setScanResolution(int scan_resolution);

  int optionId(QString name) const;
  void setOptionId(const QString& name, int option_id);

  void copyData(const ScanOptions& other);

protected:
  //  QString m_name, m_description, m_title;
  int m_dpi;
  QStringList m_modes;
  QStringList m_sources;
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
  QMap<QString, int> m_option_pairs;
};
//typedef QSharedPointer<ScanOptions> Options;
typedef QMap<QString, ScanOptions> OptionsMap;

Q_DECLARE_METATYPE(ScanOptions)

#endif // SCANOPTIONS_H
