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
#include <QGroupBox>
#include <QImage>
#include <QInputDialog>
#include <QList>
#include <QMap>
#include <QMenu>
#include <QMessageBox>
#include <QPixmapCache>
#include <QProgressDialog>
#include <QScrollArea>
#include <QScrollBar>
#include <QScrollerProperties>

#include "logger.h"

#include "ocrtools.h"
#include "pageview.h"
#include "scanimage.h"
//#include "scanpage.h"
#include "documentdata.h"
#include "scanwidgets_global.h"

class QScan;
class OcrDialog;
class DocumentDataStore;

class SCANWIDGETSSHARED_EXPORT ScanEditor : public QFrame
{
  Q_OBJECT
public:
  explicit ScanEditor(QScan* scan,
                      const QString& configdir,
                      const QString& datadir,
                      QWidget* parent = nullptr);
  ~ScanEditor();

  void loadExistingFiles();
  void loadOptions(const QString& filename = QString());
  void saveOptions(const QString& filename = QString());

  void setImage(const QImage &image, const int resolution);
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

  void loadCover(const QString& filename);

  int pageCount();
  DocumentData documentData(int page_no);

  QString optionsFile() const;
  void setOptionsFile(const QString& optionsFile);

  QString tesseractLanguage() const;
  void setTesseractLanguage(const QString& tesseractLanguage);

  void cleanupImages();

  int resolution() const;
  void setResolution(int resolution);

  signals:
  void scanCancelled();
  void selected();
  void unselected();
  void selectionUnderway();
  void imageIsLoaded();
  void ocrImage(const DocumentData& documentData);

protected:
  Log4Qt::Logger* m_logger;
  QString m_current_doc_name;
  QString m_options_file;
  QString m_data_filename;
  DocumentDataStore *m_doc_data_store{};
  ScanImage* m_scan_display{};
  QProgressDialog* m_prog_dlg{};
  QScan* m_scan_lib{};
  QScrollArea* m_scroller{};
  PageView* m_page_view{};
  OcrTools* m_ocr_tools{};
  QString m_config_dir{};
  QString m_data_dir{};
  QString m_lang{};
  OcrDialog* m_ocr_dlg{};
  QPushButton *m_crop_btn{};
  QPushButton *m_left_btn{};
  QPushButton *m_move_up_btn{};
  QPushButton *m_move_down_btn{};
  QPushButton *m_move_to_btn{};
  QPushButton *m_cover_btn{};
  QPushButton *m_right_btn{};
  QPushButton *m_both_btn{};
  QPushButton *m_single_btn{};
  QPushButton *m_internal_btn{};
  QPushButton *m_rem_txt_btn{};
  QPushButton *m_rem_img_btn{};
  QPushButton *m_rem_both_btn{};
  QGroupBox *group1{}, *group2{}, *group3{};

  DocumentData m_cover;
  int m_resolution;

  QPixmapCache::Key crop_key;
  QPixmapCache::Key move_up_key;
  QPixmapCache::Key move_down_key;
  QPixmapCache::Key move_to_key;
  QPixmapCache::Key internal_key;
  QPixmapCache::Key cover_key;
  QPixmapCache::Key left_key;
  QPixmapCache::Key right_key;
  QPixmapCache::Key single_key;
  QPixmapCache::Key both_key;
  QPixmapCache::Key scale_key;
  QPixmapCache::Key remove_both_key;
  QPixmapCache::Key remove_text_key;
  QPixmapCache::Key remove_image_key;

  bool eventFilter(QObject* obj, QEvent* event) override;

  void adjustScrollbar(qreal factor);
  void initGui();
  void makeConnections();
  void receiveImage(const QImage& image);
  void receiveImages(const QImage& left, const QImage& right);
  void receiveString(int documentData, const QString& str);
  QString saveImage(int index, const QImage& image);
  void saveModifiedImage(int index, const QImage& image);
  void makeCover();
  void receiveLoadText(int page_no);
  void receiveWorkOnRequest(int documentData);
  void receiveOcrPageRequest(int documentData);
  void receiveOcrImageRequest(int documentData, const QImage &image, const QRect &rect);
  void receiveOcrPageResult(const DocumentData &doc_data);
  void receiveOcrImageResult(int documentData, const QString &text);
  void receiveOcrImageRectResult(int documentData, const QString &text);
  void receiveOcrDialogFinished(int result);
  void receivePageMoved(int start_row, int dest_row);
  void removeBoth(int page_no = -1);
  void removeImage(int page_no = -1);
  void removeText(int page_no = -1);
  void saveModifiedText(int page_no, const QStringList& text);
  QImage thumbnail(const QImage& image) const;
  void rescale();

  void enableMoveBtns(bool enable);
  void enableListBtns();
  void disableListBtns();
  void enableEditBtns();
  void disableEditBtns();
  //  void moveImage(const DocumentData& data);

  static const QString OCR_IMAGE_NAME;
  static const QString INTERNAL_IMAGE_NAME;
  static const QString OCR_IMAGE_FILTER;
  static const QString INTERNAL_IMAGE_FILTER;

  static const QString OPTIONS_FILE;
  static const QString CURRENT_DOCUMENT;
  static const QString TESSERACT;
  static const QString LANGUAGE;
};

#endif // SCANEDITOR_H
