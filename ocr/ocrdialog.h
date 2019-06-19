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
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QScreen>
#include <QSlider>
#include <QStackedLayout>
#include <QStatusBar>
#include <QTextEdit>
#include <QVBoxLayout>

#include <qt5/qwt/qwt_scale_engine.h>
#include <qt5/qwt/qwt_slider.h>

#include "documentdata.h"
#include "util.h"

class OcrImage;
class ScanList;

class OcrDialog : public QDialog
{
  Q_OBJECT
public:
  explicit OcrDialog(QWidget* parent = nullptr);
  //  ~OcrDialog() override = default;

  //  OcrDialog(const OcrDialog&) = delete;
  //  OcrDialog(OcrDialog&&) = delete;
  //  OcrDialog& operator=(const OcrDialog&) = delete;
  //  OcrDialog& operator=(OcrDialog&&) = delete;

  QImage image();
  void setData(int index, const QImage& image, const DocumentData& page);
  void setOcrImage(int index, const QImage& image);
  void setOcrText(int page_no, const QString &text);
  void appendOcrText(int page_no, const QString &text);
  QStringList text();

  bool imageChanged() const;
  QSize sizeHint() const override;

  QImage modifiedImage();
  int pageNumber();

  DocumentData page() const;

  void open() override;

  signals:
  void sendOcrRequest(int, const QImage &, const QRect &rect = QRect());
  void saveModifiedImage(int index, const QImage& image);
  void saveModifiedText(int index, const QStringList &text);

  protected:
  enum ChangeType {
    None,
    Binarise,
    Rescale,
  };
  ScanList *m_text_edit;
  OcrImage *m_image_display;
  int m_page_no{};
  DocumentData m_doc_data;
  bool m_image_changed;
  QPushButton *m_crop_btn{};
  QPushButton *m_clr_to_back_btn{};
  QPushButton *m_binarise_btn{};
  QPushButton *m_ocr_btn{};
  QPushButton *m_ocr_sel_btn{};
  QPushButton *denoise_btn{};
  QPushButton *dewarp_btn{}, *m_save_txt_btn{};
  //  QPushButton *deskew_btn{};
  QwtSlider *m_intvalue_slider{};
  QwtSlider *m_dblvalue_slider{};
  QLabel *m_intvalue_lbl{}, *m_dblvalue_lbl{};
  QLabel *m_image_name_lbl{}, *m_image_size_lbl{}, *m_message_lbl{}, *m_res_lbl{};
  QString m_image_name, m_image_size, m_message, m_resolution;
  QStackedLayout *m_ctl_stack{};
  QStatusBar *m_status_bar{};
  int m_btn_stack{};
  int m_intvalue_stack{}, m_dblvalue_stack{};
  ChangeType m_change_type;

  void resizeEvent(QResizeEvent *event) override;

  void setMessageLabel(const QString &text);
  void setSizeLabel(int width, int height);
  void setNameLabel(int page, QString name);
  void setResLabel(int xres, int yres);

  void initGui();
  void requestOcr();
  void requestOcrOnSelection();
  void help();
  void setSelected();
  void setUnselected();
  void crop();
  void binarise();
  void invert();
  void denoise();
  void dewarp();
  void deskew();
  void rotate180();
  void rotateCW();
  void rotateCCW();
  void rescale();
  void saveText();
  void saveImage();
  void discard();
  void acceptChanges();
  void undoChanges();
  void close();

  void setIntValueLabel(qreal value);
  void setDoubleValueLabel(qreal value);
  void valueAccepted();
  void applyValue();
  void aValue();

  void setCurrentValueType();

  void disableBinarise();
  void enableBinarise();

  void enableCleanImageBtns(bool enable);
  void imageSizeHasChanged(int width, int height, int xres, int yres);

  QFrame *initIntSliderFrame();
  QFrame *initDoubleSliderFrame();
  void setResolution(const QImage &image);
};

#endif // OCRDIALOG_H
