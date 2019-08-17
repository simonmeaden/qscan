#ifndef UTIL_H
#define UTIL_H

#include <QtMath>
#include <QString>

namespace DocUtil {

/*!
  \brief Rounds a qreal value to a set number of places.

  Takes a qreal value and rounds it to a specified number of places.
  \code round(2.7653, 2); \endcode
  returns a value of 3.77.
*/
qreal round(qreal value, quint8 places);

/*!
  \brief Convert a dots-per-inch value to dots-per-meter.

  Most images use a dots-per-inch value, however Qt at least,
  probably others as well, use a dots-per-meter value. This method converts
  value dots-per-inch into dots-per-metre, returning the converted value.
*/
int dpiToDpm(int value);

/*!
  \brief Convert a  dots-per-meter value to dots-per-inch.

  Most images use a dots-per-inch value, however Qt at least,
  probably others as well, use a dots-per-meter value. This method converts
  value dots-per-metre into dots-per-inch, returning the converted value.
*/
int dpmToDpi(int value);

int makeOdd(qreal value);
int makeEven(qreal value);

/*!
  \brief Cleans text of dodgy characters and un-escaped single and double
  quotes..

  Some characters such as em-dash which are detected by Tesseract
  do not convert well in YAML so convert them to simpler forms such a hyphen.

  Double quotes, either standard double quotes (") or their unicode
  brethren, left and right double quotes, are converted into escaped
  standard double quotes.
*/
QString cleanText(const QString& text);

static const QChar LF(0x000A);
static const QChar CR(0x000D);
static const QChar BACKSLASH(0x005C);
static const QChar ESCAPE(BACKSLASH);
static const QChar S_QUOTE(0x0027);
static const QChar LEFT_S_QUOTE(0x2018);
static const QChar RIGHT_S_QUOTE(0x2019);
static const QChar D_QUOTE(0x0022);
static const QChar LEFT_D_QUOTE(0x201C);
static const QChar RIGHT_D_QUOTE(0x201D);
static const QChar EMDASH(0x2014);
static const QChar HYPHEN(0x002D);

static const QChar n_CHAR(0x006E);

}; // namespace DocUtil

#endif // UTIL_H
