#ifndef STYLEINFO_H
#define STYLEINFO_H

#include <QObject>
#include <QSharedPointer>
#include <QBitArray>

class StyleData
{
public:
  enum Type
  {
    Normal,
    Bold,
    Italic,
    Underline,
    Font_X_X_Small,
    Font_X_Small,
    Font_Small,
    Font_Medium,
    Font_Normal,
    Font_Large,
    Font_V_Large,
    Font_X_Large,
    Font_X_X_Large,
  };

  StyleData();
  StyleData(Type style, int start, int length);

  int start() const;
  void setStart(int start);

  int length() const;
  void setLength(int length);

  Type style() const;
  void setStyle(const Type& style);

  bool isValid();

  static int fontStretch(Type type);

protected:
  int m_start, m_length;
  Type m_style;

};
using Style = QSharedPointer<StyleData>;

class StyledString : public QString
{
public:
  StyledString();
  StyledString(const char* string);
  StyledString(QString string);

  void appendStyle(Style style);
  void removeStyle(Style style);
  void removeStyle(int index);
  void insertStyle(int index, Style style);
  int indexOf(const Style& style);
  void move(int from, int to);

  StyledString& operator=(const QString&);
  StyledString& operator=(const StyledString&);

  void setText(const QString& text);

  QList<Style> styles() const;
  void setStyles(const QList<Style>& styles);

protected:
  QList<Style> m_styles;

};

Q_DECLARE_METATYPE(StyleData)
Q_DECLARE_METATYPE(Style)
Q_DECLARE_METATYPE(StyledString)

#endif // STYLEINFO_H
