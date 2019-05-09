/*
    Copyright © Simon Meaden 2019.
    This file was developed as part of the QScan cpp library but could
    easily be used elsewhere.

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
#ifndef SCANEDITOR_H
#define SCANEDITOR_H

#include <QAction>
#include <QFrame>
#include <QGridLayout>
#include <QImage>
#include <QInputDialog>
#include <QList>
#include <QMap>
#include <QMenu>
#include <QProgressDialog>
#include <QScrollArea>
#include <QScrollBar>
#include <QMessageBox>

#include "logger.h"

#include "ocrtools.h"
#include "pageview.h"
#include "scanimage.h"
#include "scanpage.h"
#include "scanwidgets_global.h"

class QScan;

class SCANWIDGETSSHARED_EXPORT ScanEditor : public QFrame
{
  Q_OBJECT
public:
  explicit ScanEditor(QScan* scan,
                      QString& configdir,
                      QString  datadir,
                      QWidget* parent = nullptr);

  void loadExistingFiles();
  void loadOptions(const QString& filename = QString());
  void saveOptions(const QString& filename = QString());

  void setImage(const QImage& image);
  void setScanProgress(const int& progress);
  void scanningStarted();
  QString documentName() const;
  void setDocumentName(const QString& documentName);
  void setDocumentPath(const QString& path);
  void completeDocument();

  void setSelectedName(const QString& selected_name);

  void selectAll();
  void rotate180();
  void rotateCW();
  void rotateACW();
  void rotateByAngle();
  void rotateByEdge();
  void copySelection();
  void cropToSelection();
  void clearSelection();
  void cropToContent();
  void rescan();
  void scale();
  void save();
  void saveAs();
  void zoomIn();
  void zoomOut();
  void fitBest();
  void fitHeight();
  void fitWidth();
  void setDefaultPageCropSize();
  void splitPages();
  void splitLeftPage();
  void splitRightPage();
  void makePage();

  void loadCover(const QImage& cover);
  void loadImage(int index, const QImage& image, const QString& text);

  int pageCount();
  Page page(int index);

  QString optionsFile() const;
  void setOptionsFile(const QString& optionsFile);

  QString tesseractLanguage() const;
  void setTesseractLanguage(const QString& tesseractLanguage);

signals:
  void scanCancelled();
  void selected();
  void unselected();
  void selectionUnderway();
  void imageIsLoaded();
  void ocrImage(const Page& page);

protected:
  Log4Qt::Logger* m_logger;
  QString m_current_doc_name;
  QString m_options_file;
  ScanImage* m_scan_display{};
  QProgressDialog* m_prog_dlg{};
  QScan* m_scan_lib{};
  QScrollArea* m_scroller{};
  PageView* m_page_view{};
  OcrTools* m_ocr_tools{};
  QString m_config_dir{};
  QString m_data_dir{};
  QString m_lang{};

  QMap<int, Page> m_pages;
  Page m_cover;
  bool m_save_all_texts{};

  bool eventFilter(QObject* obj, QEvent* event) override;

  void adjustScrollbar(qreal factor);
  void initGui();
  void makeConnections();
  void receiveImage(const QImage& img);
  void receiveImages(const QImage& left, const QImage& right);
  void receiveString(int page, const QString& str);
  void saveImage(int index, const QImage& image);
  void saveAsCover(const QImage& image);
  void receiveOcrRequest(int page);
  void receiveOcrResult(const Page& page);
  void saveText(int index, const Page& page);
  void clearSaveAllTextsFlag();

  static const QString OPTIONS_FILE;
  static const QString CURRENT_DOCUMENT;
  static const QString TESSERACT;
  static const QString LANGUAGE;
};

#endif // SCANEDITOR_H
