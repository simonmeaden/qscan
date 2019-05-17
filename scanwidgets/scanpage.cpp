#include "scanpage.h"

ScanPage::ScanPage()
{}

/*!
   \brief returns the image path

   \return a string containing the image path/
*/
QString ScanPage::imagePath() const
{
  return m_image_path;
}

/*!
   \brief Sets the image path

   \param image the image path
*/
void ScanPage::setImagePath(const QString& image)
{
  m_image_path = image;
}

/*!
   \brief returns the test as a string list.

   \return a QStringList.
*/
QStringList ScanPage::text() const
{
  return m_text;
}

/*!
   \brief returns a single line of the text list.

   \param index the row to return.
   \return a QString.
*/
QString ScanPage::text(int index) const
{
  return m_text.at(index);
}

/*!
   \brief Sets the text at supplied row to text value.

  \param text the text to set.
*/
void ScanPage::setText(int row, const QString& text)
{
  if (row >= 0 && row < m_text.size()) {
    m_text.insert(row, text);
  }
}
/*!
   \brief Sets the text as a single line string list.

  Clears existing text.

  \param text the text to set.
*/
void ScanPage::setText(const QString& text)
{
  m_text.clear();
  m_text << text;
}

void ScanPage::setTextList(const QStringList& list)
{
  m_text = list;
}
