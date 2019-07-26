#ifndef OCRDIALOG_H
#define OCRDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QFileInfo>
#include <QFont>
#include <QFrame>
#include <QGridLayout>
#include <QGuiApplication>
#include <QLabel>
#include <QMap>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QScreen>
#include <QSlider>
#include <QStackedLayout>
#include <QStatusBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QStandardPaths>
#include <QDir>

#include <qwt_scale_engine.h>
#include <qwt_slider.h>

#include "qscanplugin_global.h"
#include "ocrtools.h"
#include "documentdata.h"
#include "util.h"
#include "baseeditor.h"

class OcrImage;
class ScanList;

class QSCANPLUGINSHARED_EXPORT OcrEditor : public BaseEditor
{
  Q_OBJECT
public:
  explicit OcrEditor(/*QString lang = "eng",*/ QWidget* parent = nullptr);
  //  ~OcrDialog() override = default;

  //  OcrDialog(const OcrDialog&) = delete;
  //  OcrDialog(OcrDialog&&) = delete;
  //  OcrDialog& operator=(const OcrDialog&) = delete;
  //  OcrDialog& operator=(OcrDialog&&) = delete;

  QImage image();
  void setData(int page_no, const QImage& image, const DocumentData& doc_data, const QString& lang);
  void setOcrImage(int page_no, const QImage& image);
  void setOcrText(int page_no, const QString& texts);
  void appendOcrText(int page_no, const QString& texts);
  void ocrFailed(int page_no);
  QList<StyledString> texts();

  bool imageChanged() const;

  bool hasText();

  QImage modifiedImage();
  int pageNumber();

  DocumentData documentData() const;

  QList<QMenu*> menus();
  QList<QToolBar*> toolbars();

signals:
  void sendOcrRequest(int, const QImage&, const QRect& rect = QRect());
  void saveModifiedImage(int page_no, const QImage& image, const QString filename);
  void saveModifiedData(const DocumentData& data);
  void saveSelectedDocumentImage(int page_no,
                                 int image_index,
                                 const QImage& image,
                                 const DocumentData& doc_data);
  void makeCompleted(DocumentData);
  void goToScanEditor();
  void rejectChanges();

protected:
  enum ChangeType
  {
    None,
    Greyscale,
    Monochrome,
    Rescale,
    Denoise,
  };
  ScanList* m_scan_list;
  OcrImage* m_image_display;
  OcrTools* m_ocr_tools{};
  int m_page_no{};
  int m_item_count{};
  DocumentData m_doc_data;
  QPushButton* m_crop_btn{};
  QPushButton* m_clr_to_back_btn{};
  QPushButton* m_greyscale_btn{};
  QPushButton* m_monochrome_btn{};
  QPushButton* m_ocr_btn{};
  //  QPushButton* m_ocr_sel_btn{};
  QPushButton* m_invert_btn{};
  QPushButton* m_denoise_btn{};
  QPushButton* m_dewarp_btn{};
  QPushButton* m_deskew_btn{};
  QPushButton* m_save_txt_btn{};
  QPushButton* m_save_img_btn;
  QPushButton* m_close_and_save_btn;
  QPushButton* m_move_sel_to_doc_btn{};
  QPushButton* m_rem_selection_btn{};
  QwtSlider* m_intvalue_slider{};
  QwtSlider* m_denoise_template_slider{};
  QwtSlider* m_denoise_search_slider{};
  QwtSlider* m_denoise_filter_slider{};
  QwtSlider* m_dblvalue_slider{};
  QLabel* m_intvalue_lbl{};
  QLabel* m_dblvalue_lbl{};
  QLabel* m_denoise_filter_lbl{};
  QLabel* m_denoise_template_lbl{};
  QLabel* m_denoise_search_lbl{};
  QLabel* m_image_name_lbl{};
  QLabel* m_image_size_lbl{};
  QLabel* m_message_lbl{};
  QLabel* m_res_lbl{};
  QStackedLayout* m_ctl_stack{};
  QStatusBar* m_status_bar{};

  QString m_image_name;
  QString m_lang;
  QString m_image_size;
  QString m_message;
  QString m_resolution;
  int m_btn_stack{};
  int m_intvalue_stack{};
  int m_dblvalue_stack{};
  int m_denoise_stack{};
  ChangeType m_change_type;

  void loadOptions();
  //  void receiveOcrPageRequest(int page_no);

  void resizeEvent(QResizeEvent* event) override;

  void setMessageLabel(const QString& texts);
  void setSizeLabel(int width, int height);
  void setNameLabel(int documentData, QString name);
  void setResLabel(int xres, int yres);

  void initGui();
  void requestOcr();
  void requestOcrOnSelection();
  void help();
  void setSelected();
  void setUnselected();
  void crop();
  void greyscale();
  void monochrome();
  void invert();
  void denoise();
  void dewarp();
  void deskew();
  void rotate180();
  void rotateCW();
  void rotateCCW();
  void rescale();
  void saveData();
  void saveCurrentImage();
  //  void discard();
  void acceptChanges();
  void undoChanges();
  void close();

  void scanListDataChanged();
  void ocrImageChanged(bool changed);

  void moveToDocument();
  void removeItemFromDocument();

  void setIntValueLabel(qreal value);
  void setDoubleValueLabel(qreal value);
  void setDenoiseTemplateLabel(qreal value);
  void setDenoiseSearchLabel(qreal value);
  void setDenoiseFilterLabel(qreal value);
  void valueAccepted();
  void applyValue();

  void setCurrentChangeState();

  void disableBinarise();
  void enableBinarise();

  void scanListWasClicked(const QModelIndex&);
  void scanListWasDoubleClicked(const QModelIndex& index);

  void completeOperation();

  void enableCleanImageBtns(bool enable);
  void imageSizeHasChanged(int width, int height, int xres, int yres);

  QFrame* initIntSliderFrame();
  QFrame* initDoubleSliderFrame();
  QFrame* initDenoiseSliderFrame();
  void setResolution(const QImage& image);
  //  QFrame* initOcrBtns();
  QFrame* initMoveBtns();
  QFrame* initModifyBtns();
  QFrame* initFinaliseBtns();

  static const QString TESSERACT;
  static const QString LANGUAGE;

};

//class OcrDialog : public QDialog
//{
//  Q_OBJECT
//public:
//  explicit OcrDialog(QWidget* parent = nullptr);

//  void open() override;
//};

#endif // OCRDIALOG_H
