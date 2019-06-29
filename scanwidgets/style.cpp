#include "style.h"

/* StyleInfo
 *****************************************************************************/
StyleData::StyleData()
  : m_start(-1)
  , m_length(-1)
  , m_style(Normal)
{}

StyleData::StyleData(Type style, int start, int end)
  : m_start(start)
  , m_length(end)
  , m_style(style)
{}

int StyleData::start() const
{
  return m_start;
}

void StyleData::setStart(int start)
{
  m_start = start;
}

int StyleData::length() const
{
  return m_length;
}

void StyleData::setLength(int end)
{
  m_length = end;
}

StyleData::Type StyleData::style() const
{
  return m_style;
}

void StyleData::setStyle(const StyleData::Type& style)
{
  m_style = style;
}

bool StyleData::isValid()
{
  return (m_start >= 0 && m_length >= 0 && m_style != Normal);
}

int StyleData::fontStretch(Type type)
{
  switch (type) {
  case StyleData::Font_X_X_Small:
    return 33;

  case StyleData::Font_X_Small:
    return 45;

  case StyleData::Font_Small:
    return (58);

  case StyleData::Font_Medium:
    return (83);

  case StyleData::Font_Normal:
    return (100);

  case StyleData::Font_Large:
    return (113);

  case StyleData::Font_V_Large:
    return (129);

  case StyleData::Font_X_Large:
    return (148);

  case StyleData::Font_X_X_Large:
    return (200);

  default:
    return -1;
  }
}

/* StyledString
 *****************************************************************************/
StyledString::StyledString()
{

}

StyledString::StyledString(const char* string) : QString(string)
{

}

StyledString::StyledString(QString string) : QString(string)
{

}

void StyledString::appendStyle(Style style)
{
  m_styles.append(style);
}

void StyledString::removeStyle(Style style)
{
  m_styles.removeOne(style);
}

void StyledString::removeStyle(int index)
{
  m_styles.removeAt(index);
}

void StyledString::insertStyle(int index, Style style)
{
  m_styles.insert(index, style);
}

int StyledString::indexOf(const Style& style)
{
  return m_styles.indexOf(style);
}

void StyledString::move(int from, int to)
{
  m_styles.move(from, to);
}

StyledString& StyledString::operator=(const StyledString& other)
{
  this->setText(other);
  m_styles = other.styles();
  return *this;
}

StyledString& StyledString::operator=(const QString& other)
{
  this->setText(other);
  this->m_styles.clear();
  return *this;
}

void StyledString::setText(const QString& text)
{
  replace(0, length(), text);
}

QList<Style> StyledString::styles() const
{
  return m_styles;
}

void StyledString::setStyles(const QList<Style>& styles)
{
  m_styles = styles;
}
