#ifndef BASEEDITOR_H
#define BASEEDITOR_H

#include <QWidget>
#include <QString>
#include <QDir>

#include "stackableframe.h"

namespace QScanner {

class BaseEditor : public StackableFrame
{
public:
  BaseEditor(QWidget* parent = nullptr);

  void saveImage(const QImage& image, const QString& filename);

  static const char* PNG;

  void setLang(const QString& lang);

protected:
  QString OPTIONS_FILE;
  QString m_options_file;
  QString m_data_dir;
  QString m_config_dir;
  QString m_lang;

};

} // end of namespace QScanner

#endif // BASEEDITOR_H
