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
#include <QStackedLayout>
#include <QStandardPaths>
#include <QDir>
#include <QPushButton>
#include <QComboBox>
#include <QToolBar>
#include <QMenuBar>

#include "qscanlogging.h"
#include "qscanplugin_global.h"
#include "scaninterface.h"
#include "pageview.h"
#include "scanimage.h"
#include "documentdata.h"
#include "stackableframe.h"

class QScan;
class OcrEditor;
class DocumentDataStore;
//class ScanImage;

class QSCANPLUGINSHARED_EXPORT ScanEditor : public StackableFrame
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
  QList<QMenu*> menus();
  //  QList<QToolBar*> toolbars() override;

signals:
  void scanCancelled();
  void selected();
  void unselected();
  void selectionUnderway();
  void imageIsLoaded();
  void ocrImage(const DocumentData& documentData);
  void currentScanner(const QString);
  void currentMode(const QString);
  void currentSource(const QString);
  void currentResolution(int);
  //  void editingImage(bool);

  void sendOcrRequest(int);
  void optionsSet(ScanDevice*);
  void sendWorkData(int page_no, QImage image, DocumentData doc_data, QString lang);


protected:
  QScan* m_scan_lib{};
  int m_main_id, m_ocr_stack_id;
  QStackedLayout* m_scan_layout;
  QGridLayout* m_editor_layout;
  QAction*  m_open_act{}, *m_close_act{}, *m_set_docname_act{};
  QMenu* m_scanners_menu{}, *m_modes_menu{}, *m_src_menu{}, *m_res_menu{};
  //  OcrFrame* m_ocr_frame;

  QString m_current_doc_name;
  QString m_options_file;
  QString m_data_filename;
  DocumentDataStore* m_doc_data_store{};
  ScanImage* m_scan_display{};
  QProgressDialog* m_prog_dlg{};
  QScrollArea* m_scroller{};
  PageView* m_page_view{};
  QString m_config_dir{};
  QString m_data_dir{};
  QString m_lang{};
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
  QLabel* m_min_res{}, *m_max_res{}, *m_curr_src{}, *m_curr_mode{}/*,m_curr_scanner{}*/;
  QIntValidator* m_res_validator{};
  QGroupBox* image_edit_group{}, *page_list_group{}, *image_transfer_group{};
  QString m_selected_name;

  int m_image_resolution;

  bool eventFilter(QObject* obj, QEvent* event) override;
  void receiveOptionsSet(ScanDevice* device);
  void receiveScannerChange(bool);
  void receiveModeChange(ScanDevice* device);
  void receiveSourceChange(ScanDevice* device);
  void resolutionEdited(const QString& value);
  void modeChangeSelected(const bool);
  void sourceChangeSelected(bool);
  void resolutionChangeSelected(bool);
  void resolutionRangedChangeSelected(bool);
  void scannerSelectionChanged(int index);
  void scanHasFailed();
  void scanOpenHasFailed();
  void startScanning();

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
  void makeCover();
  void receiveLoadText(int page_no);
  void receiveWorkOnRequest(int documentData);
  //  void receiveOcrPageRequest(int documentData);
  //  void receiveOcrImageRequest(int documentData, const QImage& image, const QRect& rect);
  void receiveOcrPageResult(const DocumentData& doc_data);
  //  void receiveOcrImageResult(int documentData, const QString& text);
  //  void receiveOcrImageRectResult(int documentData, const QString& text);
  void receiveModifyActionFinished(DocumentData doc_data);
  void receiveModificationCancelled();
  void receivePageMoved(int start_row, int dest_row);
  void removeBoth(int page_no = -1);
  void removeImage(int page_no = -1);
  void removeText(int page_no = -1);
  void saveModifiedData(const DocumentData& doc_data);
  QImage thumbnail(const QImage& image) const;
  void rescale();
  void moveSelectionToInternal();

  void enableMoveBtns(bool enable);
  void enableListBtns();
  void disableListBtns();
  void enableEditBtns();
  void disableEditBtns();
  //  void moveImage(const DocumentData& data);
  void initPageListBtns();
  void initImageEditBtns();
  void initTransferBtns();
  void saveDataStore();

  //  QFrame* initMenuAndToolbars();
  void initScannerMenu();

  static const QString OCR_IMAGE_NAME;
  static const QString INTERNAL_IMAGE_NAME;
  static const QString OCR_IMAGE_FILTER;
  static const QString INTERNAL_IMAGE_FILTER;

  static const QString OPTIONS_FILE;
  static const QString CURRENT_DOCUMENT;
};

#endif // SCANEDITOR_H
