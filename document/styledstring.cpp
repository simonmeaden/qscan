#include "styledstring.h"


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

namespace DocUtil {

/*!
  \brief Cleans text of dodgy characters and un-escaped single and double quotes..

  Some characters such as em-dash which are detected by trhe scanner
  do not convert well in YAML so convert them to simpler forms such a hyphen.

  Double quotes, either standard double quotes (") or their unicode
  brethren, left and right double quotes, are converted into escaped
  standard double quotes.
*/
StyledString cleanText(const StyledString& styled_str)
{
  QString str = DocUtil::cleanText(styled_str.text());

  StyledString text(str);
  text.setStyles(styled_str.styles());

  return text;
}

}

/* StyleData
 *****************************************************************************/
/*!
   \brief Constructs a null StyleData. The type is set to NoType, start and length to -1.
*/
StyleData::StyleData()
  : m_start(-1)
  , m_length(-1)
  , m_type(NoType)
{}

/*!
   \brief Constructs a typed but empty StyleData. The type is set to type, start and length to 0.
*/
StyleData::StyleData(StyleData::Type type)
  : m_start(0)
  , m_length(0)
  , m_type(type)
{

}

/*!
   \brief Constructs a StyleData. The type is set to type, start is set to start and length to length.
*/
StyleData::StyleData(Type type, int start, int length)
  : m_start(start)
  , m_length(length)
  , m_type(type)
{}

/*!
   \brief Get the start position of the style.

   \see length()
*/
int StyleData::start() const
{
  return m_start;
}

/*!
   \brief Set the start position of the style to start.

   \see setLength()
*/
void StyleData::setStart(int start)
{
  m_start = start;
}

/*!
   \brief Get the length of the style.

   \see start()
   \see end()
*/
int StyleData::length() const
{
  return m_length;
}

/*!
   \brief Set the length of the style to length.

   \see length()
   \see start()
   \see end()
*/
void StyleData::setLength(int length)
{
  m_length = length;
}

/*!
   \brief Get the end position of the style.

   \see start()
   \see length()
*/
int StyleData::end() const
{
  return m_start + m_length - 1;
}

/*!
   \brief set the end point of the style.

   \see setStart()
   \see setLength()
*/
void StyleData::setEnd(int end)
{
  m_length = end - m_start + 1;
}

/*!
   \brief Get the StyleData Type of the style.
*/
StyleData::Type StyleData::type() const
{
  return m_type;
}

/*!
   \brief Set the StyleData Type of the style.
*/
void StyleData::setType(const StyleData::Type& style)
{
  m_type = style;
}

/*!
   \brief returns true if the StyleData is not empty and is a valid block.
*/
bool StyleData::isValid()
{
  return (m_start >= 0 && m_length > 0 && m_type != NoType);
}

/*!
   \brief Returns the font_stretch value for the appropriate style.

   Font sizes in a webpage and Qt WebEngine are defined as percentages of the
   default size for the webpage. Hence this returns values ± 100 and by
   default range from 33% to 200%. There are 9 value steps and they can be
   reset using the setFontStretch() method.

*/
int StyleData::fontStretch()
{
  switch (type()) {
  case StyleData::Font_X_X_Small:
    return font_sizes[0];

  case StyleData::Font_X_Small:
    return font_sizes[1];

  case StyleData::Font_Small:
    return font_sizes[2];

  case StyleData::Font_Medium:
    return font_sizes[3];

  case StyleData::Font_Normal:
    return font_sizes[4];

  case StyleData::Font_Large:
    return font_sizes[5];

  case StyleData::Font_V_Large:
    return font_sizes[6];

  case StyleData::Font_X_Large:
    return font_sizes[7];

  case StyleData::Font_X_X_Large:
    return font_sizes[8];

  default:
    return -1;
  }
}

/*!
   \brief Sets one of the nine font stretch range values.

   \see fontStretch()
*/
void StyleData::setFontStretch(int index, int value)
{
  if (index >= 0 && index < 9) {
    font_sizes[index] = value;
  }
}


/*!
   \brief A static method of StyleData that returns a QSharedPointer<StyleData> object.

   The method takes three parameters, all of which have null default values so you
   can produce an empty invalid Style which can have it's values reset later, a
   valid but empty Style if you merely uae the Type parameter or a fully functional
   Style by setting all three parameters.
   Using this method saves you having to write
   \code
   Style style(new StyleData(StyleData::Bold, 16, 8));
   \endcode
   instead you can write
   \code
   Style style = StyleData::getStyle(StyleData::Bold, 16, 8);
   \endcode
   Not certain if that is really an improvement but I prefer it.
*/
Style StyleData::getStyle(StyleData::Type type, int start, int length)
{
  if (type == NoType) {
    return Style(new StyleData());
  }

  if (start < 0 || length < 0) {
    return  Style(new StyleData(type));
  }

  return Style(new StyleData(type, start, length));
}

/*!
   \brief This function overloads operator==().

   Returns true if this StyleData is equal to other; otherwise returns false.
*/
bool StyleData::operator==(const StyleData& other)
{
  if (m_type == other.type() &&
      m_start == other.m_start &&
      m_length == other.length()) {
    return true;
  }

  return false;
}

/* StyledString
 *****************************************************************************/

/*!
   \brief Constructs a null string. Null strings are also empty and contain to styles.
*/
StyledString::StyledString()
{

}

/*!
   \brief Constructs a string initialized with the 8-bit string string. The given const char pointer
   is converted to Unicode using the fromUtf8() function.
*/
StyledString::StyledString(const char* string)
  : m_text(string)
{

}

/*!
   \brief Constructs a string initialized with the QString object.
*/
StyledString::StyledString(QString string)
  : m_text(string)
{

}

/*!
   \brief Constructs a string initialized with another StyledString object.
*/
StyledString::StyledString(const StyledString& string)
  : m_text(string.m_text)
  , m_styles(string.styles())
{

}

/*!
   \brief Gets the text component of the StyledString as a QString.

   \see setText()
*/
QString StyledString::text() const
{
  return m_text;
}

/*!
   \brief Returns the number of characters in this string as an int. Equivalent to size().
*/
int StyledString::length()
{
  return m_text.length();
}

/*!
   \brief Returns the number of characters in this string as an int. Equivalent to length().
*/
int StyledString::size()
{
  return length();
}

/*!
   \brief Returns true if the string has no characters; otherwise returns false.
*/
bool StyledString::isEmpty()
{
  return m_text.isEmpty();
}

/*!
   \brief Returns true if the string has no characters; otherwise returns false.
*/
bool StyledString::isEmptyOrWhitespace()
{
  QString text = m_text.trimmed();

  if (text.isEmpty()) {
    return true;
  }

  return false;
}

/*!
   \brief Returns a string that contains n characters of this string,
          starting at the specified position index.

   Returns a null string if the position index exceeds the length of the string.
   If there are less than n characters available in the string starting at
   the given position, or if n is -1 (default), the function returns all
   characters that are available from the specified position.

   All styles added to the parent string will be passed to the substring if they
   apply to the selected characters.

   \code
   StyledString s = "A basic string";
   s.appendStyle(StyledString::getStyle(StyleData::Italic, 2, 5);
   StyledString sub = s.mid(2, 5);
   \endcode
   sub == \em basic \em .

   \see right()
   \see left()
*/
StyledString StyledString::mid(int position, int n)
{
  if (position > length()) {
    return StyledString();

  } else if (position + n > length()) {
    n = length() - position;
  }

  if (n < 0) {
    int length_rqd = m_text.length() - position - 1;
    StyledString str = right(length_rqd);
    return str;

  } else if (position == 0) {
    return left(n);

  }  else {
    int left_len = m_text.length() - position;
    StyledString str = right(left_len);
    str = str.left(n);
    return str;
  }
}

/*!
   \brief Returns a substring that contains the n leftmost characters of the string.

   The entire string is returned if n is greater than or equal to size(), or
   less than zero.

   All styles added to the parent string will be passed to the substring if they
   apply to the selected characters.

   \code
   StyledString s = "A basic string";
   s.appendStyle(StyledString::getStyle(StyleData::Italic, 2, 5);
   StyledString sub = s.left(7); // sub == A \em basic \em
   \endcode

   \see right()
   \see mid()
*/
StyledString StyledString::left(int n)
{
  if (n > length() || n < 0) {
    return *this;
  }

  StyledString str = m_text.left(n);

  for (Style style : m_styles) {
    Style s = StyleData::getStyle(style->type());

    if (style->start() < n) {
      s->setStart(style->start());

      if (style->end() <= n - 1) {
        s->setLength(style->length());

      } else {
        s->setLength(n - style->start());
      }

      if (s->length() > 0) {
        str.appendStyle(s);
      }
    }
  }

  return str;
}

/*!
   \brief Returns a substring that contains the n rightmost characters of the string.

   The entire string is returned if n is greater than or equal to size(), or less than zero.

   All styles added to the parent string will be passed to the substring if they
   apply to the selected characters.

   \code
   StyledString s = "A basic string";
   s.appendStyle(StyledString::getStyle(StyleData::Italic, 8, 6);
   StyledString sub = s.right(6); // sub == \a string \a
   \endcode

   \see left()
   \see mid()
*/
StyledString StyledString::right(int n)
{
  if (n > length() || n < 0) {
    return *this;
  }

  StyledString styled_str = m_text.right(n);
  int start, end;

  if (n == length()) {
    start = 0;
    end = length() - 1;
    styled_str.appendStyles(styles());

  } else {
    start = m_text.length() - n;
    end = m_text.length() - 1;

    for (Style style : m_styles) {
      Style s = StyleData::getStyle(style->type());

      if (style->end() < start || style->start() > end) {
        // totally outside
        continue;

      } else {
        if (style->start() == start) {
          s->setStart(0);

          if (style->end() > end) {
            s->setLength(end - start);

          } else {
            s->setLength(style->length());
          }

        } else if (style->start() < start) {
          s->setStart(0);
          s->setLength(style->length() - (start - style->start()));

        } else if (style->start() > start) {
          s->setStart(style->start() - start);
          s->setLength(style->length());
        }

        styled_str.appendStyle(s);
      }
    }
  }

  return styled_str;
}

/*!
   \brief Adds a new style Style object to the StyledString.

   Note: Styles that are added must be inside the string size.
   Styles that are outside the string size are trimmed to the
   size of the string.

   \see appendStyles()
   \see removeStyle()
   \see insertStyle()
*/
bool StyledString::appendStyle(Style style)
{
  if (style->type() == StyleData::Paragraph) {
    // make certain paragraphs don't overlap
    for (Style p_style : m_styles) {
      if (p_style->type() != StyleData::Paragraph) {
        continue;
      }

      if (p_style->end() < style->start() ||
          p_style->start() > style->end()) {
        continue;

      } else {
        // there is an overlap.
        return false;
      }
    }

    m_styles.append(style);
    return true;

  } else {
    if (style->start() >= 0 &&
        style->start() < m_text.length() &&
        style->start() + style->length() <= m_text.length()) {
      m_styles.append(style);
      return true;

    } else {
      // prune if style larger than data.
      if (style->start() < 0) {
        style->setLength(style->length() - style->start());
        style->setStart(0);
      }

      if (style->length() > m_text.length()) {
        style->setLength(m_text.length());
      }

      m_styles.append(style);
      return true;
    }
  }
}

/*!
   \brief Adds a QList of Style objects to the string.

   \param styles a QList<Style> object.
   \see appendStyle(Style)
*/
void StyledString::appendStyles(QList<Style> styles)
{
  for (Style style : styles) {
    m_styles.append(style);
  }
}

/*!
   \brief Removes the first occurrence of style in the list and returns true on success; otherwise returns false.

   \param style - the style to remove.
   \see appendStyle()
   \see appendStyles()
   \see insertStyle()
*/
bool StyledString::removeStyle(Style style)
{
  return m_styles.removeOne(style);
}

/*!
   \brief Removes the style at index position i. i must be a valid index position
   in the list (i.e., 0 <= i < size()).

   \param index the int index.
   \see appendStyle()
   \see appendStyles()
   \see insertStyle()
*/
void StyledString::removeStyle(int index)
{
  m_styles.removeAt(index);
}

/*!
   \brief Inserts style at index position i in the list.
   If i <= 0, the style is prepended to the list.
   If i >= size(), the value is appended to the list.

   \param index
   \param style
   \see appendStyle()
   \see appendStyles()
   \see removeStyle()
*/
void StyledString::insertStyle(int index, Style style)
{
  m_styles.insert(index, style);
}

/*!
   \brief Returns the index position of the first occurrence of style in the list, searching
   forward from index position from. Returns -1 if no item matched.

   \param style
   \return the index of the style.
*/
int StyledString::indexOf(const Style& style)
{
  return m_styles.indexOf(style);
}

/*!
   \brief Moves the item at index position from to index position to.

   This function assumes that both from and to are at least 0 but less than size().
   To avoid failure, test that both from and to are at least 0 and less than size().

   \param from the source index
   \param to the destination index
*/
void StyledString::move(int from, int to)
{
  m_styles.move(from, to);
}

/*!
   \brief Returns true if the list contains an occurrence of style;
   otherwise returns false.

   \param style a Style object.
   \return true if the list contains the style type;
*/
bool StyledString::contains(Style style)
{
  return m_styles.contains(style);
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
StyledStringList StyledString::splitParagraphs()
{
  // TODO split string at paragraph boundaries.
  QMap<int, Style> paras;
  QList<Style>others;

  // split out the paragraphs from the non-paragraphs.
  for (auto data : m_styles) {
    if (data->type() == StyleData::Paragraph) {
      paras.insert(data->start(), data);

    } else {
      others.append(data);
    }
  }

  StyledString styled_str(m_text);
  styled_str.appendStyles(others);

  StyledStringList splits;

  if (paras.isEmpty() ||
      (paras.size() == 1 &&
       paras.first()->start() == 0 &&
       paras.first()->length() == length())) {
    /* Special case of a StyledString without a Paragraph style
      or a StyledString with a full length Paragraph style. */
    styled_str.appendStyle(
      StyleData::getStyle(
        StyleData::Paragraph,
        0,
        m_text.length()));
    splits.append(styled_str);
    return splits;

  } else {
    StyledString before, centre;
    int start = 0;

    for (Style para : paras) {
      int p_start = para->start();
      int p_len = para->length();
      int b_len = p_start - start;

      before = styled_str.mid(start, b_len);
      before.appendStyle(StyleData::getStyle(StyleData::Paragraph, start, b_len));

      if (!before.isEmptyOrWhitespace()) {
        splits.append(before);
      }

      centre = styled_str.mid(p_start, p_len);
      centre.appendStyle(StyleData::getStyle(StyleData::Paragraph, 0, p_len));

      if (!centre.isEmptyOrWhitespace()) {
        splits.append(centre);
      }

      start = para->end() + 1;
    }

    if (start < styled_str.length()) {
      StyledString after = styled_str.right(styled_str.length() - start);
      after.appendStyle(StyleData::getStyle(StyleData::Paragraph, 0, after.length()));

      if (!after.isEmptyOrWhitespace()) {
        splits.append(after);
      }
    }
  }

  return splits;
}

/*!
   \brief Sets the text value of the StyledString.

   \param text - the text to set.
   \see text()
*/
void StyledString::setText(const QString& text)
{
  m_text = text;
}

/*!
   \brief Gets the list of styles.

   \return  a QList<Style> object.
*/
QList<Style> StyledString::styles() const
{
  return m_styles;
}

/*!
   \brief Set the style list.

   \param styles a QList<Style> object.
*/
void StyledString::setStyles(const QList<Style>& styles)
{
  m_styles = styles;
}


/*!
   \brief Assigns other to this StyledString and returns a reference to this StyledString.
*/
StyledString& StyledString::operator=(const StyledString& other)
{
  this->setText(other.m_text);
  m_styles = other.styles();
  return *this;
}

/*!
   \brief Assigns other to this StyledString and returns a reference to
   this QString.

   The style list will be empty.
*/
StyledString& StyledString::operator=(const QString& other)
{
  this->setText(other);
  this->m_styles.clear();
  return *this;
}

/*!
   \brief This function overloads operator==().

   Returns true if this StyledString is equal to other; otherwise returns false.
*/
bool StyledString::operator==(const StyledString& other)
{
  if (m_text != other.m_text) {
    return false;
  }

  if (m_styles.size() == other.styles().size()) {
    for (int i = 0; i < m_styles.size(); i++) {
      Style style = m_styles.at(i);
      Style o_style = other.styles().at(i);

      if (style->type() != o_style->type() ||
          style->start() != o_style->start() ||
          style->length() != o_style->length()) {
        return false;
      }
    }

  } else {
    return false;
  }

  return true;
}

/*!
   \brief This function overloads operator<().

   Returns true if string s1 is lexically less than string s2,
   otherwise returns false.

   The comparison ONLY compares the string value and is equivalent
   to comparing the QString values recovered usint text().
*/
bool StyledString::operator<(const StyledString& other)
{
  return (m_text < other.m_text);
}

/*!
   \brief This function overloads operator<=().

   Returns true if string s1 is lexically less than or equal to string s2,
   otherwise returns false.

   The comparison ONLY compares the string value and is equivalent
   to comparing the QString values recovered usint text().
*/
bool StyledString::operator<=(const StyledString& other)
{
  return (m_text <= other.m_text);
}

/*!
   \brief This function overloads operator>().

   Returns true if this string is lexically greater than the parameter
   string other, otherwise returns false.


   The comparison ONLY compares the string value and is equivalent
   to comparing the QString values recovered usint text().

*/
bool StyledString::operator>(const StyledString& other)
{
  return (m_text > other.m_text);
}

/*!
   \brief This function overloads operator>=().

   Returns true if this string is lexically greater than
   or equal to the parameter
   string other, otherwise returns false.

   The comparison ONLY compares the string value and is equivalent
   to comparing the QString values recovered usint text().

*/
bool StyledString::operator>=(const StyledString& other)
{
  return (m_text >= other.m_text);
}

/*!
   \brief This function overloads operator+=().

   Appends the string section referenced by other to this string
   and the appends the style list from other.
*/
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

/*!
   \brief This function overloads operator+().

   Returns a string which is the result of concatenating this and other.

   Appends the string section referenced by other to this string
   and the appends the style list from other.

*/
StyledString& StyledString::operator+(const StyledString& other)
{
  m_text += other.m_text;
  m_styles.append(other.styles());
  return *this;
}

/*!
   \brief Returns a string which is the result of concatenating s1 and s2.
*/
const StyledString operator+(const StyledString& lhs, const StyledString& rhs)
{
  StyledString str = lhs;
  str += rhs;
  return str;
}

