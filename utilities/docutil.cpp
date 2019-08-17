#include "docutil.h"

namespace DocUtil {

/*!
  \brief Rounds a qreal value to a set number of places.

  Takes a qreal value and rounds it to a specified number of places.
  \code round(2.7653, 2); \endcode
  returns a value of 3.77.
*/
qreal round(qreal value, quint8 places) {
  int mult = int(qPow(qreal(10), places));
  int result = qRound((value * qreal(mult)) + 0.5);
  return qreal(result / qreal(mult));
}

/*!
  \brief Convert a dots-per-inch value to dots-per-meter.

  Most images use a dots-per-inch value, however Qt at least,
  probably others as well, use a dots-per-meter value. This method converts
  value dots-per-inch into dots-per-metre, returning the converted value.
*/
int dpiToDpm(int value) {
  int ppm = int((double(value) / 25.4) * 1000.0);
  return ppm;
}

/*!
  \brief Convert a  dots-per-meter value to dots-per-inch.

  Most images use a dots-per-inch value, however Qt at least,
  probably others as well, use a dots-per-meter value. This method converts
  value dots-per-metre into dots-per-inch, returning the converted value.
*/
int dpmToDpi(int value) {
  int dpi = int((double(value) * 25.4) / 1000.0);
  return dpi;
}

/*!
  \brief Cleans text of dodgy characters and un-escaped single and double
  quotes..

  Some characters such as em-dash which are detected by trhe scanner
  do not convert well in YAML so convert them to simpler forms such a hyphen.

  Double quotes, either standard double quotes (") or their unicode
  brethren, left and right double quotes, are converted into escaped
  standard double quotes.

  \param text - the string to clean.
  \return - the 'cleaned' text.
*/
QString cleanText(const QString &text) {
  if (text.contains(D_QUOTE) ||       // standard d-quote
      text.contains(S_QUOTE) ||       // standard s-quote
      text.contains(LEFT_D_QUOTE) ||  // left d-quote
      text.contains(RIGHT_D_QUOTE) || // right d-quote
      text.contains(LEFT_S_QUOTE) ||  // left s-quote
      text.contains(RIGHT_D_QUOTE)) { // right s-quote
    QString escaped_text;
    bool bs = false;

    //    int index = text.indexOf("tug") + 3;
    //    QChar h = text.at(index);

    for (auto c : text) {
      if (c == D_QUOTE || c == S_QUOTE) { // standard d-quote or s-quote
        //        if (!bs) {
        //          //          escaped_text += BACKSLASH; // escape it
        //        }

        escaped_text += c;
        bs = false;

      } else if (c == LEFT_D_QUOTE ||
                 c == RIGHT_D_QUOTE) { // left or right d-quote char
        //        if (!bs) {                                          //
        //        un-escaped
        //          //          escaped_text += BACKSLASH; // escape it

        //        } else {
        //          bs = false;
        //        }

        escaped_text += D_QUOTE; // replace with standard s-quote
        bs = false;

      } else if (c == LEFT_S_QUOTE ||
                 c == RIGHT_S_QUOTE) { // left or right s-quote char
        //        if (!bs) {                                          //
        //        un-escaped
        //          //          escaped_text += BACKSLASH; // escape it

        //        } else {
        //          bs = false;
        //        }

        escaped_text += S_QUOTE; // replace with standard s-quote
        bs = false;

      } else if (c == EMDASH) {
        escaped_text += HYPHEN;

      } else if (c == BACKSLASH) { // escaped backslash char
        if (!bs) {
          bs = true;

        } else { /* escaped \ ie \\ */
          bs = false;
          escaped_text += c;
        }

      } else {
        // any other character cancels escaped character.
        if (bs) {
          escaped_text += BACKSLASH; // escape it
          bs = false;
        }

        escaped_text += c;
      }
    }

    return escaped_text;
  }

  return text;
}

int makeOdd(qreal value) { return ((int((int(value) - 1) / 2.0) * 2) + 1); }

int makeEven(qreal value) { return int(int(int(value) / 2.0) * 2.0); }

} // namespace DocUtil
