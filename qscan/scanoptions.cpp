#include "scanoptions.h"

#include "scaninterface.h"

ScanOptions::ScanOptions(QObject* parent)
  : QObject(parent)
{}

ScanOptions::~ScanOptions() {}

int
ScanOptions::dpi() const
{
  return m_dpi;
}

void
ScanOptions::setDpi(int dpi)
{
  m_dpi = dpi;
}

ScanMode
ScanOptions::scanMode() const
{
  return m_mode;
}

void
ScanOptions::setScanMode(const ScanMode& mode)
{
  m_mode = mode;
}

int
ScanOptions::depth() const
{
  return m_depth;
}

void
ScanOptions::setDepth(int depth)
{
  m_depth = depth;
}

ScanType
ScanOptions::type() const
{
  return m_type;
}

void
ScanOptions::setType(const ScanType& type)
{
  m_type = type;
}

int
ScanOptions::paperWidth() const
{
  return m_paper_width;
}

void
ScanOptions::setPaperWidth(int paper_width)
{
  m_paper_width = paper_width;
}

int
ScanOptions::paperHeight() const
{
  return m_paper_height;
}

void
ScanOptions::setPaperHeight(int paper_height)
{
  m_paper_height = paper_height;
}

int
ScanOptions::brightness() const
{
  return m_brightness;
}

void
ScanOptions::setBrightness(int brightness)
{
  m_brightness = brightness;
}

int
ScanOptions::contrast() const
{
  return m_contrast;
}

void
ScanOptions::setContrast(int contrast)
{
  m_contrast = contrast;
}

int
ScanOptions::pageDelay() const
{
  return m_page_delay;
}

void
ScanOptions::setPageDelay(int page_delay)
{
  m_page_delay = page_delay;
}

int
ScanOptions::topLeftX() const
{
  return m_geometry.x();
}

void
ScanOptions::setTopLeftX(int top_left_x)
{
  m_geometry.setX(top_left_x);
}

int
ScanOptions::topLeftY() const
{
  return m_geometry.y();
}

void
ScanOptions::setTopLeftY(int top_left_y)
{
  m_geometry.setX(top_left_y);
}

int
ScanOptions::bottomRightX() const
{
  return m_geometry.bottomRight().x();
}

void
ScanOptions::setBottomRightX(int bottom_right_x)
{
  QPoint p = m_geometry.bottomRight();
  p.setX(bottom_right_x);
  m_geometry.setBottomRight(p);
}

int
ScanOptions::bottomRightY() const
{
  return m_geometry.bottomRight().y();
}

void
ScanOptions::setBottomRightY(int bottom_right_y)
{
  QPoint p = m_geometry.bottomRight();
  p.setY(bottom_right_y);
  m_geometry.setBottomRight(p);
}

QRect
ScanOptions::geometry()
{
  return m_geometry;
}

void
ScanOptions::setGeometry(QRect geometry)
{
  m_geometry = geometry;
}

int
ScanOptions::scanResolutionX() const
{
  return m_scan_resolution_x;
}

void
ScanOptions::setScanResolutionX(int scan_resolution_x)
{
  m_scan_resolution_x = scan_resolution_x;
}

int
ScanOptions::scanResolutionY() const
{
  return m_scan_resolution_y;
}

void
ScanOptions::setScanResolutionY(int scan_resolution_y)
{
  m_scan_resolution_y = scan_resolution_y;
}

int
ScanOptions::scanResolution() const
{}

void
ScanOptions::setScanResolution(int scan_resolution)
{
  m_scan_resolution = scan_resolution;
  setScanResolutionX(scan_resolution);
  setScanResolutionY(scan_resolution);
}

QString
ScanOptions::name() const
{
  return m_name;
}

void
ScanOptions::setName(const QString& name)
{
  m_name = name;
}

QString
ScanOptions::description() const
{
  return m_description;
}

void
ScanOptions::setDescription(const QString& description)
{
  m_description = description;
}

QString
ScanOptions::title() const
{
  return m_title;
}

void
ScanOptions::setTitle(const QString& title)
{
  m_title = title;
}

int
ScanOptions::getScannerValue(Device device, int option_id)
{
  if (option_id >= 0) {
    SANE_Word value;
    const SANE_Option_Descriptor* opt;
    opt = sane_get_option_descriptor(device->sane_handle, option_id);
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
