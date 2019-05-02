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
#include <QMenu>
#include <QProgressDialog>
#include <QScrollArea>
#include <QScrollBar>

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
  ScanEditor(QScan* scan,
             const QString& configdir,
             const QString& datapath,
             const QString& lang,
             QWidget* parent = nullptr);
  //  ~ScanEditor() override;

  void setImage(const QImage& image);
  void setScanProgress(const int& progress);
  void scanningStarted();
  QString documentName() const;
  void setDocumentName(const QString& documentName);

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
  void receiveOcrPage(int index);

  int pageCount();
  Page page(int index);

signals:
  void scanCancelled();
  void selected();
  void unselected();
  void selectionUnderway();
  void imageIsLoaded();

protected:
  Log4Qt::Logger* m_logger;
  QString m_document_name;
  ScanImage* m_image_display;
  QProgressDialog* m_prog_dlg;
  QPoint m_origin;
  QScan* m_scan_lib;
  QString m_selected_name;
  QScrollArea* m_scroller;
  PageView* m_page_view;
  OcrTools* m_ocr_tools;
  QString m_configdir, m_datadir;

  QList<Page> m_pages;

  bool eventFilter(QObject* obj, QEvent* event) override;

  void adjustScrollbar(qreal factor);
  void initGui();
  void connectActions();
  void receiveImage(const QImage& img);
  void receiveImages(const QImage& left, const QImage& right);
  void receiveString(int page, const QString& str);
  void saveImage(int index, const QImage& image);
  void saveAsCover(const QImage& image);
};

#endif // SCANEDITOR_H
