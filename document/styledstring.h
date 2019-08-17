#ifndef STYLEINFO_H
#define STYLEINFO_H

#include <QObject>
#include <QSharedPointer>
#include <QBitArray>
#include <QMap>
#include <QList>

#include <qyaml-cpp/QYamlCpp>
#include <yaml-cpp/yaml.h>

#include "logging.h"
#include "docutil.h"

/*!
   \class StyledString
   \brief StyledString is a class that defines a string that holds certain basic styles.

   A StyledString comprises a string plus a set of StyleData objects which define
   certain style data for the string.

*/

/*!
   \class StyleData
   \brief StyleData defines data about a style within a StyledString.

   StyleData holds certain style information about s string.
   Styles are defined using a type, start and length information and are attached
   to a StyledString using the appendStyle() method or other similar methods. There
   are three sections defining the size of a block start, length and end. Start
   points to the first character of the StyleData block, length defines the length of
   the block and end points to the last character of the block. These can be set using
   either the StyleData(Type, int start, int length) or the setStart(), setLength() and
   setEnd() methods although you should note that setting the end point using setEnd()
   will alter the length as well as \em end \em is basically \em start + length - 1 \em
   so use one or the other not both.

   The types, defined using the StypeData::Type enumeration include such things as
   Bold, Italic and Font sizes.

   The Paragraph style can be used, along with the splitParagraphs() method to
   split a StyledString into multiple separate StyledString's.

   \see Type
*/

class StyledString;

namespace DocUtil {
StyledString cleanText(const StyledString& text);
}

class StyleData;
/*!

*/
using Style = QSharedPointer<StyleData>;
using StyleList = QList<Style>;
using StyledStringList = QList<StyledString>;

class StyleData
{
public:

  /*!
     \enum Type

     Defines certain types of styling. All types with the exception of Paragraph
     can be overlain. For example you could define string "A test string" for example
     and set the whole thing to Italic and set the test section to Bold. The result
     would be \em A \b test \b string \em .

     \code
     StyledString s(tr("A test string"));
     s.appendStyle(StyleData::getStyle(StyleData::Italic, 0, s.length());
     s.appendStyle(StyleData::getStyle(StyleData::Bold, 2, 4);
     \endcode

     Paragraphs are strictly speaking not styles at all, they are used to split a
     StyledString into list of StyledStrings, splitting the styles appropriately.
     To do this define a paragraph, adding to the previous example:

     \code
     s.appendStyle(StyleData::getStyle(StyleData::Paragraph, 2, 4);
     \endcode

     Utilising the StyledString splitParagraphs() would result in three paragraphs
     \li \em A \em - in italics
     \li \b \em test \em \b - in bold italics
     \li \em string \em
     Empty strings or strings comprising only whitespace, tabs, space, newline etc.
     are thrown away.

     If you try to appendStyle() a paragraph, overlaying all or part of an existing
     paragraph it will return false and ignore the style.

  */
  enum Type
  {
    NoType = 0, /**< enum value NoType. Primarily used to hit the default in a switch.*/
    Normal, /**< enum value Normal - just normal unstyled text. */
    Bold, /**< enum value Bold. Bold text. */
    Italic, /**< enum value Italic. Italic text */
    Underline, /**< enum value Underline. Underlined text */
    Subscript, /**< enum value Subscript. Subscripted text */
    Superscript, /**< enum value Superscript. Superscripted text */
    Font_X_X_Small, /**< enum value Font size. By default 33% normal. */
    Font_X_Small, /**< enum value Font size. By default 45% normal. */
    Font_Small, /**< enum value Font size. By default 58% normal. */
    Font_Medium, /**< enum value Font size. By default 83% normal. */
    Font_Normal, /**< enum value Font size. By default 100% normal. */
    Font_Large, /**< enum value Font size. By default 113% normal. */
    Font_V_Large, /**< enum value Font size. By default 129% normal. */
    Font_X_Large, /**< enum value Font size. By default 148% normal. */
    Font_X_X_Large, /**< enum value Font size. By default 200% normal. */
    Paragraph = 100, /**< enum value Paragraph. Defines a separate paragraph block*/
  };

  /*!
     \brief Constructs a null StyleData. The type is set to NoType, start and length to -1.
  */
  StyleData();
  /*!
     \brief Constructs a typed but empty StyleData. The type is set to type, start and length to 0.
  */
  StyleData(Type type);
  /*!
     \brief Constructs a StyleData. The type is set to type, start is set to start and length to length.
  */
  StyleData(Type type, int start, int length);

  /*!
     \brief Get the start value.

     \see length()
  */
  int start() const;
  /*!
     \brief Set the start value to start.

     \see setLength()
  */
  void setStart(int start);

  /*!
     \brief Get the length value.

     \see start()
     \see end()
  */
  int length() const;
  /*!
     \brief Set the length value to length.

     \see length()
     \see start()
     \see end()
  */
  void setLength(int length);

  /*!
     \brief Get the end value.

     \see start()
     \see length()
  */
  int end() const;
  /*!
     \brief set the end point of the StyledString.

     \see setStart()
     \see setLength()
  */
  void setEnd(int end);

  /*!
     \brief Get the StyleData Type
  */
  Type type() const;
  /*!
     \brief Set the StyleData Type
  */
  void setType(const Type& type);

  /*!
     \brief returns true if the StyleData is not empty and is a valid block.
  */
  bool isValid();

  /*!
     \brief Returns the font_stretch value for the appropriate style.

     Font sizes in a webpage and Qt WebEngine are defined as percentages of the
     default size for the webpage. Hence this returns values ± 100% and by
     default range from 33% to 200%. There are 9 value steps and they can be
     reset using the setFontStretch() method.

     \see setFontStretch()

  */
  int fontStretch();
  /*!
     \brief Sets one of the nine font stretch range values.

     \see fontStretch()
  */
  void setFontStretch(int index, int value);


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
  static Style getStyle(Type type = NoType, int start = -1, int length = -1);

  /*!
     \brief This function overloads operator==().

     Returns true if this StyleData is equal to other; otherwise returns false.
  */
  bool operator==(const StyleData&);

private:
  QList<int> font_sizes = {33, 45, 58, 83, 100, 113, 129, 148, 200};

  int m_start, m_length;
  Type m_type;

};

class StyledString /*: public QString*/
{
public:
  StyledString();
  StyledString(const char* string);
  StyledString(QString string);
  StyledString(const StyledString& string);

  QString text() const;
  int length();
  int size();
  bool isEmpty();
  bool isEmptyOrWhitespace();
  StyledString mid(int position, int n = -1);
  StyledString left(int n);
  StyledString right(int n);

  bool appendStyle(Style style);
  void appendStyles(QList<Style> styles);
  bool removeStyle(Style style);
  void removeStyle(int index);
  void insertStyle(int index, Style style);
  int indexOf(const Style& style);
  void move(int from, int to);
  bool contains(Style style);

  StyledStringList splitParagraphs();


  void setText(const QString& text);

  QList<Style> styles() const;
  void setStyles(const QList<Style>& styles);

  StyledString& operator=(const QString&);
  StyledString& operator=(const StyledString&);
  bool operator==(const StyledString&);
  bool operator<(const StyledString&);
  bool operator<=(const StyledString&);
  bool operator>(const StyledString&);
  bool operator>=(const StyledString&);
  StyledString& operator+(const StyledString&);
  StyledString& operator+=(const StyledString&);
  friend const StyledString operator+(const StyledString&, const StyledString&);

protected:
  //! The actual string.
  /*!
    Stores the string as a QString object..
  */
  QString m_text;
  //! A list of Style objects.
  /*!
    Style objects are QSharedPointer's to StyleData objects. They hold
    individual style data.
  */
  QList<Style> m_styles;

  //  void transferStyles(StyledString& data_str, int p_start, int p_end, QMap<int, Style> others);

};

namespace YAML {

template<>
struct convert<StyledString>
{
  static Node encode(const StyledString& rhs) {
    Node node;
    node = rhs.text().toStdString();
    return node;
  }

  static bool decode(const Node& node, StyledString& rhs) {
    if (!node.IsScalar()) {
      return false;
    }

    std::string sstr = node.as<std::string>();
    rhs = StyledString(sstr.c_str());

    return true;
  }
};

Emitter& operator<<(Emitter& emitter, StyledString& v);
Emitter& operator<<(Emitter& emitter, const StyledString& v);

void operator>>(const Node& node, QString& q);
void operator<<(Node& node, const QString& q);

}

Q_DECLARE_METATYPE(StyleData)
Q_DECLARE_METATYPE(Style)
Q_DECLARE_METATYPE(StyledString)

#endif // STYLEINFO_H
