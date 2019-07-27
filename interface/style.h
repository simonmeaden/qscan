#ifndef STYLEINFO_H
#define STYLEINFO_H

#include <QObject>
#include <QSharedPointer>
#include <QBitArray>
#include <QMap>
#include <QList>

#include <qyaml-cpp/QYamlCpp>
#include <yaml-cpp/yaml.h>

#include "util.h"

class StyledString;

namespace Util {
StyledString cleanText(const StyledString& text);
}

class StyleData
{
public:
  enum Type
  {
    Normal,
    Bold,
    Italic,
    Underline,
    Subscript,
    Superscript,
    Font_X_X_Small,
    Font_X_Small,
    Font_Small,
    Font_Medium,
    Font_Normal,
    Font_Large,
    Font_V_Large,
    Font_X_Large,
    Font_X_X_Large,
    Paragraph,
  };

  StyleData();
  StyleData(Type type);
  StyleData(Type type, int start, int length);

  int start() const;
  void setStart(int start);

  int length() const;
  void setLength(int length);

  int end() const;

  Type type() const;
  void setType(const Type& type);

  bool isValid();

  static int fontStretch(Type type);

protected:
  int m_start, m_length;
  Type m_style;

};
using Style = QSharedPointer<StyleData>;

class StyledString /*: public QString*/
{
public:
  StyledString();
  StyledString(const char* string);
  StyledString(QString string);
  StyledString(const StyledString& string);

  QString text() const;
  int length();
  bool isEmpty();
  StyledString mid(int position, int n = -1);
  StyledString left(int n);
  StyledString right(int n);

  void appendStyle(Style style);
  void appendStyles(QList<Style> styles);
  void removeStyle(Style style);
  void removeStyle(int index);
  void insertStyle(int index, Style style);
  int indexOf(const Style& style);
  void move(int from, int to);

  QList<StyledString> splitParagraphs();


  void setText(const QString& text);

  QList<Style> styles() const;
  void setStyles(const QList<Style>& styles);

  StyledString& operator=(const QString&);
  StyledString& operator=(const StyledString&);
  bool operator==(const StyledString&);
  StyledString& operator+(const StyledString&);
  StyledString& operator+=(const StyledString&);
  friend const StyledString operator+(const StyledString&, const StyledString&);

protected:
  QString m_text;
  QList<Style> m_styles;

  void transferStyles(StyledString& data_str, int p_start, int p_end, QMap<int, Style> others);

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
