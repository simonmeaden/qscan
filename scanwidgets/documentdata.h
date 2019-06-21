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
  ~DocData();

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

  protected:
  int m_page_no{};
  QString m_filename;
  bool m_remove_image_later;
  bool m_remove_text_later;
  bool m_inverted;
  int m_resolution{};
};

class DocImageData : public DocData
{
  public:
  DocImageData();
  DocImageData(int page_no, const QString &filename);
  ~DocImageData();

  bool isDocImage() const override;
};

class DocOcrData : public DocData
{
  public:
  DocOcrData();
  DocOcrData(int page_no, const QString &filename);
  ~DocOcrData();

  bool hasText();
  bool textWasInitialised() const;
  bool textHasChanged();
  void clearText();
  QString text(int index) const;
  QStringList textList();
  void setText(const QString &text);
  void setText(const QStringList &text_list);
  void appendText(const QString &text);
  void appendText(const QStringList &text_list);
  void removeText(int index);
  bool removeText(const QString &text);
  void insertText(int index, const QString &text);
  bool isEmpty();

  protected:
  QStringList m_text_list;
  bool m_text_has_changed;
  bool m_text_initialised;
};

using DocumentData = QSharedPointer<DocData>;
using OcrData = QSharedPointer<DocOcrData>;
using ImageData = QSharedPointer<DocImageData>;

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
  static const QString INTERNAL_IMAGE;
  static const QString INVERTED;
  static const QString INTERNAL_IMAGE_NAME;
  static const QString TEXT_LIST;

  static int m_highest_page;
};

Q_DECLARE_METATYPE(DocumentData)
Q_DECLARE_METATYPE(DocOcrData)
Q_DECLARE_METATYPE(DocImageData)
Q_DECLARE_METATYPE(DocData)
Q_DECLARE_METATYPE(OcrData)
Q_DECLARE_METATYPE(ImageData)

#endif // DOCUMENTDATA_H
