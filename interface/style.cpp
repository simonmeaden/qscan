#include "style.h"

namespace YAML {
/*= Adds StyledString capability to QYAML-CPP and YAML-CPP
   =========================================================================================*/
/*!
    \brief operator >> for QString
*/
void operator>>(const Node& node, QString& q)
{
  std::string sstr;
  sstr = node.as<std::string>();
  q = QString(sstr.c_str());
}

/*!
    \brief operator << for QString
*/
void operator<<(Node& node, const QString& q)
{
  std::string sstr = q.toStdString();
  node = sstr;
}
/*!
    \brief Emitter operator << overload for StyledString
*/
Emitter& operator<<(Emitter& emitter, StyledString& v)
{
  return emitter.Write(v.text().toStdString());
}

/*!
    \brief Emitter operator << overload for const \\\\StyledString
*/
Emitter& operator<<(Emitter& emitter, const StyledString& v)
{
  return emitter.Write(v.text().toStdString());
}

} // end of namespace YAML

namespace Util {

/*!
  \brief Cleans text of dodgy characters and un-escaped single and double quotes..

  Some characters such as em-dash which are detected by trhe scanner
  do not convert well in YAML so convert them to simpler forms such a hyphen.

  Double quotes, either standard double quotes (") or their unicode
  brethren, left and right double quotes, are converted into escaped
  standard double quotes.

  \param text - the string to clean.
  \return - the 'cleaned' text.
*/
StyledString cleanText(const StyledString& styled_str)
{
  QString str = Util::cleanText(styled_str.text());

  StyledString text(str);
  text.setStyles(styled_str.styles());

  return text;
}

}

/* StyleInfo
 *****************************************************************************/
StyleData::StyleData()
  : m_start(-1)
  , m_length(-1)
  , m_style(Normal)
{}

StyleData::StyleData(Type style, int start, int length)
  : m_start(start)
  , m_length(length)
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

int StyleData::end() const
{
  return m_start + m_length - 1;
}

StyleData::Type StyleData::type() const
{
  return m_style;
}

void StyleData::setType(const StyleData::Type& style)
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

StyledString::StyledString(const char* string)
  : m_text(string)
{

}

StyledString::StyledString(QString string)
  : m_text(string)
{

}

StyledString::StyledString(const StyledString& string)
  : m_text(string.m_text)
  , m_styles(string.styles())
{

}

QString StyledString::text() const
{
  return m_text;
}

int StyledString::length()
{
  return m_text.length();
}

bool StyledString::isEmpty()
{
  return m_text.isEmpty();
}

StyledString StyledString::left(int n)
{
  StyledString str = m_text.left(n);

  for (Style style : m_styles) {
    if (style->start() < str.length()) {
      style->setLength(str.length() - style->start());
      str.appendStyle(style);
    }
  }

  return str;
}

StyledString StyledString::right(int n)
{
  StyledString str = m_text.right(n);
  int start = length() - str.length() - 1;
  int end = m_text.length() - 1;

  for (Style style : m_styles) {
    bool is_start_in = style->start() >= start && style->start() <= end ;
    bool is_end_in = style->end() >= start && style->end() <= end;

    if (is_start_in && is_end_in) {
      style->setStart(style->start() - start);
      str.appendStyle(style);

    } else if (is_end_in) {
      style->setLength(style->length() - (start - style->start() + 1));
      style->setStart(0);
      str.appendStyle(style);
    }
  }

  return str;
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

/*!
   \brief Splits a styled string into paragraphs.

   Takes the single StyledString and splits it about specified
   paragraph boundaries. If no paragraphs are defined then the
   string is returned unchanged. Sections before, between and after
   specified paragraphs are returned as paragraphs if they contain
   text, otherwise they are deleted.

   \return a QList of StyledString elements.
*/
QList<StyledString> StyledString::splitParagraphs()
{
  // TODO split string at paragraph boundaries.
  QMap<int, Style> paras;
  QMap<int, Style> others;

  // split out the paragraphs from the non-paragraphs.
  for (auto data : m_styles) {
    if (data->type() == StyleData::Paragraph) {
      paras.insert(data->start(), data);

    } else {
      others.insert(data->start(), data);
    }
  }

  StyledString centre = *this;
  int end = centre.length();
  QList<StyledString> splits;
  int p_start = -1, p_end = -1, p_length = -1, start = 0;

  for (auto para : paras) {
    if (para->start() == 0 && para->length() == this->length()) {
      // a whole string paragraph. no further work required.
      splits.append(centre);
      return splits;
    }

    StyledString left, right;
    p_start = para->start();
    p_length = para->length();
    p_end = para->end();

    left = centre.m_text.left(p_start);
    right = centre.m_text.mid(p_end + 1);
    centre = centre.m_text.mid(p_start, p_length);

    if (!left.isEmpty()) {
      transferStyles(left, start, p_start - 1, others);
      left.appendStyle(Style(new StyleData(StyleData::Paragraph, start, left.length())));
      splits.append(left);
    }

    if (!centre.isEmpty()) {
      transferStyles(centre, p_start, p_end, others);
      centre.appendStyle(para);
      splits.append(centre);
    }

    centre = right;
    start = p_end;
  }

  if (!centre.isEmpty()) {
    // if p_start is -1 then no paras were defined so just add a paragraph style
    // with no style transfer as styles are already included.
    if (p_start > -1) {
      // what was left at the end.
      transferStyles(centre, start, end, others);
    }

    centre.appendStyle(Style(new StyleData(StyleData::Paragraph, 0, centre.length())));
    splits.append(centre);
  }

  return splits;
}

void StyledString::transferStyles(StyledString& data_str, int p_start, int p_end, QMap<int, Style> others)
{
  for (auto style : others) {
    int s_start = style->start();
    int s_end = style->end();
    int s_length = style->length();

    if (s_end < p_start || s_start >= p_end) {// A & E
      //  style before or after para - ignore
      continue;

    } else {
      if (s_start <= p_start) {// B & F
        // style start is before para start - just shift to offset from para start
        s_start = 0;

        if (s_end <= p_end) { // B
          // style end is before para end - just shift to offset from para start
          s_length = s_end - p_start;

        } else { // F
          // style end is after para end - truncate
          s_length = p_end - p_start + 1;
        }

      } else {
        if (s_end <= p_end) { //C
          // style is entirely internal to para - just shift to offset from para start
          s_start -= p_start;

        } else { /*if (s_end >= p_end)*/  //D
          s_length = p_end - s_start + 1;
        }
      }
    }

    Style s(new StyleData(style->type(),
                          s_start,
                          s_length));
    data_str.appendStyle(s);
  }
}

StyledString& StyledString::operator=(const StyledString& other)
{
  this->setText(other.m_text);
  m_styles = other.styles();
  return *this;
}

StyledString& StyledString::operator+=(const StyledString& other)
{
  QString str = m_text + other.m_text;
  int len = m_text.length();
  QList<Style> style_list = styles();

  for (Style s : other.styles()) {
    // shift start of other styles.
    s->setStart(s->start() + len);
    style_list.append(s);
  }

  m_text = str;
  m_styles = style_list;
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
  m_text = text;
}

QList<Style> StyledString::styles() const
{
  return m_styles;
}

void StyledString::setStyles(const QList<Style>& styles)
{
  m_styles = styles;
}
