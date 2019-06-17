#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <QtMath>

namespace Util {

qreal round(qreal value, quint8 places);
int dpiToDpm(int value);
int dpmToDpi(int value);

}; // namespace Util

#endif // UTIL_H
