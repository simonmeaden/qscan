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
#include "mainwindow.h"
#include <QApplication>
#include <QImage>
#include <QLoggingCategory>
#include <QPlainTextEdit>
#include <QTextStream>
#include <QtDebug>

#include "documentdata.h"
#include "style.h"

using Image = QSharedPointer<QImage>;
Q_DECLARE_METATYPE(Image)

int main(int argc, char* argv[])
{
  //
  QApplication a(argc, argv);
  qRegisterMetaType<Image>();
  qRegisterMetaType<DocData>();
  qRegisterMetaType<DocumentData>();
  qRegisterMetaType<StyleData>();
  qRegisterMetaType<Style>();
  qRegisterMetaType<StyledString>();

  //  qSetMessagePattern("%{file}(%{line}): %{message}");

  MainWindow w;

  QTranslator translator_gb, translator_us;

  translator_us.load("Scanner_en_US");
  translator_us.load("ocr_en_US");
  translator_us.load("scanwidgets_en_US");
  translator_us.load("qscan_en_US");
  a.installTranslator(&translator_us);

  translator_gb.load("Scanner_en_GB");
  translator_gb.load("ocr_en_GB");
  translator_gb.load("scanwidgets_en_GB");
  translator_gb.load("qscan_en_GB");
  a.installTranslator(&translator_gb);

  w.show();
  int res = QApplication::exec();

  return res;
}
