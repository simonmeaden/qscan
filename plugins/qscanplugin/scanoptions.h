/*
  Copyright © Simon Meaden 2019.
  This file was developed as part of the Biblios application but could
  easily be used elsewhere.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#ifndef SCANOPTIONS_H
#define SCANOPTIONS_H

#include <QMap>
#include <QObject>
#include <QRect>
#include <QVariant>

#include <sane/sane.h>
#include <sane/saneopts.h>

namespace QScanner {

class ScanDevice;
using Device = QSharedPointer<ScanDevice>;

struct RangeData
{
  int min;
  int max;
};

struct ScanRange
{
  QVariant range_data;
  int value;

};

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

  //  int minResolution() const;
  //  void setMinResolution(int minResolution);

  //  int maxResolution() const;
  //  void setMaxResolution(int maxResolution);
  bool isResolutionRange();
  void setResulutionRange(ScanRange scan_range);
  ScanRange resolutionRange();

  int optionId(const QString& name) const;
  void setOptionId(const QString& name, int option_id);
  int optionSize(const QString& name) const;
  void setOptionSize(const QString& name, int size);
  int optionType(const QString& name) const;
  void setOptionType(const QString& name, int type);

  void copyData(const ScanOptions& other);

  QString mode() const;
  void setMode(const QString& mode);

  QString source() const;
  void setSource(const QString& source);

  ScanUnits units() const;
  void setUnits(const ScanUnits& units);

protected:
  QStringList m_modes;
  QStringList m_sources;
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
  ScanRange m_resolution_range;
  //  int m_min_resolution = 1;
  //  int m_max_resolution = 1;
  int m_resolution_x = -1;
  int m_resolution_y = -1;
  QMap<QString, int> m_option_id_map;
  QMap<QString, int> m_option_size_map;
  QMap<QString, int> m_option_type_map;
};
using OptionsMap = QMap<QString, ScanOptions>;

} // end of namespace QScanner

Q_DECLARE_METATYPE(QScanner::RangeData);
Q_DECLARE_METATYPE(QScanner::ScanRange);
Q_DECLARE_METATYPE(QScanner::ScanOptions)


#endif // SCANOPTIONS_H
