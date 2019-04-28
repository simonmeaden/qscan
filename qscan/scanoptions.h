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
using Device = QSharedPointer<ScanDevice>;

enum ScanUnits
{
  DPI,
  MM,
};

class ScanOptions : public QObject
{
  Q_OBJECT
public:
  explicit ScanOptions(QObject* parent = nullptr);
  ScanOptions(const ScanOptions& other);
  ~ScanOptions() override;

  ScanOptions& operator=(const ScanOptions& rhs);

  //  int getIntScannerValue(const ScanDevice* device,
  //                         const int option_id,
  //                         const SANE_Option_Descriptor* opt);
  //  QStringList getStringScannerValue(const ScanDevice* device,
  //                                    const int option_id,
  //                                    const SANE_Option_Descriptor* opt);

  QStringList modes() const;
  void setModes(const QStringList& modes);

  QStringList sources() const;
  void setSources(const QStringList& sources);

  int depth() const;
  void setDepth(int depth);

  //  ScanType type() const;
  //  void setType(const ScanType& type);

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

  int resolutionX() const;
  void setResolutionX(int scan_resolution_x);

  int resolutionY() const;
  void setResolutionY(int scan_resolution_y);

  int resolution() const;
  void setResolution(int scan_resolution);

  int minResolution() const;
  void setMinResolution(int minResolution);

  int maxResolution() const;
  void setMaxResolution(int maxResolution);

  int optionId(const QString& name) const;
  void setOptionId(const QString& name, int option_id);

  void copyData(const ScanOptions& other);

  QString mode() const;
  void setMode(const QString& mode);

  QString source() const;
  void setSource(const QString& source);

  ScanUnits units() const;
  void setUnits(const ScanUnits& units);

  QString currentMode() const;
  void setCurrentMode(const QString& currentMode);

  QString currentSource() const;
  void setCurrentSource(const QString& currentSource);

protected:
  QStringList m_modes;
  QString m_current_mode;
  QStringList m_sources;
  QString m_current_source;
  QString m_mode;
  QString m_source;
  int m_depth;
  //  ScanType m_type;
  ScanUnits m_units;
  int m_paper_width;
  int m_paper_height;
  int m_brightness = -1;
  int m_contrast = -1;
  int m_page_delay;
  QRect m_geometry;
  int m_resolution = -1;
  int m_min_resolution = 1;
  int m_max_resolution = 1;
  int m_resolution_x = -1;
  int m_resolution_y = -1;
  QMap<QString, int> m_option_pairs;
};
using OptionsMap = QMap<QString, ScanOptions>;

Q_DECLARE_METATYPE(ScanOptions)

#endif // SCANOPTIONS_H
