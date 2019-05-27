#include "documentdata.h"

/* DocumentDataStore
   ============================================================================*/
DocumentDataStore::DocumentDataStore(QObject* parent) : QObject(parent)
{

}

void DocumentDataStore::appendData(DocumentData& data)
{
  m_data.insert(data->pageNumber(), data);
}

void DocumentDataStore::insertData(int index, DocumentData& data)
{
  m_data.insert(index, data);
}



QList<int> DocumentDataStore::documentKeys()
{
  return m_data.keys();
}

DocumentData DocumentDataStore::documentData(int key)
{
  return m_data.value(key);
}

int DocumentDataStore::size()
{
  return m_data.size();
}

/* DocData
   ============================================================================*/
DocData::DocData() = default;

DocData::DocData(const QString& filename, const QString& text, bool is_internal)
{
  m_filename = filename;
  m_text_list.append(text);
  m_is_internal_image = is_internal;
}

bool DocData::isText()
{
  return (!m_text_list.isEmpty());
}

QString DocData::filename() const
{
  return m_filename;
}

void DocData::setFilename(const QString& filename)
{
  m_filename = filename;
}

QString DocData::text(int index) const
{
  return m_text_list.at(index);
}

QStringList DocData::textList()
{
  return m_text_list;
}

void DocData::setText(const QString& text)
{
  m_text_list.clear();
  appendText(text);
}

void DocData::setText(const QStringList& text_list)
{
  m_text_list = text_list;
}

void DocData::appendText(const QString& text)
{
  m_text_list.append(text);
}

void DocData::appendText(const QStringList& text_list)
{
  m_text_list.append(text_list);
}

void DocData::removeText(int index)
{
  m_text_list.removeAt(index);
}

bool DocData::removeText(const QString& text)
{
  return m_text_list.removeOne(text);
}

void DocData::insertText(int index, const QString& text)
{
  m_text_list.insert(index, text);
}

bool DocData::isInternalImage() const
{
  return m_is_internal_image;
}

void DocData::setIsInternalImage(bool is_internal_image)
{
  m_is_internal_image = is_internal_image;
}

int DocData::pageNumber() const
{
  return m_page_no;
}

void DocData::setPageNumber(int page_no)
{
  m_page_no = page_no;
}
