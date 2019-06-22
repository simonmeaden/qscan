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
//const QString DocumentDataStore::INTERNAL_IMAGE = "internal image";
const QString DocumentDataStore::INVERTED = "inverted";
const QString DocumentDataStore::INTERNAL_IMAGE_NAME = "internal name";
const QString DocumentDataStore::TEXT_LIST = "text list";
const QString DocumentDataStore::IMAGE_LIST = "image list";

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
          bool inverted = false;
          QMap<int, QString> texts;
          QMap<int, QString> images;

          if (item[FILENAME]) {
            filename = item[FILENAME].as<QString>();
          }

          if (item[INVERTED]) {
            inverted = item[INVERTED].as<bool>();
          }

          if (item[TEXT_LIST]) {
            YAML::Node text_list = item[TEXT_LIST];

            if (text_list.IsMap()) {
              for (YAML::const_iterator it = text_list.begin(); it != text_list.end(); ++it) {
                int key = it->first.as<int>();
                QString text(it->second.as<std::string>().c_str());
                texts.insert(key, Util::cleanText(text));
              }
            }
          }

          if (item[IMAGE_LIST]) {
            YAML::Node image_list = item[IMAGE_LIST];

            if (image_list.IsMap()) {
              for (YAML::const_iterator it = image_list.begin(); it != image_list.end(); ++it) {
                int key = it->first.as<int>();
                QString text(it->second.as<std::string>().c_str());
                images.insert(key, Util::cleanText(text));
              }
            }
          }

          DocumentData doc_data(new DocData(page_number, filename));
          doc_data->setInverted(inverted);
          doc_data->setText(texts);
          doc_data->setImage(images);
          appendData(doc_data);
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

    emitter << YAML::BeginDoc;
    emitter << YAML::BeginMap; // begin of document map

    for (int page_no : documentKeys()) {
      DocumentData doc_data = documentData(page_no);
      emitter << YAML::Key << page_no; // page number as documant key
      emitter << YAML::Value;
      {
        // document data section
        emitter << YAML::BeginMap; // begin of document data map
        emitter << YAML::Key << FILENAME;

        if (doc_data->isRemoveImageLater()) {
          emitter << YAML::Value << QString();

        } else {
          emitter << YAML::Value << doc_data->filename();
        }

        if (doc_data) {
          emitter << YAML::Key << INVERTED;
          emitter << YAML::Value << doc_data->inverted();

          emitter << YAML::Key << TEXT_LIST;

          if (doc_data->isRemoveTextLater()) {
            emitter << YAML::Value << QString();

          } else {
            emitter << YAML::Value;
            emitter << YAML::BeginMap; // begin of text map

            QMap<int, QString> map = doc_data->textList();

            for (auto it = map.keyBegin(), end = map.keyEnd(); it != end; ++it) {
              emitter << YAML::Key << *it;
              emitter << YAML::Value << map.value(*it, QString());
            }

            emitter << YAML::EndMap; // end of text map
          }

          emitter << YAML::Key << IMAGE_LIST;

          emitter << YAML::Value;
          emitter << YAML::BeginMap; // begin of image map

          QMap<int, QString> map = doc_data->imageList();

          for (auto it = map.keyBegin(), end = map.keyEnd(); it != end; ++it) {
            emitter << YAML::Key << *it;
            emitter << YAML::Value << map.value(*it, QString());
          }

          emitter << YAML::EndMap; // end of image map
        }

        emitter << YAML::EndMap; // end of document data map
      }                          // end of document data section
    }

    emitter << YAML::EndMap; // end of document map
    emitter << YAML::EndDoc;

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

    if (doc_data->isRemoveImageLater() && doc_data->isRemoveTextLater()) {
      m_data.remove(page_no);

    } else {
      if (doc_data->isRemoveTextLater()) {
        doc_data->clearText();
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
  , m_text_initialised(false)
  , m_text_has_changed(false)
  , m_images_changed(false)
{}

DocData::DocData(int page_no, QString filename)
  : m_page_no(page_no)
  , m_remove_image_later(false)
  , m_remove_text_later(false)
  , m_filename(std::move(filename))
  , m_inverted(false)
  , m_text_initialised(false)
  , m_text_has_changed(false)
  , m_images_changed(false)
{}

//DocData::~DocData() = default;

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

bool DocData::hasText()
{
  return (!m_text_list.isEmpty());
}

/*!
  \brief Text was initialised using setText().

  \return  true if text was loaded using setText(), otherwise false;
*/
bool DocData::textWasInitialised() const
{
  return m_text_initialised;
}

/*!
  \brief Text has been modified.

  \return  true if text has been modified, otherwise false;
*/
bool DocData::textHasChanged()
{
  return m_text_has_changed;
}

QString DocData::text(int index) const
{
  return m_text_list.value(index);
}

QString DocData::image(int index) const
{
  return m_image_list.value(index);
}

QMap<int, QString> DocData::textList()
{
  return m_text_list;
}

QMap<int, QString> DocData::imageList()
{
  return m_image_list;
}

void DocData::setText(int index, const QString &text)
{
  if (!m_text_list.isEmpty()) {
    m_text_initialised = true;
  }

  QString cleaned = Util::cleanText(text);
  m_text_list.clear();
  m_text_list.insert(index, cleaned);
}

void DocData::setText(const QMap<int, QString> &text_list)
{
  if (!m_text_list.isEmpty()) {
    m_text_initialised = true;
  }

  // don't need to clean as this was already done.

  m_text_list = text_list;
}

void DocData::setImage(int index, const QString &text)
{
  m_image_list.clear();
  m_image_list.insert(index, text);
}

void DocData::setImage(const QMap<int, QString> &text_list)
{
  m_image_list = text_list;
}

int DocData::appendText(const QString &text)
{
  QList<int> page_nos = m_text_list.keys();
  std::sort(page_nos.begin(), page_nos.end());
  int max_page = page_nos.last();

  m_text_list.insert(++max_page, text);
  m_text_has_changed = true;
  return max_page;
}

int DocData::appendImage(const QString &image)
{
  QList<int> page_nos = m_image_list.keys();
  std::sort(page_nos.begin(), page_nos.end());
  int max_page = page_nos.last();

  m_text_list.insert(++max_page, image);
  m_images_changed = true;
  return max_page;
}

//void DocData::appendText(const QStringList& text_list)
//{
//  m_text_list.append(text_list);
//  m_text_has_changed = true;
//}

void DocData::removeText(int index)
{
  m_text_list.remove(index);
  m_text_has_changed = true;
}

void DocData::removeImage(int index)
{
  m_image_list.remove(index);
  m_images_changed = true;
}

//bool DocData::removeText(const QString& text)
//{
//  return m_text_list.removeOne(text);
//  m_text_has_changed = true;
//}

void DocData::insertText(int index, const QString &text)
{
  m_text_list.insert(index, text);
  m_text_has_changed = true;
}

void DocData::insertImage(int index, const QString &text)
{
  m_image_list.insert(index, text);
  m_images_changed = true;
}

/*!
   \brief no text has been added to document data.

  \return true if no text has been added, otherwise false.
*/
bool DocData::isEmpty()
{
  return m_text_list.isEmpty();
}

void DocData::clearText()
{
  m_text_list.clear();
}

void DocData::clearImages()
{
  m_image_list.clear();
}
