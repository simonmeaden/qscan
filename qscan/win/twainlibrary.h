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
#ifndef SCANTWAIN_H
#define SCANTWAIN_H

#include <QObject>

#include "scaninterface.h"

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)

class ScanTwain : public ScanLibrary
{
public:
  ScanTwain(QObject* parent = nullptr);

  bool init() override;
  QStringList getDevices() override;
  Scanner getDevice(QString name) override;
};

#endif

#endif // SCANTWAIN_H