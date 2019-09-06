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
#ifndef SCANEDITOR_H
#define SCANEDITOR_H

#include <QAction>
#include <QComboBox>
#include <QDir>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QImage>
#include <QInputDialog>
#include <QList>
#include <QMap>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmapCache>
#include <QProgressDialog>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QScrollerProperties>
#include <QStackedLayout>
#include <QStandardPaths>
#include <QToolBar>

#include "baseeditor.h"
#include "documentdata.h"
#include "iscanwidgetinterface.h"
#include "pageview.h"
#include "widgets_global.h"
#include "scanimage.h"
#include "iscanlibrary.h"

namespace QScanner {

class QScan;
class OcrEditor;
class DocumentDataStore;
// class ScanImage;

class WIDGETSSHARED_EXPORT ScanEditor
  : public BaseEditor
  , public IScanWidgetInterface
{
  Q_OBJECT
public:
  explicit ScanEditor(QWidget* parent = nullptr);
  ~ScanEditor();

  void loadCurrentDocument();
  void loadOptions();
  void saveOptions();

  void setImage(const QImage& image, const int resolution);
  void setScanProgress(const int& progress);
  void scanningStarted();
  QString documentName() const;
  void setDocumentName(const QString& documentName);
  void setDocumentPath(const QString& path);
  void completeDocument(DocumentData doc_data);

  void setSelectedName(const QString& selected_name);

  void selectAll();
  void rotate180();
  void rotateCW();
  void rotateACW();
  void rotateByAngle();
  void rotateByEdge();
  void copySelection(bool);
  void cropToSelection();
  void clearSelection();
  void cropToContent();
  void help();
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
  void moveBothPagesToDocument();
  void moveLeftPageToDocument();
  void moveRightPageToDocument();
  void movePageToDocument();

  void loadCover(const QString& filename);

  int pageCount();
  DocumentData documentData(int page_no);

  QString optionsFile() const;
  //  void setOptionsFile(const QString& optionsFile);

  QString tesseractLanguage() const;
  void setTesseractLanguage(const QString& tesseractLanguage);

  void cleanupImages();

  int resolution() const;
  void setResolution(int resolution);

  void setConfigDir(const QString& config_dir);
  void setDataDir(const QString& data_dir);
  void setLang(const QString& lang);

  QStringList scanners();
  QList<QMenu*> menus() override;
  QList<QToolBar*> toolbars() override;

signals:
  void scanCancelled();
  void selected();
  void unselected();
  void selectionUnderway();
  void imageIsLoaded();
  void ocrImage(const DocumentData& documentData);

  void sendOcrRequest(int);
  void optionsSet();
  void sendWorkData(int page_no,
                    QImage image,
                    DocumentData doc_data,
                    QString lang);
  void goToOcrEditor();

protected:
  QScan* m_scan_lib{};
  int m_main_id, m_ocr_stack_id;
  QStackedLayout* m_scan_layout;
  QGridLayout* m_editor_layout;
  QAction* m_open_act{}, *m_close_act{}, *m_set_docname_act{};
  QMenu* m_batch_menu{}, *m_scanners_menu{}, *m_modes_menu{}, *m_src_menu{},
         *m_res_menu{};
  //  OcrFrame* m_ocr_frame;

  QString m_current_doc_name;
  QString m_data_filename;
  DocumentDataStore* m_doc_data_store{};
  ScanImage* m_scan_display{};
  QProgressDialog* m_prog_dlg{};
  QScrollArea* m_scroller{};
  PageView* m_page_view{};
  QPushButton* m_crop_btn{};
  QPushButton* m_left_btn{};
  QPushButton* m_move_up_btn{};
  QPushButton* m_move_down_btn{};
  QPushButton* m_move_to_btn{};
  //  QPushButton *m_move_to_doc_btn{};
  QPushButton* m_cover_btn{};
  QPushButton* m_right_btn{};
  QPushButton* m_both_btn{};
  QPushButton* m_single_btn{};
  QPushButton* m_internal_btn{};
  QPushButton* m_rem_txt_btn{};
  QPushButton* m_rem_img_btn{};
  QPushButton* m_rem_both_btn{};
  //  QComboBox* m_scanner_box{}/*, *m_mode_box{}*//*, *m_res_combo{}*//*, *m_source_box{}*/;
  QStackedLayout* m_res_layout{};
  int m_stack_range_id, m_stack_list_id;
  QFrame* m_res_range{};
  QFrame* m_res_list{};
  QLabel* m_min_res{}, *m_max_res{}, *m_curr_src{},
          *m_curr_mode{} /*,m_curr_scanner{}*/;
  QIntValidator* m_res_validator{};
  QGroupBox* image_edit_group{},
             *page_list_group{} /*, *image_transfer_group{}*/;
  QString m_current_scanner;

  int m_image_resolution;

  bool eventFilter(QObject* obj, QEvent* event) override;
  void receiveScannerData();
  void receiveScannerChange(bool);
  //  void receiveModeChange(ScanDevice* device);
  //  void receiveSourceChange(ScanDevice* device);
  void resolutionEdited(const QString& value);
  void receiveModeChange();
  void receiveSourceChange();
  void receiveResolutionListChange(bool);
  void receiveResolutionRangedChange(bool);
  //  void receiveScannerChange(int index);
  void scanHasFailed();
  void scanOpenHasFailed();
  void startScanning();

  void batchLoad();
  void batchCrop();
  void batchOcr();

  void adjustScrollbar(qreal factor);
  void initGui();
  void makeConnections();
  void receiveImage(const QImage& image);
  void receiveImages(const QImage& left, const QImage& right);
  //  void receiveString(int page_no, const QString &str);
  QString saveImage(int index, const QImage& image);
  QString saveDocumentImage(int page_no,
                            int image_index,
                            const QImage& image,
                            const DocumentData& doc_data);
  void saveModifiedImage(int index, const QImage& image);
  void movePageToCover();
  void receiveLoadText(int page_no);
  void receiveWorkOnRequest(int documentData);
  //  void receiveOcrPageRequest(int documentData);
  //  void receiveOcrImageRequest(int documentData, const QImage& image, const QRect& rect);
  void receiveOcrPageResult(const DocumentData& doc_data);
  //  void receiveOcrImageResult(int documentData, const QString& text);
  //  void receiveOcrImageRectResult(int documentData, const QString& text);
  void receiveModifyActionFinished(DocumentData doc_data);
  void receiveModificationCancelled();
  void receivePageMoved(int, int);
  void removeBoth(int page_no = -1);
  void removeImage(int page_no = -1);
  void removeText(int page_no = -1);
  void saveModifiedData(const DocumentData& doc_data);
  QImage thumbnail(const QImage& image) const;
  void rescale();
  void moveSelectionToInternal();

  void enableMoveBtns(bool);
  void enableListBtns();
  void disableListBtns();
  void enableEditBtns();
  void disableEditBtns();
  //  void moveImage(const DocumentData& data);
  //  void initPageListBtns();
  //  void initImageEditBtns();
  //  void initTransferBtns();
  void saveDataStore();

  //  QFrame* initMenuAndToolbars();
  void initScannerMenu();

  static const QString OCR_IMAGE_NAME;
  static const QString INTERNAL_IMAGE_NAME;
  static const QString OCR_IMAGE_FILTER;
  static const QString INTERNAL_IMAGE_FILTER;

  static const QString CURRENT_DOCUMENT;
  void initRightToolbar(QToolBar* r_toolbar);
  void initTopToolbar(QToolBar* t_toolbar);
  void initLeftToolbar(QToolBar* l_toolbar);
  void initCentreToolbar(QToolBar* c_toolbar);
};

} // end of namespace QScanner

#endif // SCANEDITOR_H
