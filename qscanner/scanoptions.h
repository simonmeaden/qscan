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

/*!
   \brief The RangeData struct stores resolution range data.

   This is in the form of maximum and minimum integer values.
*/
class RangeData
{
public:
  RangeData()
    : min(-1)
    , max(-1) {

  }
  RangeData(const RangeData& other)
    : min(other.min)
    , max(other.max) {
  }
  ~RangeData() {}

  int min; //! minimum value in range.
  int max; //! maximum value in range.
};

class ScanOptions : public QObject
{
  Q_OBJECT
public:
  /*!
     \brief The units that the scanner data is defined in.
  */
  enum ScanUnits
  {
    NONE, //! No units.
    PIXEL, //! Number of pixels.
    BIT, //! Number of bits.
    DPI, //! indicates dots per inch.
    MM, //! indicates millimetres.
    PERCENT, //! Percentage.
    MICROSECOND, //! Time in micro-seconds.
  };
  enum AvailableOptions
  {
    QSCAN_OPTION_UNAVAILABLE,
    //    QSCAN_STANDARD,
    //    QSCAN_GEOMETRY,
    //    QSCAN_ENHANCEMENT,
    //    QSCAN_ADVANCED,
    //    QSCAN_SENSORS,
    //    QSCAN_PREVIEW,
    //    QSCAN_GRAY_PREVIEW,
    BIT_DEPTH, //! Bit depth of scanner in current mode.
    MODE, //! Current mode string.
    //    QSCAN_MODES, //! List of available mode strings.
    //    QSCAN_SPEED,
    SOURCE, //! Current source string.
    //    QSCAN_SOURCES, //! List of available source strings.
    //    QSCAN_BACKTRACK,
    LEFT, //! left edge of scanning area.
    TOP, //! top edge point of scanning area.
    RIGHT, //! right edge point of scanning area.
    BOTTOM, //! bottom edge point of scanning area.
    //    QSCAN_VIEW_UNITS,
    RESOLUTION, //! Scanner resolution. Identical to X and Y if they are the same, -1 otherwise.
    //    RESOLUTION_RANGE, //! The possible resolution values, either as a min/max range or a list.
    //    QSCAN_RESOLUTION_UNITS, //! Scanner units.
    X_RESOLUTION, //! Scanner X resolution. Same as QSCAN_RESOLUTION if X and Y are the same.
    Y_RESOLUTION, //! Scanner Y resolution. Same as QSCAN_RESOLUTION if X and Y are the same.
    BRIGHTNESS, //! Scanner brightness
    CONTRAST, //! Scanner contrast
    //    QSCAN_GRAIN_SIZE,
    //    QSCAN_HALFTONE, //!
    //    QSCAN_HALFTONES, //!
    //    QSCAN_BLACK_LEVEL,
    //    QSCAN_WHITE_LEVEL,
    //    QSCAN_WHITE_LEVEL_R,
    //    QSCAN_WHITE_LEVEL_G,
    //    QSCAN_WHITE_LEVEL_B,
    //    QSCAN_SHADOW,
    //    QSCAN_SHADOW_R,
    //    QSCAN_SHADOW_G,
    //    QSCAN_SHADOW_B,
    //    QSCAN_HIGHLIGHT,
    //    QSCAN_HIGHLIGHT_R,
    //    QSCAN_HIGHLIGHT_G,
    //    QSCAN_HIGHLIGHT_B,
    //    QSCAN_HUE,
    //    QSCAN_SATURATION,
    //    QSCAN_FILE,
    //    QSCAN_HALFTONE_DIMENSION,
    //    QSCAN_HALFTONE_PATTERN,
    //    QSCAN_RESOLUTION_BIND,
    //    QSCAN_NEGATIVE,
    //    QSCAN_QUALITY_CAL,
    //    QSCAN_DOR,
    //    QSCAN_RGB_BIND,
    //    QSCAN_THRESHOLD,
    //    QSCAN_ANALOG_GAMMA,
    //    QSCAN_ANALOG_GAMMA_R,
    //    QSCAN_ANALOG_GAMMA_G,
    //    QSCAN_ANALOG_GAMMA_B,
    //    QSCAN_ANALOG_GAMMA_BIND,
    //    QSCAN_WARMUP,
    //    QSCAN_CAL_EXPOS_TIME,
    //    QSCAN_CAL_EXPOS_TIME_R,
    //    QSCAN_CAL_EXPOS_TIME_G,
    //    QSCAN_CAL_EXPOS_TIME_B,
    //    QSCAN_SCAN_EXPOS_TIME,
    //    QSCAN_SCAN_EXPOS_TIME_R,
    //    QSCAN_SCAN_EXPOS_TIME_G,
    //    QSCAN_SCAN_EXPOS_TIME_B,
    //    QSCAN_SELECT_EXPOSURE_TIME,
    //    QSCAN_CAL_LAMP_DEN,
    //    QSCAN_SCAN_LAMP_DEN,
    //    QSCAN_SELECT_LAMP_DENSITY,
    //    QSCAN_LAMP_OFF_AT_EXIT,
    //    QSCAN_SCAN,
    //    QSCAN_EMAIL,
    //    QSCAN_FAX,
    //    QSCAN_COPY,
    //    QSCAN_PDF,
    //    QSCAN_CANCEL,
    //    QSCAN_PAGE_LOADED,
    //    QSCAN_COVER_OPEN,
  };

  explicit ScanOptions(QObject* parent = nullptr);
  ScanOptions(const ScanOptions& other);
  ~ScanOptions() override;

  ScanOptions& operator=(const ScanOptions& rhs);

  bool isAvailable(AvailableOptions option);
  bool isAvailable(const QString& option);
  QVariant optionValue(AvailableOptions option);
  QVariant optionValue(const QString& option);
  QVariant optionData(AvailableOptions option);
  QVariant optionData(const QString& option);
  void setOptionValue(AvailableOptions option, const QVariant& value);
  void setOptionValue(const QString& option, const QVariant& value);
  QList<ScanOptions::AvailableOptions> standardOptions();
  QStringList nonStandardOptions();

  /*!
     \brief The available scanner modes.

     A list of the mode names. This is of the form "Color", "Black and White" and "Gray"
     but can include many other values.
  */
  QStringList modes() const;

  /*!
     \brief The available scanner source names.

     A list of the name strings of the scanner sources. This is normally such values
     as "Flatbed" or "Automatic Document feeder" and are used to tell the scanner exactly
     which source to use.
  */
  QStringList sources() const;

  /*!
     \brief The right
     \return
  */
  int left() const;
  void setLeft(int left);
  int top() const;
  void setTop(int top);
  int right() const;
  void setRight(int right);
  int bottom() const;
  void setBottom(int bottom);
  int height() const;
  void setHeight(const int height);
  int width() const;
  void setWidth(const int width);
  QPoint bottomRight();
  QPoint topLeft();
  QRect geometry();
  void setGeometry(QRect geometry);

  int depth() const;
  void setDepth(int depth);
  int brightness() const;
  void setBrightness(int brightness);
  int contrast() const;
  void setContrast(int contrast);

  int resolutionX() const;
  void setResolutionX(int scan_resolution_x);
  int resolutionY() const;
  void setResolutionY(int scan_resolution_y);
  int resolution() const;
  void setResolution(int scan_resolution);
  //  bool isResolutionRange();
  //  bool isResolutionList();
  //  void setResulutionRange(RangeData scan_range);
  //  void setResulutionRange(QList<int> scan_values);
  //  QVariant resolutionRange();

  int optionId(AvailableOptions option) const;
  void setOptionId(AvailableOptions option, int option_id);
  size_t optionSize(AvailableOptions option) const;
  void setOptionSize(AvailableOptions option, size_t size);
  int optionType(AvailableOptions option) const;
  void setOptionType(AvailableOptions option, int type);

  QString mode() const;
  QString source() const;

  ScanUnits units(AvailableOptions option) const;
  void setUnits(AvailableOptions option, ScanUnits units);
  void setUnits(QString option, ScanUnits units);
  bool isDpi(AvailableOptions option);
  bool isMM(AvailableOptions option);
  QString unitsToString(AvailableOptions option);

  static QString optionDescripterString(AvailableOptions option);
  static AvailableOptions getAvailableOption(const QString& descriptor);
  //  static AvailableOptions getMatchingListOption(AvailableOptions option);
  static ScanUnits getScanUnits(const int units);

  QString optionDesc(AvailableOptions option) const;
  void setOptionDesc(AvailableOptions option, const QString& desc);
  QString optionName(AvailableOptions option) const;
  void setOptionName(const AvailableOptions option, const QString& name);
  QString optionTitle(AvailableOptions option) const;
  void setOptionTitle(const AvailableOptions option, const QString& title);

private:
  QMap<AvailableOptions, int> m_option_id_map;
  QMap<AvailableOptions, size_t> m_option_size_map;
  QMap<AvailableOptions, int> m_option_type_map;

  QList<AvailableOptions> m_available_options;
  QList<QString> m_ns_options;

  QMap<AvailableOptions, QVariant> m_option_values;
  QMap<AvailableOptions, ScanUnits> m_units;
  QMap<AvailableOptions, QVariant> m_list_option_values;

  QMap<QString, QVariant> m_ns_options_values;
  QMap<QString, QVariant> m_list_ns_options_values;
  QMap<QString, ScanUnits> m_ns_units;

  QMap<AvailableOptions, QString> m_option_desc;
  QMap<AvailableOptions, QString> m_option_names;
  QMap<AvailableOptions, QString> m_option_titles;

  static const QMap<AvailableOptions, QString> m_option_to_string;
  static const QMap<QString, AvailableOptions> m_string_to_option;

  QVariant getValue(ScanOptions::AvailableOptions option) const;
  QVariant getValue(const QString& option) const;
  QVariant getValueData(ScanOptions::AvailableOptions option) const;
  QVariant getValueData(const QString& option) const;
  void setValue(ScanOptions::AvailableOptions option, const QVariant& value);
  void setValue(const QString& option, const QVariant& value);
  void copyData(const ScanOptions& other);

  //  static const QString STANDARD;
  //  static const QString GEOMETRY;
  //  static const QString ENHANCEMENT;
  //  static const QString ADVANCED;
  //  static const QString SENSORS;
  //  static const QString PREVIEW;
  //  static const QString GRAY_PREVIEW;
  static const QString SCAN_DEPTH;
  static const QString SCAN_MODE;
  //  static const QString SPEED;
  static const QString SCAN_SOURCE;
  //  static const QString BACKTRACK;
  static const QString SCAN_LEFT;
  static const QString SCAN_TOP;
  static const QString SCAN_RIGHT;
  static const QString SCAN_BOTTOM;
  static const QString SCAN_RESOLUTION;
  static const QString SCAN_X_RESOLUTION;
  static const QString SCAN_Y_RESOLUTION;
  //  static const QString PAGE_WIDTH;
  //  static const QString PAGE_HEIGHT;
  //  static const QString CUSTOM_GAMMA;
  //  static const QString GAMMA_VECTOR;
  //  static const QString GAMMA_VECTOR_R;
  //  static const QString GAMMA_VECTOR_G;
  //  static const QString GAMMA_VECTOR_B;
  static const QString SCAN_BRIGHTNESS;
  static const QString SCAN_CONTRAST;
  //  static const QString GRAIN_SIZE;
  //  static const QString HALFTONE;
  //  static const QString BLACK_LEVEL;
  //  static const QString WHITE_LEVEL;
  //  static const QString WHITE_LEVEL_R;
  //  static const QString WHITE_LEVEL_G;
  //  static const QString WHITE_LEVEL_B;
  //  static const QString SHADOW;
  //  static const QString SHADOW_R;
  //  static const QString SHADOW_G;
  //  static const QString SHADOW_B;
  //  static const QString HIGHLIGHT;
  //  static const QString HIGHLIGHT_R;
  //  static const QString HIGHLIGHT_G;
  //  static const QString HIGHLIGHT_B;
  //  static const QString HUE;
  //  static const QString SATURATION;
  //  static const QString FILE;
  //  static const QString HALFTONE_DIMENSION;
  //  static const QString HALFTONE_PATTERN;
  //  static const QString RESOLUTION_BIND;
  //  static const QString NEGATIVE;
  //  static const QString QUALITY_CAL;
  //  static const QString DOR;
  //  static const QString RGB_BIND;
  //  static const QString THRESHOLD;
  //  static const QString ANALOG_GAMMA;
  //  static const QString ANALOG_GAMMA_R;
  //  static const QString ANALOG_GAMMA_G;
  //  static const QString ANALOG_GAMMA_B;
  //  static const QString ANALOG_GAMMA_BIND;
  //  static const QString WARMUP;
  //  static const QString CAL_EXPOS_TIME;
  //  static const QString CAL_EXPOS_TIME_R;
  //  static const QString CAL_EXPOS_TIME_G;
  //  static const QString CAL_EXPOS_TIME_B;
  //  static const QString SCAN_EXPOS_TIME;
  //  static const QString SCAN_EXPOS_TIME_R;
  //  static const QString SCAN_EXPOS_TIME_G;
  //  static const QString SCAN_EXPOS_TIME_B;
  //  static const QString SELECT_EXPOSURE_TIME;
  //  static const QString CAL_LAMP_DEN;
  //  static const QString SCAN_LAMP_DEN;
  //  static const QString SELECT_LAMP_DENSITY;
  //  static const QString LAMP_OFF_AT_EXIT;
  //  static const QString SCAN;
  //  static const QString EMAIL;
  //  static const QString FAX;
  //  static const QString COPY;
  //  static const QString PDF;
  //  static const QString CANCEL;
  //  static const QString PAGE_LOADED;
  //  static const QString COVER_OPEN;

  // Non standard strings
  // Epson
  static const QString DROPOUT;
  static const QString BRIGHTNESS_METHOD;

};
using OptionsMap = QMap<QString, ScanOptions>;

} // end of namespace QScanner

Q_DECLARE_METATYPE(QScanner::RangeData);
//Q_DECLARE_METATYPE(QScanner::ScanRange);
Q_DECLARE_METATYPE(QScanner::ScanOptions)
Q_DECLARE_METATYPE(QScanner::ScanOptions::ScanUnits)


#endif // SCANOPTIONS_H
