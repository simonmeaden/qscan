#include <utility>

#include <utility>

#include "documentdata.h"

#include <algorithm>

//#include <qyaml-cpp/QYamlCpp>
#include "QYamlCpp"
#include <yaml-cpp/yaml.h>

/* DocumentDataStore
   ============================================================================*/

const QString DocumentDataStore::FILENAME = "filename";
const QString DocumentDataStore::PAGE_NUMBER = "page number";
const QString DocumentDataStore::INTERNAL_IMAGE = "internal image";
const QString DocumentDataStore::INVERTED = "inverted";
const QString DocumentDataStore::INTERNAL_IMAGE_NAME = "internal name";
const QString DocumentDataStore::TEXT_LIST = "text list";

int DocumentDataStore::m_highest_page = -1;

DocumentDataStore::DocumentDataStore(QObject* parent)
  : QObject(parent)
{}

int DocumentDataStore::nextPageNumber()
{
  return ++m_highest_page;
}

void DocumentDataStore::appendData(DocumentData &data)
{
  int page_no = data->pageNumber();
  m_data.insert(page_no, data);

  if (page_no > m_highest_page) {
    m_highest_page = page_no;
  }
}

void DocumentDataStore::insertData(int index, DocumentData &data)
{
  int page_no = data->pageNumber();
  m_data.insert(index, data);

  if (page_no > m_highest_page) {
    m_highest_page = page_no;
  }
}

QList<int> DocumentDataStore::documentKeys()
{
  return m_data.keys();
}

DocumentData DocumentDataStore::documentData(int key)
{
  return m_data.value(key);
}

void DocumentDataStore::load(const QString &filename)
{
  QFile file(filename);

  if (file.exists()) {
    YAML::Node doc_data = YAML::LoadFile(filename);

    if (!doc_data.IsNull()) {
      if (doc_data.IsMap()) {
        for (YAML::const_iterator it = doc_data.begin(); it != doc_data.end(); ++it) {
          int page_number = it->first.as<int>();
          YAML::Node item = it->second;
          QString filename;
          bool is_internal = false, inverted = false;
          QStringList texts;

          if (item[FILENAME]) {
            filename = item[FILENAME].as<QString>();
          }

          if (item[INTERNAL_IMAGE]) {
            is_internal = item[INTERNAL_IMAGE].as<bool>();
          }

          if (item[INVERTED]) {
            inverted = item[INVERTED].as<bool>();
          }

          if (item[TEXT_LIST]) {
            YAML::Node text_list = item[TEXT_LIST];

            if (text_list) {
              for (auto &&text_node : text_list) {
                QString text = text_node.as<QString>();
                texts.append(text);
              }
            }
          }

          if (is_internal) {
            DocumentData data(new DocImageData(page_number, filename));
            appendData(data);

          } else {
            DocumentData data(new DocOcrData(page_number, filename));
            OcrData doc_data = data.dynamicCast<DocOcrData>();
            doc_data->setInverted(inverted);
            doc_data->setText(texts);
            appendData(data);
          }
        }
      }
    }
  }
}

void DocumentDataStore::save(const QString &filename)
{
  QFile file(filename);

  if (file.open(QFile::WriteOnly | QFile::Truncate)) {
    YAML::Emitter emitter;

    emitter << YAML::BeginMap; // begin of document map

    for (int page_no : documentKeys()) {
      DocumentData data = documentData(page_no);
      emitter << YAML::Key << page_no; // page number as documant key
      emitter << YAML::Value;
      {
        // document data section
        emitter << YAML::BeginMap; // begin of document data map
        emitter << YAML::Key << FILENAME;

        if (data->isRemoveImageLater()) {
          emitter << YAML::Value << QString();

        } else {
          emitter << YAML::Value << data->filename();
        }

        OcrData ocr_data = data.dynamicCast<DocOcrData>();

        if (ocr_data) {
          emitter << YAML::Key << INTERNAL_IMAGE;
          emitter << YAML::Value << false;

          emitter << YAML::Key << INVERTED;
          emitter << YAML::Value << ocr_data->inverted();

          emitter << YAML::Key << TEXT_LIST;

          if (data->isRemoveTextLater()) {
            emitter << YAML::Value << QString();

          } else {
            emitter << YAML::Value;
            emitter << YAML::BeginSeq; // begin of text sequance

            for (const QString &text : ocr_data->textList()) {
              emitter << YAML::Value << text;
            }

            emitter << YAML::EndSeq; // end of text sequance
          }

        } else {
          emitter << YAML::Key << INTERNAL_IMAGE;
          emitter << YAML::Value << true;
        }

        emitter << YAML::EndMap; // end of document data map
      }                          // end of document data section
    }

    emitter << YAML::EndMap; // end of document map

    QTextStream out(&file);
    out << emitter.c_str();
    file.close();
  }
}

void DocumentDataStore::remove(int index)
{
  m_data.remove(index);
}

void DocumentDataStore::remove(const QString &filename)
{
  QList<DocumentData> values = m_data.values();

  QList<DocumentData>::iterator data_it = std::find_if(values.begin(),
                                                       values.end(),
                                                       [filename](DocumentData data) {
                                                         return data->filename() == filename;
                                                       });

  if (!data_it->isNull()) {
    m_data.remove(m_data.key(*data_it));
  }
}

void DocumentDataStore::remove(const DocumentData &data)
{
  m_data.remove(m_data.key(data));
}

/*!
   \brief Moves a DocData object to a different page number.

   \param start the initial page number
   \param finish the new page number
*/
void DocumentDataStore::moveKey(int old_page, int new_page)
{
  m_data.insert(new_page, m_data.value(old_page));
  m_data.remove(old_page);
}

int DocumentDataStore::size()
{
  return m_data.size();
}

void DocumentDataStore::cleanUpData()
{
  QList<int> keys = m_data.keys();

  for (int page_no : keys) {
    DocumentData doc_data = m_data.value(page_no);
    OcrData ocr_data = doc_data.dynamicCast<DocOcrData>();

    if (ocr_data) {
      if (ocr_data->isRemoveImageLater() && ocr_data->isRemoveTextLater()) {
        m_data.remove(page_no);

      } else {
        if (ocr_data->isRemoveTextLater()) {
          ocr_data->clearText();
        }
      }
    }
  }
}

bool DocumentDataStore::isEmpty()
{
  return m_data.isEmpty();
}

/* DocData
   ============================================================================*/

DocData::DocData()
  : m_page_no(-1)
  , m_remove_image_later(false)
  , m_remove_text_later(false)
  , m_filename(QString())
  , m_inverted(false)
{}

DocData::DocData(int page_no, QString filename)
  : m_page_no(page_no)
  , m_remove_image_later(false)
  , m_remove_text_later(false)
  , m_filename(std::move(filename))
  , m_inverted(false)
{}

DocData::~DocData() = default;

QString DocData::filename() const
{
  return m_filename;
}

void DocData::setFilename(const QString &filename)
{
  m_filename = filename;
}

int DocData::pageNumber() const
{
  return m_page_no;
}

void DocData::setPageNumber(int page_no)
{
  m_page_no = page_no;
}

bool DocData::isRemoveImageLater() const
{
  return m_remove_image_later;
}

void DocData::setRemoveImageLater(bool remove_later)
{
  m_remove_image_later = remove_later;
}

bool DocData::isRemoveTextLater() const
{
  return m_remove_text_later;
}

void DocData::setRemoveTextLater(bool remove_text_later)
{
  m_remove_text_later = remove_text_later;
}

bool DocData::isDocImage() const
{
  return false;
}

bool DocData::inverted() const
{
  return m_inverted;
}

void DocData::setInverted(bool inverted)
{
  m_inverted = inverted;
}

int DocData::resolution() const
{
  return m_resolution;
}

void DocData::setResolution(int resolution)
{
  m_resolution = resolution;
}

/* DocOcrData
   ============================================================================*/

DocOcrData::DocOcrData()
  : DocData()
  , m_text_initialised(false)
  , m_text_has_changed(false)
{}

DocOcrData::DocOcrData(int page_no, const QString &filename)
  : DocData(page_no, filename)
  , m_text_initialised(false)
  , m_text_has_changed(false)
{}

DocOcrData::~DocOcrData() = default;

bool DocOcrData::hasText()
{
  return (!m_text_list.isEmpty());
}

/*!
  \brief Text was initialised using setText().

  \return  true if text was loaded using setText(), otherwise false;
*/
bool DocOcrData::textWasInitialised() const
{
  return m_text_initialised;
}

/*!
  \brief Text has been modified.

  \return  true if text has been modified, otherwise false;
*/
bool DocOcrData::textHasChanged()
{
  return m_text_has_changed;
}

QString DocOcrData::text(int index) const
{
  return m_text_list.at(index);
}

QStringList DocOcrData::textList()
{
  return m_text_list;
}

void DocOcrData::setText(const QString &text)
{
  if (!m_text_list.isEmpty()) {
    m_text_initialised = true;
  }

  m_text_list.clear();
  appendText(text);
}

void DocOcrData::setText(const QStringList &text_list)
{
  if (!m_text_list.isEmpty()) {
    m_text_initialised = true;
  }

  m_text_list = text_list;
}

void DocOcrData::appendText(const QString &text)
{
  m_text_list.append(text);
  m_text_has_changed = true;
}

void DocOcrData::appendText(const QStringList &text_list)
{
  m_text_list.append(text_list);
  m_text_has_changed = true;
}

void DocOcrData::removeText(int index)
{
  m_text_list.removeAt(index);
  m_text_has_changed = true;
}

bool DocOcrData::removeText(const QString &text)
{
  return m_text_list.removeOne(text);
  m_text_has_changed = true;
}

void DocOcrData::insertText(int index, const QString &text)
{
  m_text_list.insert(index, text);
  m_text_has_changed = true;
}

/*!
   \brief no text has been added to document data.

  \return true if no text has been added, otherwise false.
*/
bool DocOcrData::isEmpty()
{
  return m_text_list.isEmpty();
}

void DocOcrData::clearText()
{
  m_text_list.clear();
}

/* DocImageData
   ============================================================================*/
DocImageData::DocImageData()
  : DocData()
{}

DocImageData::DocImageData(int page_no, const QString &filename)
  : DocData(page_no, filename)
{}

DocImageData::~DocImageData() = default;

bool DocImageData::isDocImage() const
{
  return true;
}
