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
#include "scanoptions.h"

#include <cstring>

#include "scaninterface.h"

ScanOptions::ScanOptions(QObject* parent)
  : QObject(parent)
  , m_units(ScanUnits::DPI)
{}

ScanOptions::ScanOptions(const ScanOptions& other)
{
  copyData(other);
}

ScanOptions::~ScanOptions() = default;

void ScanOptions::copyData(const ScanOptions& other)
{
  m_modes = other.m_modes;
  m_sources = other.m_sources;
  m_depth = other.m_depth;
  //  m_type = other.m_type;
  m_paper_width = other.m_paper_width;
  m_paper_height = other.m_paper_height;
  m_brightness = other.m_brightness;
  m_contrast = other.m_contrast;
  m_page_delay = other.m_page_delay;
  m_geometry = other.m_geometry;
  m_resolution = other.m_resolution;
  m_resolution_x = other.m_resolution_x;
  m_resolution_y = other.m_resolution_y;
  m_option_id_map = other.m_option_id_map;
  m_units = other.m_units;
}

QString ScanOptions::mode() const
{
  return m_mode;
}

void ScanOptions::setMode(const QString& mode)
{
  m_mode = mode;
}

QString ScanOptions::source() const
{
  return m_source;
}

void ScanOptions::setSource(const QString& source)
{
  m_source = source;
}

ScanUnits ScanOptions::units() const
{
  return m_units;
}

void ScanOptions::setUnits(const ScanUnits& units)
{
  m_units = units;
}

// int
// ScanOptions::minResolution() const
//{
//  return m_min_resolution;
//}

// void
// ScanOptions::setMinResolution(int min_resolution)
//{
//  m_min_resolution = min_resolution;
//}

// int
// ScanOptions::maxResolution() const
//{
//  return m_max_resolution;
//}

// void
// ScanOptions::setMaxResolution(int max_resolution)
//{
//  m_max_resolution = max_resolution;
//}

ScanOptions& ScanOptions::operator=(const ScanOptions& rhs)
{
  copyData(rhs);
  return *this;
}

QStringList ScanOptions::modes() const
{
  return m_modes;
}

void ScanOptions::setModes(const QStringList& modes)
{
  m_modes = modes;
}

QStringList ScanOptions::sources() const
{
  return m_sources;
}

void ScanOptions::setSources(const QStringList& sources)
{
  m_sources = sources;
}

int ScanOptions::depth() const
{
  return m_depth;
}

void ScanOptions::setDepth(int depth)
{
  m_depth = depth;
}

// ScanType
// ScanOptions::type() const
//{
//  return m_type;
//}

// void
// ScanOptions::setType(const ScanType& type)
//{
//  m_type = type;
//}

int ScanOptions::paperWidth() const
{
  return m_paper_width;
}

void ScanOptions::setPaperWidth(int paper_width)
{
  m_paper_width = paper_width;
}

int ScanOptions::paperHeight() const
{
  return m_paper_height;
}

void ScanOptions::setPaperHeight(int paper_height)
{
  m_paper_height = paper_height;
}

int ScanOptions::brightness() const
{
  return m_brightness;
}

void ScanOptions::setBrightness(int brightness)
{
  m_brightness = brightness;
}

int ScanOptions::contrast() const
{
  return m_contrast;
}

void ScanOptions::setContrast(int contrast)
{
  m_contrast = contrast;
}

int ScanOptions::pageDelay() const
{
  return m_page_delay;
}

void ScanOptions::setPageDelay(int page_delay)
{
  m_page_delay = page_delay;
}

int ScanOptions::topLeftX() const
{
  return m_geometry.x();
}

void ScanOptions::setTopLeftX(int top_left_x)
{
  m_geometry.setX(top_left_x);
}

int ScanOptions::topLeftY() const
{
  return m_geometry.y();
}

void ScanOptions::setTopLeftY(int top_left_y)
{
  m_geometry.setX(top_left_y);
}

int ScanOptions::bottomRightX() const
{
  return m_geometry.bottomRight().x();
}

void ScanOptions::setBottomRightX(int bottom_right_x)
{
  QPoint p = m_geometry.bottomRight();
  p.setX(bottom_right_x);
  m_geometry.setBottomRight(p);
}

int ScanOptions::bottomRightY() const
{
  return m_geometry.bottomRight().y();
}

void ScanOptions::setBottomRightY(int bottom_right_y)
{
  QPoint p = m_geometry.bottomRight();
  p.setY(bottom_right_y);
  m_geometry.setBottomRight(p);
}

QRect ScanOptions::geometry()
{
  return m_geometry;
}

void ScanOptions::setGeometry(QRect geometry)
{
  m_geometry = geometry;
}

int ScanOptions::resolutionX() const
{
  return m_resolution_x;
}

void ScanOptions::setResolutionX(int scan_resolution_x)
{
  m_resolution_x = scan_resolution_x;
}

int ScanOptions::resolutionY() const
{
  return m_resolution_y;
}

void ScanOptions::setResolutionY(int scan_resolution_y)
{
  m_resolution_y = scan_resolution_y;
}

int ScanOptions::resolution() const
{
  return m_resolution;
}

void ScanOptions::setResolution(int scan_resolution)
{
  m_resolution = scan_resolution;
  setResolutionX(scan_resolution);
  setResolutionY(scan_resolution);
}

bool ScanOptions::isResolutionRange()
{
  if (m_resolution_range.type() == QMetaType::QVariantList) {
    return false;
  }

  return true;
}

void ScanOptions::setResulutionRange(QVariant v)
{
  m_resolution_range = v;
}

QVariant ScanOptions::resolutionRange()
{
  return m_resolution_range;
}

int ScanOptions::optionId(const QString& name) const
{
  return m_option_id_map.value(name, -1);
}

void ScanOptions::setOptionId(const QString& name, int option_id)
{
  m_option_id_map.insert(name, option_id);
}

int ScanOptions::optionSize(const QString& name) const
{
  return m_option_size_map.value(name);
}

void ScanOptions::setOptionSize(const QString& name, int size)
{
  m_option_size_map.insert(name, size);
}

int ScanOptions::optionType(const QString& name) const
{
  return m_option_type_map.value(name);
}

void ScanOptions::setOptionType(const QString& name, int type)
{
  m_option_type_map.insert(name, type);
}

// int
// ScanOptions::getIntScannerValue(const ScanDevice* device,
//                                const int option_id,
//                                const SANE_Option_Descriptor* opt)
//{
//  if (option_id >= 0) {
//    SANE_Word value;
//    SANE_Handle sane_handle;
//    SANE_Status status;
//    status = sane_open(device->name.toStdString().c_str(), &sane_handle);

//    sane_control_option(
//      sane_handle, option_id, SANE_ACTION_GET_VALUE, &value, nullptr);

//    if (status == SANE_STATUS_GOOD) {

//      switch (opt->type) {
//        case SANE_TYPE_INT:
//          return value;

//        case SANE_TYPE_FIXED:
//          return SANE_Int(SANE_UNFIX(value));

//        default:
//          return -1;
//      }
//    }
//    sane_close(sane_handle);
//  }

//  return -1;
//}

// QStringList
// ScanOptions::getStringScannerValue(const ScanDevice* device,
//                                   const int option_id,
//                                   const SANE_Option_Descriptor* opt)
//{
//  QStringList list;

//  if (option_id >= 0) {
//    SANE_Word value;
//    SANE_Handle sane_handle;
//    SANE_Status status;
//    status = sane_open(device->name.toStdString().c_str(), &sane_handle);

//    status = sane_control_option(
//      sane_handle, option_id, SANE_ACTION_GET_VALUE, &value, nullptr);

//    if (status == SANE_STATUS_GOOD) {
//      switch (opt->type) {
//        case SANE_TYPE_STRING: {
//          QString s;

//          switch (opt->constraint_type) {
//            case SANE_CONSTRAINT_STRING_LIST:
//              for (int i = 0; opt->constraint.string_list[i] != nullptr; i++)
//              {
//                s = QString(opt->constraint.string_list[i]);
//                list.append(s);
//              }

//              break;

//            case SANE_CONSTRAINT_WORD_LIST:
//              for (int i = 1; i <= opt->constraint.word_list[0]; i++) {
//                s = QString(opt->constraint.word_list[i]);
//                list.append(s);
//              }

//              break;

//            default:
//              break;
//          }

//          return list;
//        }

//        default:
//          break;
//      }
//    }

//    sane_close(sane_handle);

//    return list;
//  }

//  return list;
//}
