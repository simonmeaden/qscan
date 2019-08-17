#ifndef IMAGEEDITDIALOG_H
#define IMAGEEDITDIALOG_H

#include <QDialog>
#include <QImage>
#include <QPixmap>

namespace QScanner {

class ImageEditDialog : public QDialog
{
  Q_OBJECT

    public:
  explicit ImageEditDialog(QWidget* parent = nullptr);
  ~ImageEditDialog() override;

  void setImage(const QImage& image);
  QImage image();

    private:
  QImage m_image;
};

} // end of namespace QScanner

#endif  // IMAGEEDITDIALOG_H
