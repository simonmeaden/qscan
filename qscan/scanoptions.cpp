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
{}

ScanOptions::ScanOptions(const ScanOptions& other)
{
  copyData(other);
}

ScanOptions::~ScanOptions() {}

void ScanOptions::copyData(const ScanOptions& other)
{
  m_dpi = other.m_dpi;
  m_modes = other.m_modes;
  m_sources = other.m_sources;
  m_depth = other.m_depth;
  m_type = other.m_type;
  m_paper_width = other.m_paper_width;
  m_paper_height = other.m_paper_height;
  m_brightness = other.m_brightness;
  m_contrast = other.m_contrast;
  m_page_delay = other.m_page_delay;
  m_geometry = other.m_geometry;
  m_scan_resolution = other.m_scan_resolution;
  m_scan_resolution_x = other.m_scan_resolution_x;
  m_scan_resolution_y = other.m_scan_resolution_y;
  m_option_pairs = other.m_option_pairs;
}

ScanOptions ScanOptions::operator =(const ScanOptions rhs)
{
  copyData(rhs);
  return *this;
}

int ScanOptions::dpi() const
{
  return m_dpi;
}

void ScanOptions::setDpi(int dpi)
{
  m_dpi = dpi;
}

QStringList ScanOptions::scanModes() const
{
  return m_modes;
}

void ScanOptions::setScanModes(const QStringList& modes)
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

ScanType ScanOptions::type() const
{
  return m_type;
}

void ScanOptions::setType(const ScanType& type)
{
  m_type = type;
}

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

int ScanOptions::scanResolutionX() const
{
  return m_scan_resolution_x;
}

void ScanOptions::setScanResolutionX(int scan_resolution_x)
{
  m_scan_resolution_x = scan_resolution_x;
}

int ScanOptions::scanResolutionY() const
{
  return m_scan_resolution_y;
}

void ScanOptions::setScanResolutionY(int scan_resolution_y)
{
  m_scan_resolution_y = scan_resolution_y;
}

int ScanOptions::scanResolution() const
{
  //TODO
}

void ScanOptions::setScanResolution(int scan_resolution)
{
  m_scan_resolution = scan_resolution;
  setScanResolutionX(scan_resolution);
  setScanResolutionY(scan_resolution);
}

int ScanOptions::optionId(QString name) const
{
  return m_option_pairs.value(name, -1);
}

void ScanOptions::setOptionId(const QString& name, int option_id)
{
  m_option_pairs.insert(name, option_id);
}

int ScanOptions::getIntScannerValue(const ScanDevice* device, const int option_id, const SANE_Option_Descriptor* opt)
{
  if (option_id >= 0) {
    SANE_Word value;
    sane_control_option(device->sane_handle, option_id, SANE_ACTION_GET_VALUE, &value, nullptr);

    switch (opt->type) {
    case SANE_TYPE_INT:
      return value;

    case SANE_TYPE_FIXED:
      return SANE_Int(SANE_UNFIX(value));

    default:
      return -1;
    }
  }

  return -1;
}

QStringList ScanOptions::getStringScannerValue(const ScanDevice* device, const int option_id,
    const SANE_Option_Descriptor* opt)
{
  QStringList list;

  if (option_id >= 0) {
    SANE_Word value;
    SANE_Status status = sane_control_option(device->sane_handle, option_id, SANE_ACTION_GET_VALUE, &value, nullptr);

    if (status == SANE_STATUS_GOOD) {
      switch (opt->type) {
      case SANE_TYPE_STRING: {
        QString s;

        switch (opt->constraint_type) {
        case SANE_CONSTRAINT_STRING_LIST:
          for (int i = 0; opt->constraint.string_list[i] != nullptr; i++) {
            s = QString(opt->constraint.string_list[i]);
            list.append(s);
          }

          break;

        case SANE_CONSTRAINT_WORD_LIST:
          for (int i = 1; i <= opt->constraint.word_list[0]; i++) {
            s = QString(opt->constraint.word_list[i]);
            list.append(s);
          }

          break;

        default:
          break;
        }

        return list;
      }

      default:
        break;
      }
    }

    return list;
  }

  return list;
}
