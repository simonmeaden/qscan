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
  DocData(QString  filename,
          const QString& text = QString(),
          bool is_internal = false);
  DocData(const QString& filename,
          const QStringList& text,
          bool is_internal = false);
  bool hasText();

  QString filename() const;
  void setFilename(const QString& filename);

  QString text(int index) const;
  QStringList textList();
  void setText(const QString& text);
  void setText(const QStringList& text_list);
  void appendText(const QString& text);
  void appendText(const QStringList& text_list);
  void removeText(int index);
  bool removeText(const QString& text);
  void insertText(int index, const QString& text);
  bool isEmpty();
  bool textWasInitialised() const;
  //  void setTextWasInitialised(bool textWasInitialised);
  bool textHasChanged();
  void clearText();

  int pageNumber() const;
  void setPageNumber(int pageNumber);

  bool isRemoveImageLater() const;
  void setRemoveImageLater(bool isRemoveImageLater);

  bool isRemoveTextLater() const;
  void setRemoveTextLater(bool isRemoveTextLater);

  bool isInternalImage() const;
  void setIsInternalImage(bool isInternalImage);
  QString internalName() const;
  void setInternalName(const QString &internalName);

  bool inverted() const;
  void setInverted(bool inverted);

  int resolution() const;
  void setResolution(int resolution);

  protected:
  int m_page_no{};
  QString m_filename;
  QString m_internal_name;
  QStringList m_text_list;
  bool m_is_internal_image{};
  bool m_text_has_changed;
  bool m_text_initialised;
  bool m_remove_image_later;
  bool m_remove_text_later;
  bool m_inverted;
  int m_resolution{};
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
  static const QString INTERNAL_IMAGE;
  static const QString INVERTED;
  static const QString INTERNAL_IMAGE_NAME;
  static const QString TEXT_LIST;

  static int m_highest_page;
};

Q_DECLARE_METATYPE(DocData)
Q_DECLARE_METATYPE(DocumentData)

#endif // DOCUMENTDATA_H
