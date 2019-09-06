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
#include <cstring>

#include "scanoptions.h"
#include "iscanlibrary.h"

namespace QScanner {

// These are only used internally to ScanOptions.
//const QString ScanOptions::STANDARD = SANE_NAME_STANDARD;
//const QString ScanOptions::GEOMETRY = SANE_NAME_GEOMETRY;
//const QString ScanOptions::ENHANCEMENT = SANE_NAME_ENHANCEMENT;
//const QString ScanOptions::ADVANCED = SANE_NAME_ADVANCED;
//const QString ScanOptions::SENSORS = SANE_NAME_SENSORS;
//const QString ScanOptions::PREVIEW = SANE_NAME_PREVIEW;
//const QString ScanOptions::GRAY_PREVIEW = SANE_NAME_GRAY_PREVIEW;
const QString ScanOptions::SCAN_DEPTH = SANE_NAME_BIT_DEPTH;
const QString ScanOptions::SCAN_MODE = SANE_NAME_SCAN_MODE;
//const QString ScanOptions::SPEED = SANE_NAME_SCAN_SPEED;
const QString ScanOptions::SCAN_SOURCE = SANE_NAME_SCAN_SOURCE;
//const QString ScanOptions::BACKTRACK = SANE_NAME_BACKTRACK;
const QString ScanOptions::SCAN_LEFT = SANE_NAME_SCAN_TL_X;
const QString ScanOptions::SCAN_TOP = SANE_NAME_SCAN_TL_Y;
const QString ScanOptions::SCAN_RIGHT = SANE_NAME_SCAN_BR_X;
const QString ScanOptions::SCAN_BOTTOM = SANE_NAME_SCAN_BR_Y;
const QString ScanOptions::SCAN_RESOLUTION = SANE_NAME_SCAN_RESOLUTION;
const QString ScanOptions::SCAN_X_RESOLUTION = SANE_NAME_SCAN_X_RESOLUTION;
const QString ScanOptions::SCAN_Y_RESOLUTION = SANE_NAME_SCAN_Y_RESOLUTION;
//const QString ScanOptions::PAGE_WIDTH = SANE_NAME_PAGE_WIDTH;
//const QString ScanOptions::PAGE_HEIGHT = SANE_NAME_PAGE_HEIGHT;
//const QString ScanOptions::CUSTOM_GAMMA = SANE_NAME_CUSTOM_GAMMA;
//const QString ScanOptions::GAMMA_VECTOR = SANE_NAME_GAMMA_VECTOR;
//const QString ScanOptions::GAMMA_VECTOR_R = SANE_NAME_GAMMA_VECTOR_R;
//const QString ScanOptions::GAMMA_VECTOR_G = SANE_NAME_GAMMA_VECTOR_G;
//const QString ScanOptions::GAMMA_VECTOR_B = SANE_NAME_GAMMA_VECTOR_B;
const QString ScanOptions::SCAN_BRIGHTNESS = SANE_NAME_BRIGHTNESS;
const QString ScanOptions::SCAN_CONTRAST = SANE_NAME_CONTRAST;
//const QString ScanOptions::GRAIN_SIZE = SANE_NAME_GRAIN_SIZE;
//const QString ScanOptions::HALFTONE = SANE_NAME_HALFTONE;
//const QString ScanOptions::BLACK_LEVEL = SANE_NAME_BLACK_LEVEL;
//const QString ScanOptions::WHITE_LEVEL = SANE_NAME_WHITE_LEVEL;
//const QString ScanOptions::WHITE_LEVEL_R = SANE_NAME_WHITE_LEVEL_R;
//const QString ScanOptions::WHITE_LEVEL_G = SANE_NAME_WHITE_LEVEL_G;
//const QString ScanOptions::WHITE_LEVEL_B = SANE_NAME_WHITE_LEVEL_B;
//const QString ScanOptions::SHADOW = SANE_NAME_SHADOW;
//const QString ScanOptions::SHADOW_R = SANE_NAME_SHADOW_R;
//const QString ScanOptions::SHADOW_G = SANE_NAME_SHADOW_G;
//const QString ScanOptions::SHADOW_B = SANE_NAME_SHADOW_B;
//const QString ScanOptions::HIGHLIGHT = SANE_NAME_HIGHLIGHT;
//const QString ScanOptions::HIGHLIGHT_R = SANE_NAME_HIGHLIGHT_R;
//const QString ScanOptions::HIGHLIGHT_G = SANE_NAME_HIGHLIGHT_G;
//const QString ScanOptions::HIGHLIGHT_B = SANE_NAME_HIGHLIGHT_B;
//const QString ScanOptions::HUE = SANE_NAME_HUE;
//const QString ScanOptions::SATURATION = SANE_NAME_SATURATION;
//const QString ScanOptions::FILE = SANE_NAME_FILE;
//const QString ScanOptions::HALFTONE_DIMENSION = SANE_NAME_HALFTONE_DIMENSION;
//const QString ScanOptions::HALFTONE_PATTERN = SANE_NAME_HALFTONE_PATTERN;
//const QString ScanOptions::RESOLUTION_BIND = SANE_NAME_RESOLUTION_BIND;
//const QString ScanOptions::NEGATIVE = SANE_NAME_NEGATIVE;
//const QString ScanOptions::QUALITY_CAL = SANE_NAME_QUALITY_CAL;
//const QString ScanOptions::DOR = SANE_NAME_DOR;
//const QString ScanOptions::RGB_BIND = SANE_NAME_RGB_BIND;
//const QString ScanOptions::THRESHOLD = SANE_NAME_THRESHOLD;
//const QString ScanOptions::ANALOG_GAMMA = SANE_NAME_ANALOG_GAMMA;
//const QString ScanOptions::ANALOG_GAMMA_R = SANE_NAME_ANALOG_GAMMA_R;
//const QString ScanOptions::ANALOG_GAMMA_G = SANE_NAME_ANALOG_GAMMA_G;
//const QString ScanOptions::ANALOG_GAMMA_B = SANE_NAME_ANALOG_GAMMA_B;
//const QString ScanOptions::ANALOG_GAMMA_BIND = SANE_NAME_ANALOG_GAMMA_BIND;
//const QString ScanOptions::WARMUP = SANE_NAME_WARMUP;
//const QString ScanOptions::CAL_EXPOS_TIME = SANE_NAME_CAL_EXPOS_TIME;
//const QString ScanOptions::CAL_EXPOS_TIME_R = SANE_NAME_CAL_EXPOS_TIME_R;
//const QString ScanOptions::CAL_EXPOS_TIME_G = SANE_NAME_CAL_EXPOS_TIME_G;
//const QString ScanOptions::CAL_EXPOS_TIME_B = SANE_NAME_CAL_EXPOS_TIME_B;
//const QString ScanOptions::SCAN_EXPOS_TIME = SANE_NAME_SCAN_EXPOS_TIME;
//const QString ScanOptions::SCAN_EXPOS_TIME_R = SANE_NAME_SCAN_EXPOS_TIME_R;
//const QString ScanOptions::SCAN_EXPOS_TIME_G = SANE_NAME_SCAN_EXPOS_TIME_G;
//const QString ScanOptions::SCAN_EXPOS_TIME_B = SANE_NAME_SCAN_EXPOS_TIME_B;
//const QString ScanOptions::SELECT_EXPOSURE_TIME = SANE_NAME_SELECT_EXPOSURE_TIME;
//const QString ScanOptions::CAL_LAMP_DEN = SANE_NAME_CAL_LAMP_DEN;
//const QString ScanOptions::SCAN_LAMP_DEN = SANE_NAME_SCAN_LAMP_DEN;
//const QString ScanOptions::SELECT_LAMP_DENSITY = SANE_NAME_SELECT_LAMP_DENSITY;
//const QString ScanOptions::LAMP_OFF_AT_EXIT = SANE_NAME_LAMP_OFF_AT_EXIT;

//const QString ScanOptions::SCAN = SANE_NAME_SCAN;
//const QString ScanOptions::EMAIL = SANE_NAME_EMAIL;
//const QString ScanOptions::FAX = SANE_NAME_FAX;
//const QString ScanOptions::COPY = SANE_NAME_COPY;
//const QString ScanOptions::PDF = SANE_NAME_PDF;
//const QString ScanOptions::CANCEL = SANE_NAME_CANCEL;
//const QString ScanOptions::PAGE_LOADED = SANE_NAME_PAGE_LOADED;
//const QString ScanOptions::COVER_OPEN = SANE_NAME_COVER_OPEN;

// Epson special values.
const QString ScanOptions::DROPOUT = "dropout";
const QString ScanOptions::BRIGHTNESS_METHOD = "brightness-method";

const QMap<ScanOptions::AvailableOptions, QString> ScanOptions::m_option_to_string {
  //  {QSCAN_STANDARD, SANE_NAME_STANDARD},
  //  {QSCAN_GEOMETRY, SANE_NAME_GEOMETRY},
  //  {QSCAN_ENHANCEMENT, SANE_NAME_ENHANCEMENT},
  //  {QSCAN_ADVANCED, SANE_NAME_ADVANCED},
  //  {QSCAN_SENSORS, SANE_NAME_SENSORS},
  //  {QSCAN_PREVIEW, SANE_NAME_PREVIEW},
  //  {QSCAN_GRAY_PREVIEW, SANE_NAME_GRAY_PREVIEW},
  {BIT_DEPTH, SANE_NAME_BIT_DEPTH},
  {MODE, SANE_NAME_SCAN_MODE},
  //  {SPEED, SANE_NAME_SCAN_SPEED},
  {SOURCE, SANE_NAME_SCAN_SOURCE},
  //  {BACKTRACK, SANE_NAME_BACKTRACK},
  {LEFT, SANE_NAME_SCAN_TL_X},
  {TOP, SANE_NAME_SCAN_TL_Y},
  {RIGHT, SANE_NAME_SCAN_BR_X},
  {BOTTOM, SANE_NAME_SCAN_BR_Y},
  {RESOLUTION, SANE_NAME_SCAN_RESOLUTION},
  {X_RESOLUTION, SANE_NAME_SCAN_X_RESOLUTION},
  {Y_RESOLUTION, SANE_NAME_SCAN_Y_RESOLUTION},
  //  {PAGE_WIDTH, SANE_NAME_PAGE_WIDTH},
  //  {PAGE_HEIGHT, SANE_NAME_PAGE_HEIGHT},
  //  {CUSTOM_GAMMA, SANE_NAME_CUSTOM_GAMMA},
  //  {GAMMA_VECTOR, SANE_NAME_GAMMA_VECTOR},
  //  {GAMMA_VECTOR_R, SANE_NAME_GAMMA_VECTOR_R},
  //  {GAMMA_VECTOR_G, SANE_NAME_GAMMA_VECTOR_G},
  //  {GAMMA_VECTOR_B, SANE_NAME_GAMMA_VECTOR_B},
  {BRIGHTNESS, SANE_NAME_BRIGHTNESS},
  {CONTRAST, SANE_NAME_CONTRAST},
  //  {QSCAN_GRAIN_SIZE, SANE_NAME_GRAIN_SIZE},
  //  {QSCAN_HALFTONE, SANE_NAME_HALFTONE},
  //  {QSCAN_BLACK_LEVEL, SANE_NAME_BLACK_LEVEL},
  //  {QSCAN_WHITE_LEVEL, SANE_NAME_WHITE_LEVEL},
  //  {QSCAN_WHITE_LEVEL_R, SANE_NAME_WHITE_LEVEL_R},
  //  {QSCAN_WHITE_LEVEL_G, SANE_NAME_WHITE_LEVEL_G},
  //  {QSCAN_WHITE_LEVEL_B, SANE_NAME_WHITE_LEVEL_B},
  //  {QSCAN_SHADOW, SANE_NAME_SHADOW},
  //  {QSCAN_SHADOW_R, SANE_NAME_SHADOW_R},
  //  {QSCAN_SHADOW_G, SANE_NAME_SHADOW_G},
  //  {QSCAN_SHADOW_B, SANE_NAME_SHADOW_B},
  //  {QSCAN_HIGHLIGHT, SANE_NAME_HIGHLIGHT},
  //  {QSCAN_HIGHLIGHT_R, SANE_NAME_HIGHLIGHT_R},
  //  {QSCAN_HIGHLIGHT_G, SANE_NAME_HIGHLIGHT_G},
  //  {QSCAN_HIGHLIGHT_B, SANE_NAME_HIGHLIGHT_B},
  //  {QSCAN_HUE, SANE_NAME_HUE},
  //  {QSCAN_SATURATION, SANE_NAME_SATURATION},
  //  {QSCAN_FILE, SANE_NAME_FILE},
  //  {QSCAN_HALFTONE_DIMENSION, SANE_NAME_HALFTONE_DIMENSION},
  //  {QSCAN_HALFTONE_PATTERN, SANE_NAME_HALFTONE_PATTERN},
  //  {QSCAN_RESOLUTION_BIND, SANE_NAME_RESOLUTION_BIND},
  //  {QSCAN_NEGATIVE, SANE_NAME_NEGATIVE},
  //  {QSCAN_QUALITY_CAL, SANE_NAME_QUALITY_CAL},
  //  {QSCAN_DOR, SANE_NAME_DOR},
  //  {QSCAN_RGB_BIND, SANE_NAME_RGB_BIND},
  //  {QSCAN_THRESHOLD, SANE_NAME_THRESHOLD},
  //  {QSCAN_ANALOG_GAMMA, SANE_NAME_ANALOG_GAMMA},
  //  {QSCAN_ANALOG_GAMMA_R, SANE_NAME_ANALOG_GAMMA_R},
  //  {QSCAN_ANALOG_GAMMA_G, SANE_NAME_ANALOG_GAMMA_G},
  //  {QSCAN_ANALOG_GAMMA_B, SANE_NAME_ANALOG_GAMMA_B},
  //  {QSCAN_ANALOG_GAMMA_BIND, SANE_NAME_ANALOG_GAMMA_BIND},
  //  {QSCAN_WARMUP, SANE_NAME_WARMUP},
  //  {QSCAN_CAL_EXPOS_TIME, SANE_NAME_CAL_EXPOS_TIME},
  //  {QSCAN_CAL_EXPOS_TIME_R, SANE_NAME_CAL_EXPOS_TIME_R},
  //  {QSCAN_CAL_EXPOS_TIME_G, SANE_NAME_CAL_EXPOS_TIME_G},
  //  {QSCAN_CAL_EXPOS_TIME_B, SANE_NAME_CAL_EXPOS_TIME_B},
  //  {QSCAN_SCAN_EXPOS_TIME, SANE_NAME_SCAN_EXPOS_TIME},
  //  {QSCAN_SCAN_EXPOS_TIME_R, SANE_NAME_SCAN_EXPOS_TIME_R},
  //  {QSCAN_SCAN_EXPOS_TIME_G, SANE_NAME_SCAN_EXPOS_TIME_G},
  //  {QSCAN_SCAN_EXPOS_TIME_B, SANE_NAME_SCAN_EXPOS_TIME_B},
  //  {QSCAN_SELECT_EXPOSURE_TIME, SANE_NAME_SELECT_EXPOSURE_TIME},
  //  {QSCAN_CAL_LAMP_DEN, SANE_NAME_CAL_LAMP_DEN},
  //  {QSCAN_SCAN_LAMP_DEN, SANE_NAME_SCAN_LAMP_DEN},
  //  {QSCAN_SELECT_LAMP_DENSITY, SANE_NAME_SELECT_LAMP_DENSITY},
  //  {QSCAN_LAMP_OFF_AT_EXIT, SANE_NAME_LAMP_OFF_AT_EXIT},
  //  {QSCAN_SCAN, SANE_NAME_SCAN},
  //  {QSCAN_EMAIL, SANE_NAME_EMAIL},
  //  {QSCAN_FAX, SANE_NAME_FAX},
  //  {QSCAN_COPY, SANE_NAME_COPY},
  //  {QSCAN_PDF, SANE_NAME_PDF},
  //  {QSCAN_CANCEL, SANE_NAME_CANCEL},
  //  {QSCAN_PAGE_LOADED, SANE_NAME_PAGE_LOADED},
  //  {QSCAN_COVER_OPEN, SANE_NAME_COVER_OPEN},
};

const QMap<QString, ScanOptions::AvailableOptions> ScanOptions::m_string_to_option{
  //  {SANE_NAME_STANDARD, QSCAN_STANDARD},
  //  {SANE_NAME_GEOMETRY, QSCAN_GEOMETRY},
  //  {SANE_NAME_ENHANCEMENT, QSCAN_ENHANCEMENT},
  //  {SANE_NAME_ADVANCED, QSCAN_ADVANCED},
  //  {SANE_NAME_SENSORS, QSCAN_SENSORS},
  //  {SANE_NAME_PREVIEW, QSCAN_PREVIEW},
  //  {SANE_NAME_GRAY_PREVIEW, QSCAN_GRAY_PREVIEW},
  {SANE_NAME_BIT_DEPTH, BIT_DEPTH},
  {SANE_NAME_SCAN_MODE, MODE},
  //  {SANE_NAME_SCAN_SPEED, QSCAN_SPEED,},
  {SANE_NAME_SCAN_SOURCE, SOURCE},
  //  {SANE_NAME_BACKTRACK, QSCAN_BACKTRACK},
  {SANE_NAME_SCAN_TL_X, LEFT},
  {SANE_NAME_SCAN_TL_Y, TOP},
  {SANE_NAME_SCAN_BR_X, RIGHT},
  {SANE_NAME_SCAN_BR_Y, BOTTOM},
  {SANE_NAME_SCAN_RESOLUTION, RESOLUTION},
  {SANE_NAME_SCAN_X_RESOLUTION, X_RESOLUTION},
  {SANE_NAME_SCAN_Y_RESOLUTION, Y_RESOLUTION},
  //  {SANE_NAME_PAGE_WIDTH, QSCAN_PAGE_WIDTH},
  //  {SANE_NAME_PAGE_HEIGHT, QSCAN_PAGE_HEIGHT},
  //  {SANE_NAME_CUSTOM_GAMMA, QSCAN_CUSTOM_GAMMA},
  //  {SANE_NAME_GAMMA_VECTOR, QSCAN_GAMMA_VECTOR},
  //  {SANE_NAME_GAMMA_VECTOR_R, QSCAN_GAMMA_VECTOR_R},
  //  {SANE_NAME_GAMMA_VECTOR_G, QSCAN_GAMMA_VECTOR_G},
  //  {SANE_NAME_GAMMA_VECTOR_B, QSCAN_GAMMA_VECTOR_B},
  {SANE_NAME_BRIGHTNESS, BRIGHTNESS},
  {SANE_NAME_CONTRAST, CONTRAST},
  //  {SANE_NAME_GRAIN_SIZE, QSCAN_GRAIN_SIZE},
  //  {SANE_NAME_HALFTONE, QSCAN_HALFTONE},
  //  {SANE_NAME_BLACK_LEVEL, QSCAN_BLACK_LEVEL},
  //  {SANE_NAME_WHITE_LEVEL, QSCAN_WHITE_LEVEL},
  //  {SANE_NAME_WHITE_LEVEL_R, QSCAN_WHITE_LEVEL_R},
  //  {SANE_NAME_WHITE_LEVEL_G, QSCAN_WHITE_LEVEL_G},
  //  {SANE_NAME_WHITE_LEVEL_B, QSCAN_WHITE_LEVEL_B},
  //  {SANE_NAME_SHADOW, QSCAN_SHADOW},
  //  {SANE_NAME_SHADOW_R, QSCAN_SHADOW_R},
  //  {SANE_NAME_SHADOW_G, QSCAN_SHADOW_G},
  //  {SANE_NAME_SHADOW_B, QSCAN_SHADOW_B},
  //  {SANE_NAME_HIGHLIGHT, QSCAN_HIGHLIGHT},
  //  {SANE_NAME_HIGHLIGHT_R, QSCAN_HIGHLIGHT_R},
  //  {SANE_NAME_HIGHLIGHT_G, QSCAN_HIGHLIGHT_G},
  //  {SANE_NAME_HIGHLIGHT_B, QSCAN_HIGHLIGHT_B},
  //  {SANE_NAME_HUE, QSCAN_HUE},
  //  {SANE_NAME_SATURATION, QSCAN_SATURATION},
  //  {SANE_NAME_FILE, QSCAN_FILE},
  //  {SANE_NAME_HALFTONE_DIMENSION, QSCAN_HALFTONE_DIMENSION},
  //  {SANE_NAME_HALFTONE_PATTERN, QSCAN_HALFTONE_PATTERN},
  //  {SANE_NAME_RESOLUTION_BIND, QSCAN_RESOLUTION_BIND},
  //  {SANE_NAME_NEGATIVE, QSCAN_NEGATIVE},
  //  {SANE_NAME_QUALITY_CAL, QSCAN_QUALITY_CAL},
  //  {SANE_NAME_DOR, QSCAN_DOR},
  //  {SANE_NAME_RGB_BIND, QSCAN_RGB_BIND},
  //  {SANE_NAME_THRESHOLD, QSCAN_THRESHOLD},
  //  {SANE_NAME_ANALOG_GAMMA, QSCAN_ANALOG_GAMMA},
  //  {SANE_NAME_ANALOG_GAMMA_R, QSCAN_ANALOG_GAMMA_R},
  //  {SANE_NAME_ANALOG_GAMMA_G, QSCAN_ANALOG_GAMMA_G},
  //  {SANE_NAME_ANALOG_GAMMA_B, QSCAN_ANALOG_GAMMA_B},
  //  {SANE_NAME_ANALOG_GAMMA_BIND, QSCAN_ANALOG_GAMMA_BIND},
  //  {SANE_NAME_WARMUP, QSCAN_WARMUP},
  //  {SANE_NAME_CAL_EXPOS_TIME, QSCAN_CAL_EXPOS_TIME},
  //  {SANE_NAME_CAL_EXPOS_TIME_R, QSCAN_CAL_EXPOS_TIME_R},
  //  {SANE_NAME_CAL_EXPOS_TIME_G, QSCAN_CAL_EXPOS_TIME_G},
  //  {SANE_NAME_CAL_EXPOS_TIME_B, QSCAN_CAL_EXPOS_TIME_B},
  //  {SANE_NAME_SCAN_EXPOS_TIME, QSCAN_SCAN_EXPOS_TIME},
  //  {SANE_NAME_SCAN_EXPOS_TIME_R, QSCAN_SCAN_EXPOS_TIME_R},
  //  {SANE_NAME_SCAN_EXPOS_TIME_G, QSCAN_SCAN_EXPOS_TIME_G},
  //  {SANE_NAME_SCAN_EXPOS_TIME_B, QSCAN_SCAN_EXPOS_TIME_B},
  //  {SANE_NAME_SELECT_EXPOSURE_TIME, QSCAN_SELECT_EXPOSURE_TIME},
  //  {SANE_NAME_CAL_LAMP_DEN, QSCAN_CAL_LAMP_DEN},
  //  {SANE_NAME_SCAN_LAMP_DEN, QSCAN_SCAN_LAMP_DEN},
  //  {SANE_NAME_SELECT_LAMP_DENSITY, QSCAN_SELECT_LAMP_DENSITY},
  //  {SANE_NAME_LAMP_OFF_AT_EXIT, QSCAN_LAMP_OFF_AT_EXIT},
  //  {SANE_NAME_SCAN, QSCAN_SCAN},
  //  {SANE_NAME_EMAIL, QSCAN_EMAIL},
  //  {SANE_NAME_FAX, QSCAN_FAX},
  //  {SANE_NAME_COPY, QSCAN_COPY},
  //  {SANE_NAME_PDF, QSCAN_PDF},
  //  {SANE_NAME_CANCEL, QSCAN_CANCEL},
  //  {SANE_NAME_PAGE_LOADED, QSCAN_PAGE_LOADED},
  //  {SANE_NAME_COVER_OPEN, QSCAN_COVER_OPEN},
};

ScanOptions::ScanOptions(QObject* parent)
  : QObject(parent)
{
}

ScanOptions::ScanOptions(const ScanOptions& other)
  : QObject(other.parent())
{
  copyData(other);
}

ScanOptions::~ScanOptions() = default;

void ScanOptions::copyData(const ScanOptions& other)
{
  //  m_type = other.m_type;
  m_option_id_map = other.m_option_id_map;
  m_option_size_map = other.m_option_size_map;
  m_option_type_map = other.m_option_type_map;
  m_available_options = other.m_available_options;
  m_ns_options_values = other.m_ns_options_values;
  m_option_values = other.m_option_values;
}

QString ScanOptions::ScanOptions::mode() const
{
  return getValue(MODE).toString();
}

QString ScanOptions::ScanOptions::source() const
{
  return  getValue(SOURCE).toString();
}

ScanOptions::ScanUnits ScanOptions::units(AvailableOptions option) const
{
  return m_units.value(option);
}

void ScanOptions::setUnits(ScanOptions::AvailableOptions option, ScanOptions::ScanUnits units)
{
  m_units.insert(option, units);
}

void ScanOptions::setUnits(QString option, ScanOptions::ScanUnits units)
{
  m_ns_units.insert(option, units);
}

bool ScanOptions::isDpi(AvailableOptions option)
{
  if (units(option) == ScanUnits::DPI) {
    return true;
  }

  return false;
}

bool ScanOptions::isMM(AvailableOptions option)
{
  if (units(option) == ScanUnits::MM) {
    return true;
  }

  return false;
}

QString ScanOptions::unitsToString(AvailableOptions option)
{
  switch (units(option)) {
  case PERCENT:
    return QStringLiteral("%");

  case MM:
    return QStringLiteral("mm");

  case DPI:
    return QStringLiteral("dpi");

  case BIT:
    return QStringLiteral("bits");

  case MICROSECOND:
    return QStringLiteral("µS");

  case NONE:
  default:
    return QString();
  }
}

// Generic internal methods.

QVariant ScanOptions::getValue(AvailableOptions option) const
{
  return m_option_values.value(option);
}

QVariant ScanOptions::getValue(const QString& option) const
{
  return m_ns_options_values.value(option);
}

QVariant ScanOptions::getValueData(ScanOptions::AvailableOptions option) const
{
  return m_list_option_values.value(option);
}

QVariant ScanOptions::getValueData(const QString& option) const
{
  return m_list_ns_options_values.value(option);
}

void ScanOptions::setValue(ScanOptions::AvailableOptions option, const QVariant& value)
{
  m_option_values.insert(option, value);
}

void ScanOptions::setValue(const QString& option, const QVariant& value)
{
  m_ns_options_values.insert(option, value);
}

ScanOptions& ScanOptions::operator=(const ScanOptions& rhs)
{
  copyData(rhs);
  return *this;
}

bool ScanOptions::isAvailable(AvailableOptions option)
{
  return m_available_options.contains(option);
}

bool ScanOptions::isAvailable(const QString& option)
{
  return m_ns_options_values.contains(option);
}


QVariant ScanOptions::optionValue(AvailableOptions option)
{
  if (isAvailable(option)) {
    return getValue(option);
  }

  return QVariant();
}

QVariant ScanOptions::optionValue(const QString& option)
{
  if (isAvailable(option)) {
    return getValue(option);
  }

  return QVariant();
}

QVariant ScanOptions::optionData(ScanOptions::AvailableOptions option)
{
  return getValueData(option);
}

QVariant ScanOptions::optionData(const QString& option)
{
  return getValueData(option);
}

void ScanOptions::setOptionValue(ScanOptions::AvailableOptions option, const QVariant& value)
{
  if (value.isValid()) {
    if (value.canConvert<QVariantList>()) {
      m_list_option_values.insert(option, value);

    } else if (value.canConvert<RangeData>()) {
      m_list_option_values.insert(option, value);

    } else {
      setValue(option, value);
    }

    if (!m_available_options.contains(option)) {
      m_available_options.append(option);
    }
  }
}

void ScanOptions::setOptionValue(const QString& option, const QVariant& value)
{
  if (value.isValid()) {
    if (value.canConvert<QVariantList>()) {
      m_list_ns_options_values.insert(option, value);

    } else if (value.canConvert<RangeData>()) {
      m_list_ns_options_values.insert(option, value);

    } else {
      setValue(option, value);
    }

    if (!m_ns_options.contains(option)) {
      m_ns_options.append(option);
    }
  }
}

QList<ScanOptions::AvailableOptions> ScanOptions::standardOptions()
{
  return m_available_options;
}

QStringList ScanOptions::nonStandardOptions()
{
  return m_ns_options;
}

QString ScanOptions::optionDescripterString(AvailableOptions option)
{
  return m_option_to_string.value(option);
}

ScanOptions::AvailableOptions ScanOptions::getAvailableOption(const QString& descriptor)
{
  if (m_string_to_option.contains(descriptor)) {
    return m_string_to_option.value(descriptor);
  }

  return QSCAN_OPTION_UNAVAILABLE;
}

///*!
//   \brief Recovers the matching list value if available.

//   Only certain AvailableOptions have matching lists, primarily source and mode
//   and sometimes resolution lists.
//*/
//ScanOptions::AvailableOptions ScanOptions::getMatchingListOption(ScanOptions::AvailableOptions option)
//{
//  switch (option) {
//  case QSCAN_SOURCE:
//    return QSCAN_SOURCES;

//  case MODE:
//    return QSCAN_MODES;

//  case QSCAN_RESOLUTION:
//    Q_FALLTHROUGH();

//  case QSCAN_X_RESOLUTION:
//    Q_FALLTHROUGH();

//  case QSCAN_Y_RESOLUTION:
//    return QSCAN_RESOLUTION_RANGE;

//  //  case QSCAN_HALFTONE:
//  //    return QSCAN_HALFTONES;

//  default:
//    return QSCAN_OPTION_UNAVAILABLE;
//  }
//}

ScanOptions::ScanUnits ScanOptions::getScanUnits(const int units)
{
  if (units == SANE_UNIT_DPI) {
    return DPI;

  } else if (units == SANE_UNIT_PIXEL) {
    return PIXEL;

  } else if (units == SANE_UNIT_PERCENT) {
    return PERCENT;

  } else if (units == SANE_UNIT_BIT) {
    return BIT;

  } else if (units == SANE_UNIT_MM) {
    return MM;
  }

  return NONE;
}

QString ScanOptions::optionDesc(AvailableOptions option) const
{
  return m_option_desc.value(option);
}

void ScanOptions::setOptionDesc(AvailableOptions option, const QString& desc)
{
  m_option_desc.insert(option, desc);
}

QString ScanOptions::optionName(AvailableOptions option) const
{
  return m_option_names.value(option);
}

void ScanOptions::setOptionName(AvailableOptions option, const QString& name)
{
  m_option_names.insert(option, name);
}

QString ScanOptions::optionTitle(AvailableOptions option) const
{
  return m_option_titles.value(option);
}

void ScanOptions::setOptionTitle(AvailableOptions option, const QString& title)
{
  m_option_titles.insert(option, title);
}

QStringList ScanOptions::modes() const
{
  return getValueData(MODE).toStringList();
}

QStringList ScanOptions::sources() const
{
  return getValueData(SOURCE).toStringList();
}

int ScanOptions::depth() const
{
  QVariant v = getValue(BIT_DEPTH);

  if (v.isValid() && (v.type() == QVariant::Int || v.type() == QVariant::Double)) {
    return v.toInt();
  }

  return -1;
}

void ScanOptions::setDepth(int depth)
{
  setValue(BIT_DEPTH, depth);
}

int ScanOptions::brightness() const
{
  QVariant v = getValue(BRIGHTNESS);

  if (v.isValid() && (v.type() == QVariant::Int || v.type() == QVariant::Double)) {
    return v.toInt();
  }

  return -1;
}

void ScanOptions::setBrightness(int brightness)
{
  setValue(BRIGHTNESS, brightness);
}

int ScanOptions::contrast() const
{
  QVariant v = getValue(CONTRAST);

  if (v.isValid() && (v.type() == QVariant::Int || v.type() == QVariant::Double)) {
    return v.toInt();
  }

  return -1;
}

void ScanOptions::setContrast(int contrast)
{
  setValue(CONTRAST, contrast);
}

int ScanOptions::left() const
{
  QVariant v = getValue(LEFT);

  if (v.isValid() && (v.type() == QVariant::Int || v.type() == QVariant::Double)) {
    return v.toInt();
  }

  return -1;
}

void ScanOptions::setLeft(int left)
{
  setValue(LEFT, left);
}

int ScanOptions::top() const
{
  QVariant v = getValue(TOP);

  if (v.isValid() && (v.type() == QVariant::Int || v.type() == QVariant::Double)) {
    return v.toInt();
  }

  return -1;
}

void ScanOptions::setTop(int top_left_y)
{
  setValue(TOP, top_left_y);
}

int ScanOptions::right() const
{
  QVariant v = getValue(RIGHT);

  if (v.isValid() && (v.type() == QVariant::Int || v.type() == QVariant::Double)) {
    return v.toInt();
  }

  return -1;
}

void ScanOptions::setRight(int right)
{
  setValue(RIGHT, right);
}

int ScanOptions::bottom() const
{
  QVariant v = getValue(BOTTOM);

  if (v.isValid() && (v.type() == QVariant::Int || v.type() == QVariant::Double)) {
    return v.toInt();
  }

  return -1;
}

void ScanOptions::setBottom(int bottom)
{
  setValue(BOTTOM, bottom);
}

QPoint ScanOptions::bottomRight()
{
  return QPoint(left(), right());
}

QPoint ScanOptions::topLeft()
{
  return QPoint(right(), top());
}

int ScanOptions::height() const
{
  return bottom() - top();
}

void ScanOptions::setHeight(const int height)
{
  int bottom = top() + height;
  setValue(BOTTOM, bottom);
}

int ScanOptions::width() const
{
  return right() - left();
}

void ScanOptions::setWidth(const int width)
{
  int right = left() + width;
  setLeft(right);
}

QRect ScanOptions::geometry()
{
  return QRect(right(), top(), width(), height());
}

void ScanOptions::setGeometry(QRect geometry)
{
  setLeft(geometry.left());
  setRight(geometry.right());
  setTop(geometry.top());
  setBottom(geometry.bottom());
}

int ScanOptions::resolutionX() const
{
  QVariant v = getValue(X_RESOLUTION);

  if (v.isValid() && v.type() == QVariant::Int) {
    return v.toInt();
  }

  return -1;
}

void ScanOptions::setResolutionX(int scan_resolution_x)
{
  setValue(X_RESOLUTION, scan_resolution_x);
}

int ScanOptions::resolutionY() const
{
  QVariant v = getValue(Y_RESOLUTION);

  if (v.isValid() && v.type() == QVariant::Int) {
    return v.toInt();
  }

  return -1;
}

void ScanOptions::setResolutionY(int scan_resolution_y)
{
  setValue(Y_RESOLUTION, scan_resolution_y);
}

/*!
   \brief Scan resolution in dots-per-inch.

   \return integer DPI value.
*/
int ScanOptions::resolution() const
{
  QVariant v = getValue(RESOLUTION);

  if (v.isValid() && v.type() == QVariant::Int) {
    return v.toInt();
  }

  return -1;
}

/*!
   \brief Set the scan resolution in dots-per-inch.

   \param integer DPI value.
*/
void ScanOptions::setResolution(int scan_resolution)
{
  setValue(RESOLUTION, scan_resolution);
  setResolutionX(scan_resolution);
  setResolutionY(scan_resolution);
}

//bool ScanOptions::isResolutionRange()
//{
//  // can only be a QList<int> or RangeData
//  QVariant v = resolutionRange();

//  if (v.canConvert<RangeData>()) {
//    return true;
//  }

//  return false;
//}

//bool ScanOptions::isResolutionList()
//{
//  // can only be a QList<int> or RangeData
//  QVariant v = resolutionRange();

//  if (v.canConvert<QList<int>>()) {
//    return true;
//  }

//  return false;
//}

//void ScanOptions::setResulutionRange(RangeData scan_range)
//{
//  setValue(RESOLUTION_RANGE, QVariant::fromValue<RangeData>(scan_range));
//}

//void ScanOptions::setResulutionRange(QList<int> scan_range)
//{
//  setValue(RESOLUTION_RANGE, QVariant::fromValue<QList<int>>(scan_range));
//}

//QVariant ScanOptions::resolutionRange()
//{
//  return getValue(RESOLUTION_RANGE);
//}

/*!
   \brief Recovers the option id number using it's name as a key.
*/
int ScanOptions::optionId(AvailableOptions option) const
{
  return m_option_id_map.value(option, -1);
}

/*!
   \brief Stores the option id using it's name as a key.
*/
void ScanOptions::setOptionId(AvailableOptions option, int option_id)
{
  m_option_id_map.insert(option, option_id);
}

/*!
   \brief Recovers the size of the option object using it's name as a key.

   This is primarily used to internally assign memory for the option.
*/
size_t ScanOptions::optionSize(AvailableOptions option) const
{
  return m_option_size_map.value(option);
}

/*!
   \brief Stores the size of the option object using it's name as a key.
*/
void ScanOptions::setOptionSize(AvailableOptions option, size_t size)
{
  m_option_size_map.insert(option, size);
}

/*!
   \brief Recovers the option type using it's name using it's name as a key.
*/
int ScanOptions::optionType(AvailableOptions option) const
{
  return m_option_type_map.value(option);
}

/*!
   \brief Stores the option type using it's name as a key.
*/
void ScanOptions::setOptionType(AvailableOptions option, int type)
{
  m_option_type_map.insert(option, type);
}

} // end of namespace QScanner
