/*
  Copyright © Simon Meaden 2019.
  This file was developed as part of the Biblios application but could
  easily be used elsewhere.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#include "mainwindow.h"
#include <QApplication>
#include <QImage>
#include <QLoggingCategory>
#include <QPlainTextEdit>
#include <QTextStream>
#include <QtDebug>

#include "documentdata.h"
#include "styledstring.h"

using Image = QSharedPointer<QImage>;
Q_DECLARE_METATYPE(Image)

int
main(int argc, char* argv[])
{
  //
  QApplication a(argc, argv);
  qRegisterMetaType<Image>();
  qRegisterMetaType<QScanner::DocData>();
  qRegisterMetaType<QScanner::DocumentData>();
  qRegisterMetaType<StyleData>();
  qRegisterMetaType<Style>();
  qRegisterMetaType<StyledString>();

  qSetMessagePattern("%{category} %{backtrace depth=1}:%{line} - %{message}");

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
  int res;

  try {
    res = QApplication::exec();

  } catch (const std::bad_alloc&) {
    // clean up here, e.g. save the session
    // and close all config files.

    return EXIT_FAILURE; // exit the application
  }

  return res;
}
