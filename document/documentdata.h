#ifndef DOCUMENTDATA_H
#define DOCUMENTDATA_H

#include <QMap>
#include <QObject>
#include <QSharedPointer>
#include <QStringList>

#include <yaml-cpp/yaml.h>

#include "styledstring.h"

namespace QScanner {

class DocData
{
public:
  DocData();
  DocData(int page_no, QString filename);
  //  DocData(DocData const& other);
  virtual ~DocData();

  QString filename() const;
  void setFilename(const QString& filename);

  int pageNumber() const;
  void setPageNumber(int pageNumber);

  bool isRemoveImageLater() const;
  void setRemoveImageLater(bool isRemoveImageLater);

  bool isRemoveTextLater() const;
  void setRemoveTextLater(bool isRemoveTextLater);

  virtual bool isDocImage() const;

  bool inverted() const;
  void setInverted(bool inverted);

  int resolution() const;
  void setResolution(int resolution);

  bool hasText();
  bool textWasInitialised() const;
  bool textHasChanged();
  void clearText();
  void clearImages();

  StyledString text(int index) const;
  QString image(int index) const;
  QString imageName(int index) const;

  QMap<int, StyledString> textList();
  QMap<int, QString> imageList();
  QMap<int, QString> imageNames();

  void setText(int index, const StyledString& text);
  void setText(const QMap<int, StyledString>& text_list);
  void setImage(int index,
                const QString& text,
                const QString& name = QString());
  void setImage(const QMap<int, QString>& image_paths);
  void setImageNames(const QMap<int, QString>& image_names);

  int appendText(const StyledString& text);
  //  void appendText(const StyledStringList &text_list);
  void removeText(int index);
  //  bool removeText(const StyledString &text);
  void insertText(int index, const StyledString& text);
  bool isEmpty();

  int appendImage(const QString& image);
  void removeImage(int index);
  void insertImage(int index, const QString& text);

  bool isCompleted() const;
  void setCompleted(bool isCompleted);

protected:
  int m_page_no{};
  QString m_lang;
  QString m_filename;
  bool m_remove_image_later;
  bool m_remove_text_later;
  bool m_inverted;
  bool m_completed;
  int m_resolution{};
  QMap<int, StyledString> m_text_list;
  QMap<int, QString> m_image_list;
  QMap<int, QString> m_image_names;
  bool m_text_has_changed, m_images_changed;
  bool m_text_initialised;
  int highestPage();
};

using DocumentData = QSharedPointer<DocData>;

class DocumentDataStore : public QObject
{
  Q_OBJECT
public:
  explicit DocumentDataStore(QObject* parent = nullptr);

  void appendData(DocumentData& data);
  void insertData(int index, DocumentData& data);

  QList<int> documentKeys();
  DocumentData documentData(int key);

  void load(const QString& filename);
  void save(const QString& filename);

  void remove(int index);
  void remove(const QString& filename);
  void remove(const DocumentData& data);

  void moveKey(int old_page, int new_page);

  int size();

  void cleanUpData();

  bool isEmpty();

  static int nextPageNumber();

  QString lang() const;
  void setLang(const QString& lang);

protected:
  QMap<int, DocumentData> m_data;
  QString m_lang;

  static const QString FILENAME;
  static const QString PAGE_NUMBER;
  //  static const QString INTERNAL_IMAGE;
  static const QString INVERTED;
  static const QString COMPLETED;
  static const QString INTERNAL_IMAGE_NAME;
  static const QString TEXT_LIST;
  static const QString TEXT_STRING;
  static const QString TEXT_STYLES;
  static const QString TEXT_STYLE;
  static const QString TEXT_START;
  static const QString TEXT_LENGTH;
  static const QString IMAGE_LIST;
  static const QString IMAGE_NAMES;
  static const QString LANGUAGE;
  static const QString PAGES;

  static int m_highest_page;
};

} // end of namespace QScanner

Q_DECLARE_METATYPE(QScanner::DocumentData)
Q_DECLARE_METATYPE(QScanner::DocData)

#endif // DOCUMENTDATA_H
