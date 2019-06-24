#ifndef DOCUMENTDATA_H
#define DOCUMENTDATA_H

#include <QObject>
#include <QStringList>
#include <QSharedPointer>
#include <QMap>

#include "util.h"

class DocData
{
  public:
  DocData();
  DocData(int page_no, QString filename);
  //  ~DocData();

  QString filename() const;
  void setFilename(const QString &filename);

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

  QString text(int index) const;
  QString image(int index) const;

  QMap<int, QString> textList();
  QMap<int, QString> imageList();

  void setText(int index, const QString &text);
  void setText(const QMap<int, QString> &text_list);
  void setImage(int index, const QString &text);
  void setImage(const QMap<int, QString> &text_list);

  int appendText(const QString &text);
  //  void appendText(const QStringList &text_list);
  void removeText(int index);
  //  bool removeText(const QString &text);
  void insertText(int index, const QString &text);
  bool isEmpty();

  int appendImage(const QString &image);
  void removeImage(int index);
  void insertImage(int index, const QString &text);

  protected:
  int m_page_no{};
  QString m_filename;
  bool m_remove_image_later;
  bool m_remove_text_later;
  bool m_inverted;
  int m_resolution{};
  QMap<int, QString> m_text_list;
  QMap<int, QString> m_image_list;
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
  void remove(const DocumentData &data);

  void moveKey(int old_page, int new_page);

  int size();

  void cleanUpData();

  bool isEmpty();

  static int nextPageNumber();

  protected:
  QMap<int, DocumentData> m_data;

  static const QString FILENAME;
  static const QString PAGE_NUMBER;
  //  static const QString INTERNAL_IMAGE;
  static const QString INVERTED;
  static const QString INTERNAL_IMAGE_NAME;
  static const QString TEXT_LIST;
  static const QString IMAGE_LIST;

  static int m_highest_page;
};

Q_DECLARE_METATYPE(DocumentData)
Q_DECLARE_METATYPE(DocData)

#endif // DOCUMENTDATA_H
