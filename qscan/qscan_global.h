/*
    Copyright © Simon Meaden 2019.
    This file is part of the QScan cpp library.

    QScan is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QScan is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QScan.  If not, see <http://www.gnu.org/licenses/>.
    It is also available on request from Simon Meaden simonmeaden@sky.com.
*/
#ifndef SCAN_GLOBAL_H
#define SCAN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SCAN_LIBRARY)
#define SCANSHARED_EXPORT Q_DECL_EXPORT
#else
#define SCANSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SCAN_GLOBAL_H
