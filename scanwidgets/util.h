#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include <QtMath>

#include "style.h"

namespace Util {

qreal round(qreal value, quint8 places);
int dpiToDpm(int value);
int dpmToDpi(int value);

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

}; // namespace Util

#endif // UTIL_H
