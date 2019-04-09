#include "scanoptions.h"

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
ScanOptions::mode() const
{
  return m_mode;
}

void
ScanOptions::setMode(const ScanMode& mode)
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
