#ifndef DOCUMENTDATA_H
#define DOCUMENTDATA_H

#include <QObject>
#include <QStringList>
#include <QSharedPointer>
#include <QMap>

class DocData
{
public:
  DocData();
  DocData(const QString& filename,
          const QString& text = QString(),
          bool is_internal = false);
  DocData(const QString& filename,
          const QStringList& text,
          bool is_internal = false);
  bool isText();

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

  bool isInternalImage() const;
  void setIsInternalImage(bool isInternalImage);

  int pageNumber() const;
  void setPageNumber(int pageNumber);

protected:
  int m_page_no;
  QString m_filename;
  QStringList m_text_list;
  bool m_is_internal_image{};
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

  int size();

signals:

protected:
  QMap<int, DocumentData> m_data;
};

#endif // DOCUMENTDATA_H
