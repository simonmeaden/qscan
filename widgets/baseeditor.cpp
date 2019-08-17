#include "baseeditor.h"

namespace QScanner {

const char* BaseEditor::PNG = "PNG";

BaseEditor::BaseEditor(QWidget* parent)
  : StackableFrame(parent)
{
  QDir dir;
  //  QString config_path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
  // TODO remove temporary configuration path which equates to "/home/simonmeaden/.config/Scanner"
  // in the Scanner test application.
  m_config_dir = "/home/simonmeaden/.config/Biblos";
  dir.mkpath(m_config_dir);

  //  m_data_dir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
  // TODO remove temporary data store which equates to "/home/simonmeaden/.local/share/Scanner//"
  // in the Scanner test application.
  m_data_dir = "/home/simonmeaden/.local/share/Biblos/ocr";
  dir.mkpath(m_data_dir);

  m_lang = "eng";
}

void BaseEditor::saveImage(const QImage& image, const QString& filename)
{
  image.save(filename, PNG);
}

void BaseEditor::setLang(const QString& lang)
{
  m_lang = lang;
}

} // end of namespace QScanner
