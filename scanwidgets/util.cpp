#include "util.h"

namespace Util {

/*
   Rounds a qreal value to a set number of places.

  e.g. 2.7653 rounds to 3.77
*/
qreal round(qreal value, quint8 places)
{
  int mult = int(qPow(qreal(10), places));
  int result = qRound((value * qreal(mult)) + 0.5);
  return qreal(result / qreal(mult));
}

/*!
  \brief Convert a dots-per-inch value to dots-per-meter.

  Most images use a dots-per-inch value, however Qt at least,
  probably others as well, use a dots-per-meter value.

  \param value the dpi value.
  \return an integer ppm value;
*/
int dpiToDpm(int value)
{
  int ppm = int((double(value) / 25.4) * 1000.0);
  return ppm;
}

/*!
  \brief Convert a  dots-per-meter value to dots-per-inch.

  Most images use a dots-per-inch value, however Qt at least,
  probably others as well, use a dots-per-meter value.

  \param value the ppm value.
  \return an integer dpi value;
*/
int dpmToDpi(int value)
{
  int dpi = int((double(value) * 25.4) / 1000.0);
  return dpi;
}

} // namespace Util
